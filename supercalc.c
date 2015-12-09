#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "supercalc.h"
#include "main.h"
/*----------SUPERCALC---------------*/
int superCalc(int precision,char* input,char* output)
{
    /*Variables*/
    operation_t** operations;
    int cantOp=0,i;
    int bufferLength;
    FILE* fp=NULL;
    char* buffer;
    /*precision minima = Longitud de "#calculate" +\n+\0*/
    if (precision*2+5<strlen(CALCULATE_TOLKEN)) /* la longitud del buffer va a ser mas chica que eso, redefino tamaño buffer*/
        bufferLength = strlen(CALCULATE_TOLKEN)+2; /* length + \0+\n*/
    else
        bufferLength = precision*2+5; /* si no lo dejo como num1+num2+neg1+neg2+op+\n+\0 MAXIMO*/
    buffer=(char*)calloc(bufferLength,sizeof(char));
    if (buffer==NULL) /* error de memoria*/
    {
        printf(MSG_ERROR_MEMORY);
        return EXIT_FAILURE;
    }
    if (input != NULL)  /* si hay input, abrirlo */
    {
        fp = fopen(input,"r");
        if (fp == NULL){
            printf(MSG_ERROR_ARG);
            return EXIT_FAILURE;
        }
    }
    else    /* si no , trabajar con stdin*/
        fp = stdin;
    while (strncmp(fgets(buffer,bufferLength,fp),CALCULATE_TOLKEN,strlen(CALCULATE_TOLKEN)) != 0){
    /* Mientras no reciba #calculate...*/
        if (addOperation(&operations,&cantOp) == NULL) /* agrego espacio para una operacion y me fijo que no halla MEM_ERR*/
        {
            printf(MSG_ERROR_MEMORY);
            return EXIT_FAILURE;
        }
        if (buffer == NULL) /* si buffer es null, entonces recibio un string mas grande que su longitud*/
            (*(operations[cantOp-1])).st = OFW; /*Overflow*/
        else
            parseOperation(buffer,operations,cantOp,precision); /* interpreto operacion */
    }
    fclose(fp); /* cierro archivo de entrada, si es stdin no pasa nada */
    free(buffer); /* libero el buffer ya que no lo uso mas*/
    /*SOLVE OPERATIONS*/
    for(i=0;i<cantOp;i++)   /* resuelvo las operaciones */
    {
        solveOperation(operations[i],precision);
    }
    if(output != NULL) /* si dieron archivo de salida */
    {
        fp = fopen(output,"w"); /* lo creo o sobreescribo */
        if (fp == NULL)         /* si el puntero es nulo, entonces no lo puedo abrir o crear */
        {
            printf(MSG_ERROR_ARG); /* error y salgo */
            return EXIT_FAILURE;
        }
    }
    else /* si no me dieron archivo de salida, imprimo en pantalla */
        fp = stdout;
    for(i=0;i<cantOp;i++) /* imprimo operaciones: */
    {
        if ((*(operations[i])).st == OK) { /* si el estado es OK, imprimo resultado con signo*/
            if ((*(operations[i])).signAns == TRUE)
                fprintf(fp,"-");
            printListBackwards((*(operations[i])).ans,fp);
            fprintf(fp,"\n");
        }
        else if ((*(operations[i])).st == OFW) /* si el estado es OFW, imprimo infinito */
            fprintf(fp,MSG_INFINITY);
        else if ((*(operations[i])).st == ERR) /* si hubo error de memoria durante la operacion, imprimo error de mem*/
            fprintf(fp,MSG_ERROR_MEMORY);
    }
    fclose(fp); /* cierro archivo de salida, si es stdout no pasa nada */
    for(i=0;i<cantOp;i++) /* libero: */
    {
        freeList((*(operations[i])).num1); /* lista 1*/
        freeList((*(operations[i])).num2); /* lista 2*/
        freeList((*(operations[i])).ans);  /* lista ans */
        free(operations[i]);               /*estructura de operacion*/
    }
    free(operations); /* vector de punteros a operacion */
    return EXIT_SUCCESS;
}
/*------------ AGREGAR OPERACION--------------*/
operation_t** addOperation(operation_t*** operations,int* cantOp)
{
    /*VARIABLES*/
    operation_t **aux=NULL;
    operation_t *aux2=NULL;
    /*MEMORIA PARA UNA EL PUNTERO DE UNA OPERACION*/
    if (*cantOp == 0) /*si nunca crearon la memoria*/
    {
        aux = (operation_t**)calloc(1,sizeof(operation_t*)); /* Creo memoria*/
        if (aux == NULL)        /* Si Hubo error de memoria*/
        {
            printf(MSG_ERROR_MEMORY);
            return NULL;
        }
        else    /* si no hubo error de memoria*/
        {
            *operations = aux;       /*guardar la memoria*/
            (*cantOp)++;               /*sumar una operacion*/
        }
    }
    else        /*Si ya hay memoria*/
    {
        aux = (operation_t**)realloc(*operations,sizeof(operation_t*)*(*cantOp+1)); /*expando memoria*/
        if (aux == NULL) /* Si Hubo error de memoria*/
            {
            printf(MSG_ERROR_MEMORY);
            return NULL;
        }
        else
        {
            *operations = aux;       /*guardar la memoria*/
            (*cantOp)++;               /*sumar una operacion*/

        }
    }
    /*MEMORIA PARA UNA OPERACION*/
    aux2 = (operation_t*)calloc(1,sizeof(operation_t));  /*agrego una operacion y la guardo en la memoria expandida*/
    if (aux2 == NULL)
    {
        printf(MSG_ERROR_MEMORY);
        return NULL;
    }
    else
    {
        (*operations)[*cantOp-1] = aux2;
    }
return *operations;
}
/*--------INTERPRETAR OPERACION------------*/
int parseOperation(char* buffer,operation_t** operations,int cantOp,int precision)
{
    int i=0,j=0;
    /*INICIALIZO OPERACION*/
    (*(operations[cantOp-1])).num1 = NULL;
    (*(operations[cantOp-1])).num2 = NULL;
    (*(operations[cantOp-1])).ans = NULL;
    (*(operations[cantOp-1])).sign1 = FALSE;
    (*(operations[cantOp-1])).sign2 = FALSE;
    (*(operations[cantOp-1])).signAns = FALSE;
    /*SIGNO PRIMER NUMERO*/
    if (buffer[i] == SUB_CHAR) /* si hay signo menos */
    {
        (*(operations[cantOp-1])).sign1 = TRUE;
        i++;
    }
    /*PRIMER NUMERO*/
    for(j=0;buffer[i] !=ADD && buffer[i] !=SUB && buffer[i] !=MUX;j++) /* leo hasta que aparezca una operacion*/
    {
        addValue(&(*(operations[cantOp-1])).num1, buffer[i++]-ZERO_CHAR); /* agrego a la lista*/
    }
    /*OPERACION*/
    switch(buffer[i]) /* reconoce operacion */
    {
        case ADD:
        {
            (*(operations[cantOp-1])).op = ADD;
            break;
        }
        case SUB:
        {
            (*(operations[cantOp-1])).op = SUB;
            break;
        }
        default:
        {
            (*(operations[cantOp-1])).op = MUX;
            break;
        }
    }
    i++;
    /*SIGNO SEGUNDO NUMERO*/
    if (buffer[i] == SUB_CHAR) /* si hay signo menos */
    {
        (*(operations[cantOp-1])).sign2 = TRUE;
        i++;
    }
    /*SEGUNDO NUMERO*/
    for(j=0;i<strlen(buffer)-1 ;j++) /* leo hasta el final del buffer -\n*/
    {
        addValue(&(*(operations[cantOp-1])).num2, buffer[i++]-ZERO_CHAR);
    }
    return EXIT_SUCCESS;
}
/*---IMPRIMIR LISTA--------*/
void printList(t_nodo* list,FILE* file)
{
    do{
        fprintf(file,"%hi",list->val); /*imprimir valor*/
        list=list->sig;                /*ir al siguiente */
    }while (list != NULL);              /* mientras no termine*/
}
/*---IMPRIMIR LISTA AL REVES--------*/
void printListBackwards(t_nodo* list,FILE* file)
{
    while (list->sig != NULL) /*recorre hasta el final*/
        list=list->sig;
    while (list->val == 0 && list->ant != NULL) /* mientras el valor sea 0, lo salteo (0 a la izda)*/
        list = list->ant;
    while (list != NULL)    /*mientras no termine*/
    {
        fprintf(file,"%hi",list->val);  /*imprimo valor*/
        list=list->ant;             /*voy al siguiente*/
    }
}
/*---LIBERAR LISTA-----------*/
void freeList(t_nodo* list)
{
    if (list!=NULL) /* si la lista no es null*/
    {
        while(list->sig != NULL) /*recorre hasta el final*/
        {
            list=list->sig;
        }
        while(list->ant != NULL) /*va recorriendo hasta el principio y libera*/
        {
            list = list->ant;
            free(list->sig);
        }
        free(list); /* libero el ultimo*/
    }
}
/*---AGREGAR A LA LISTA---*/
int addValue(t_nodo** listPointer,short value)
{
    t_nodo* list;
    list = *listPointer;
    if (list == NULL) /* Lista creada?*/
    {
        list = (t_nodo*)calloc(1,sizeof(t_nodo)); /*pido memoria*/
        if (list == NULL) /* error de memoria */
        {
            printf(MSG_ERROR_MEMORY);
            return EXIT_FAILURE;
        }
        list->ant = NULL; /*puede ser redundante*/
    }
    else
    {
        while (list->sig != NULL) /*recorre hasta el final*/
            list=list->sig;
        list->sig =(t_nodo*)calloc(1,sizeof(t_nodo)); /* pido memoria*/
        if (list->sig == NULL) /*error de memoria*/
        {
            printf(MSG_ERROR_MEMORY);
            return EXIT_FAILURE;
        }
        list->sig->ant = list;
        list=list->sig;
    }
    list->sig = NULL; /*puede ser redundante*/
    list->val = value;
    while (list->ant != NULL) /*recorre hasta el final*/
        list=list->ant;
    *listPointer = list;
    return EXIT_SUCCESS;
}
/*-------------RESOLVER OPERACION--------------*/
void solveOperation(operation_t* oper,int precision)
{
    switch ((*oper).op)
    {
        case SUB:
        {/*cambio el signo de la segunda operacion y voy a ADD*/
            if ((*oper).sign2==TRUE)
                (*oper).sign2 = FALSE;
            else (*oper).sign2 = TRUE;
        }
        case ADD:
        {
            (*oper).st = addition(oper,precision);
            break;
        }
        case MUX:
        {
            /*SIGNO*/
            (*oper).signAns = (*oper).sign2 ^ (*oper).sign1;
            (*oper).st = multiply(oper,precision);
            break;
        }
        default:
            {
                printf(MSG_ERROR_OPERATION);
                (*oper).st = ERR;
            }
    }
}
/*-------ADD NUMBERS ---------------*/
result_state_t addNumbers(operation_t* oper,int precision)
{
    int i,carry=0, resultado;
    t_nodo *lista1, *lista2, *ans=NULL;
    lista1=(*oper).num1; /*se usan otros nombres para no modificar el puntero original*/
    lista2=(*oper).num2;
    while (lista1->sig != NULL) /*recorre hasta el final*/
        lista1=lista1->sig;
    while (lista2->sig != NULL) /*recorre hasta el final*/
        lista2=lista2->sig;
    while (lista1 != NULL || lista2 != NULL) /* mientras alguno de los 2 siga teniendo numeros*/
    {
        if (lista1 == NULL){                /* si solo lista2 tiene numeros*/
            resultado = lista2->val+carry;  /* dejo el numero y sumo carry*/
            lista2=lista2->ant;             /* voy al siguiente*/
        }
        else if (lista2==NULL){             /* si solo lista1 tiene numeros */
            resultado = lista1->val+carry;  /* dejo el numero y sumo carry*/
            lista1=lista1->ant;             /* voy al siguiente*/
        }
        else{ /* si los 2 tienen numeros */
            resultado = lista1->val + lista2->val + carry; /*n1+n2+carry*/
            lista2=lista2->ant;                             /*paso a los siguientes*/
            lista1=lista1->ant;
        }
        if (resultado > 9) {                    /* si hay carry */
            carry = resultado / 10;
            resultado = resultado % 10;
        }
        else        /* si no hay carry */
            carry = 0;
        if (addValue(&ans,resultado) == EXIT_FAILURE) /*si hay error de adicion*/
            return ERR;
    }
    if (carry != 0) /* si me quedo carry dando vueltas*/
        addValue(&ans,carry);
    (*oper).ans = ans;
    i=0;
    while (ans != NULL) { /*recorre hasta el final y lo cuenta*/
        i++;
        ans=ans->sig;
    }
    if (i>precision) /* si la cuenta supera la precision => OFW*/
        return OFW;
    return OK;
}
/*----------SUMAS--------*/
result_state_t addition(operation_t* oper,int precision)
{

    /*si los 2 son - o +, se suman los numeros y se pone el signo que corresponda*/
    if ((*oper).sign1 == TRUE && (*oper).sign2 == TRUE)
    {
        (*oper).signAns = TRUE;
        return addNumbers(oper,precision);
    }
    if ((*oper).sign1 == FALSE && (*oper).sign2 == FALSE)
    {
        (*oper).signAns = FALSE;
        return addNumbers(oper,precision);
    }
    /*si no se restan*/
    return substraction(oper,precision);
    return ERR;
}
/*----SUPERIOR DE NUM1 y NUM2------*/
int superior(operation_t* oper)
{
    int i=1,j=1;
    t_nodo *listai, *listaj;
    listai=(*oper).num1; /* se usan otros nombres para no modificar el puntero original*/
    listaj=(*oper).num2;
    while (listai->sig != NULL) { /*recorre hasta el final y cuenta*/
        listai=listai->sig;
        i++;
    }
    while (listaj->sig != NULL) { /*recorre hasta el final y cuenta */
        listaj=listaj->sig;
        j++;
    }
    if (i < j)  /*el que tenga mas numeros, es el mayor*/
        return 2;
    if (i > j)
        return 1;
    listai=(*oper).num1;
    listaj=(*oper).num2;
    while (listai != NULL){ /* si tienen la misma cantidad, me fijo uno por uno*/
        if (listai->val < listaj->val )
            return 2;
        if (listai->val > listaj->val)
            return 1;
        listai=listai->sig;
        listaj=listaj->sig;
    }
    return 0;
}
/*-----RESTAR NUMEROS---------*/
result_state_t subNumbers(operation_t* oper,int precision)
{
    int i,borrow=0,resultado;
    t_nodo *lista1, *lista2, *ans=NULL;
    if (superior(oper) == 2) /* el mayor lo pongo en lista1 y el menor en lista2*/
    {
        lista1=(*oper).num2;
        lista2=(*oper).num1;
    }
    else {
        lista1=(*oper).num1;
        lista2=(*oper).num2;
    }
    while (lista1->sig != NULL) /*recorre hasta el final*/
        lista1=lista1->sig;
    while (lista2->sig != NULL) /*recorre hasta el final*/
        lista2=lista2->sig;

    while (lista1 != NULL || lista2 != NULL) /* mismo concepto que en la suma */
    {
        if (lista1 == NULL){
            resultado = lista2->val - lista2->val - lista2->val - borrow;
            lista2=lista2->ant;
        }
        else if (lista2==NULL){
            resultado = lista1->val-borrow;
            lista1=lista1->ant;
        }
        else{
            resultado = lista1->val - lista2->val - borrow;
            lista2=lista2->ant;
            lista1=lista1->ant;
        }
        if (resultado < 0) {
            borrow = 1;
            resultado += 10;
        }
        else
            borrow = 0;
        if (addValue(&ans,resultado) == EXIT_FAILURE)
            return ERR;
    }
    (*oper).ans = ans;
    i=0;
    while (ans != NULL) { /*recorre hasta el final y cuenta*/
        i++;
        ans=ans->sig;
    }
    if (i>precision)
        return OFW;
    return OK;
}
/*-----RESTA-------*/
result_state_t substraction(operation_t* oper,int precision)
{

    (*oper).signAns = FALSE; /*asumo resultado positivo*/
    switch (superior(oper))
    {
        case 1: /* num1 > num2*/
        {
            if ((*oper).sign1 == TRUE)
                (*oper).signAns = TRUE;
            return subNumbers(oper,precision);
        }
        case 2: /* num2>num1 */
        {
            if ((*oper).sign2 == TRUE)
                (*oper).signAns = TRUE;
            return subNumbers(oper,precision);
        }
        default: /* iguales */
        {
            (*oper).signAns = FALSE;
            addValue(&((*oper).ans),0);
            return OK;
        }
    }
}
/*---MULTIPLICAR---*/
result_state_t multiply(operation_t* oper,int precision) {
    int suma,i,j;
    int lista1Length=1,lista2Length=1;
    short* bufferAns=NULL;
    t_nodo *lista1, *lista2, *ans=NULL;
    lista1=(*oper).num1;
    lista2=(*oper).num2;
    while (lista1->sig != NULL) { /*recorre hasta el final*/
        lista1=lista1->sig;
        lista1Length++;
    }
    while (lista2->sig != NULL) { /*recorre hasta el final*/
        lista2=lista2->sig;
        lista2Length++;
    }
    if (lista1Length+lista2Length-1>precision)
        return OFW;
    bufferAns = (short*)calloc(precision,sizeof(short));
    if (bufferAns == NULL)
        return ERR;
    for (j=0; lista2!=NULL;j++,lista2=lista2->ant) /*RECORRO VECTOR 2*/
    {
        lista1=(*oper).num1;
        while (lista1->sig != NULL)  /*recorre hasta el final*/
            lista1=lista1->sig;
        suma = 0;
        for (i=0;lista1!=NULL;i++,lista1=lista1->ant)/*RECORRO VECTOR 1*/
        {
            suma = (lista1->val)*(lista2->val) + bufferAns[i+j] + suma/10;
            bufferAns[i+j] = suma%10;
        }
        if (lista2->ant != NULL)
            bufferAns [j+i] = suma/10;
    }
    if (suma/10)
    {
        if (lista1Length+lista2Length-1 < precision)
            bufferAns[lista1Length+lista2Length -1] = suma/10;
        else{
            free(bufferAns);
            return OFW;
        }
    }
    for(i=0;i<precision;i++){
        addValue(&ans,bufferAns[i]);
    }
    (*oper).ans = ans;
    free(bufferAns);
    return OK;
}

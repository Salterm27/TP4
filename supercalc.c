#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "supercalc.h"
#include "main.h"
/*----------SUPERCALC---------------*/
int superCalc(int precision,char* input,char* output)
{
    /* Definicion de variables*/
    operation_t** operations;
    int cantOp=0,i;
    int bufferLength;
    FILE* fp=NULL;
    char* buffer=NULL;

    if (precision*2+5<strlen(CALCULATE_TOLKEN)) /*precision minima = #calculate*/
        bufferLength = strlen(CALCULATE_TOLKEN)+2; /* length+\n+\0 */
    else
        bufferLength = precision*2+5; /*maximo num1+num2+op+sign1+sign2+\n+\0*/
    buffer=(char*)calloc(bufferLength,sizeof(char)); /*defino el buffer */
    if (buffer==NULL) /*error de malloc*/
    {
        printf(MSG_ERROR_MEMORY);
        return EXIT_FAILURE;
    }
    if (input != NULL) /* Si no hay input, fp es stdin, si no fp es el archivo*/
    {
        fp = fopen(input,"r");
        if (fp == NULL)
        {
            printf(MSG_ERROR_ARG);
            return EXIT_FAILURE;
        }
    }
    else
        fp = stdin;
    while (strncmp(fgets(buffer,bufferLength,fp),CALCULATE_TOLKEN,strlen(CALCULATE_TOLKEN)) != 0){
    /*mientras fgets sea distinto a #calculate*/
        if (addOperation(&operations,&cantOp) == NULL) /* si addoperations = NULL, hubo error de memoria*/
        {
            printf(MSG_ERROR_MEMORY);
        }
        if (buffer == NULL) /*si el buffer es null, es porque ingresaron linea mayor al permitido -> OFW*/
            (*(operations[cantOp-1])).st = OFW;
        else
            parseOperation(buffer,operations,cantOp,precision); /* interpreto el buffer y lo guardo en estructura*/
    }
    if (fp != stdin)
        fclose(fp);
    free(buffer);
    /*SOLVE OPERATIONS*/
    for(i=0;i<cantOp;i++) /* resuelvo las operaciones 1x1*/
    {
        solveOperation(operations[i],precision);
    }
    if(output != NULL) /* si hay output, fp es el archivo, si no, fp es stdin*/
    {
        fp = fopen(output,"w");
        if (fp == NULL) /* error de archivo */
        {
            printf(MSG_ERROR_ARG);
            return EXIT_FAILURE;
        }
    }
    else
        fp = stdout;
    for(i=0;i<cantOp;i++) /* imprimo las operaciones en fp */
    {
        if ((*(operations[i])).st == OK) {
            if ((*(operations[i])).signAns == TRUE)
                fprintf(fp,"-");
            printListBackwards((*(operations[i])).ans,fp);
            fprintf(fp,"\n");
        }
        else if ((*(operations[i])).st == OFW)
            fprintf(fp,MSG_INFINITY);
        else if ((*(operations[i])).st == ERR)
            fprintf(fp,MSG_ERROR_MEMORY);
    }
    if (fp != stdout) /*cierro archivo */
        fclose(fp);
    for(i=0;i<cantOp;i++) /* libero las listas */
    {
        freeList((*(operations[i])).num1);
        freeList((*(operations[i])).num2);
        freeList((*(operations[i])).ans);
        free(operations[i]); /* libero la estructura*/
    }
    free(operations); /* libero el vector de estructuras */
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
    if (aux2 == NULL) /* error de memoria*/
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
    if (buffer[i] == SUB)
    {
        (*(operations[cantOp-1])).sign1 = TRUE;
        i++;
    }
    /*PRIMER NUMERO*/
    for(j=0;buffer[i] !=ADD && buffer[i] !=SUB && buffer[i] !=MUX;j++) /* hasta que aparezca operacion */
    {
        addValue(&(*(operations[cantOp-1])).num1, buffer[i++]-ZERO_CHAR); /* agrego a la lista1 */
    }
    /*OPERACION*/
    switch(buffer[i])
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
    if (buffer[i] == SUB)
    {
        (*(operations[cantOp-1])).sign2 = TRUE;
        i++;
    }
    /*SEGUNDO NUMERO*/
    for(j=0;i<strlen(buffer)-1 ;j++) /* hasta el final menos /n*/
    {
        addValue(&(*(operations[cantOp-1])).num2, buffer[i++]-ZERO_CHAR); /* agrego a lista2*/
    }
    return EXIT_SUCCESS;
}
/*----------IMPRIMIR LISTA-------------*/
void printList(t_nodo* list,FILE* file)
{
    if (list == NULL)
        fprintf(file,"0"); /* imprimo valor */
    else
        do{
            fprintf(file,"%hi",list->val); /* imprimo valor */
            list=list->sig;   /* voy al siguiente */
        }while (list != NULL); /* hasta que llegue al final*/
}
/*----------IMPRIMIR LISTA AL REVES-------------*/
void printListBackwards(t_nodo* list,FILE* file)
{
    if (list == NULL)
        fprintf(file,"00"); /* imprimo valor */
    else{
        while (list->sig != NULL) /*recorre hasta el final*/
            list=list->sig; /*voy al siguiente */
        while (list != NULL) /* hasta el principio */
        {
            while (list->val == 0 && list->ant != NULL){ /*mientras el valor sea 0, no imprimo nada, cero a la izquiera */
                list = list->ant;   /* voy al anterior*/
                free(list->sig);    /* libero el siguiente */
                list->sig = NULL; /*siguiente a null*/
            }
            fprintf(file,"%hi",list->val); /* imprimo valor */
            list=list->ant; /*voy al anterior */
        }
    }
}
/*----------LIBERAR LISTA-------------*/
void freeList(t_nodo* list)
{
    if (list!=NULL) /* hasta el final */
    {
        while(list->sig != NULL) /*recorre hasta el final*/
        {
            list=list->sig;
        }
        while(list->ant != NULL) /*recorre hasta el principio y libera*/
        {
            list = list->ant; /* voy al anterior*/
            free(list->sig);    /* libero el siguiente*/
        }
        free(list); /* libero el ultimo*/
    }
}
/*----------AGREGAR VALOR A LISTA-------------*/
int addValue(t_nodo** listPointer,short value)
{
    t_nodo* list;
    list = *listPointer;
    if (list == NULL) /* Lista creada?*/
    {
        list = (t_nodo*)calloc(1,sizeof(t_nodo)); /* alojo memoria */
        if (list == NULL)
        {
            printf(MSG_ERROR_MEMORY);
            return EXIT_FAILURE;
        }
        list->ant = NULL; /*puede ser redundante, por seguridad lo dejamos*/
    }
    else
    {
        while (list->sig != NULL) /*recorre hasta el final*/
            list=list->sig;
        list->sig =(t_nodo*)calloc(1,sizeof(t_nodo));
        if (list->sig == NULL) /* si el siguiente es null, no se alojo memoria*/
        {
            printf(MSG_ERROR_MEMORY);
            return EXIT_FAILURE;
        }
        list->sig->ant = list; /* el anterior es donde estaba parado */
        list=list->sig; /* me muevo al siguiente */
    }
    list->sig = NULL; /*puede ser redundante, por seguridad lo dejamos*/
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
        {
            if ((*oper).sign2==TRUE) /*cambio el signo de la segunda operacion*/
                (*oper).sign2 = FALSE;
            else (*oper).sign2 = TRUE;
        }   /* voy a ADD */
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
result_state_t addNumbers(operation_t* oper,int precision)
{
    /*Variables*/
    int i,carry=0, resultado;
    t_nodo *lista1, *lista2, *ans=NULL;
    /*defino puntero a listas para no modificar el puntero original */
    lista1=(*oper).num1;
    lista2=(*oper).num2;
    while (lista1->sig != NULL) /*recorre hasta el final*/
        lista1=lista1->sig;
    while (lista2->sig != NULL) /*recorre hasta el final*/
        lista2=lista2->sig;
    while (lista1 != NULL || lista2 != NULL) /* Mientras alguno de los 2 sea distinto de NULL */
    {
        if (lista1 == NULL){ /* se termino el primer numero */
            resultado = lista2->val+carry;
            lista2=lista2->ant;
        }
        else if (lista2==NULL){ /* se termino el segundo numero */
            resultado = lista1->val+carry;
            lista1=lista1->ant;
        }
        else{
            resultado = lista1->val + lista2->val + carry;
            lista2=lista2->ant;
            lista1=lista1->ant;
        }
        if (resultado > 9) { /* carry */
            carry = resultado / 10;
            resultado = resultado % 10;
        }
        else
            carry = 0;
        if (addValue(&ans,resultado) == EXIT_FAILURE)
            return ERR;
    }
    if (carry != 0)
        addValue(&ans,carry);
    (*oper).ans = ans;
    i=0;
    while (ans != NULL) { /*recorre hasta el final*/
        i++;
        ans=ans->sig;
    }
    if (i>precision) /* condicion de overflow */
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
int superior(operation_t* oper)
{
    int i=1,j=1;
    t_nodo *listai, *listaj;
    listai=(*oper).num1;
    listaj=(*oper).num2;
    while (listai->sig != NULL) { /*recorre hasta el final*/
        listai=listai->sig;
        i++;
    }
    while (listaj->sig != NULL) { /*recorre hasta el final*/
        listaj=listaj->sig;
        j++;
    }
    if (i < j)
        return 2;
    if (i > j)
        return 1;
    listai=(*oper).num1;
    listaj=(*oper).num2;
    while (listai != NULL){
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
    /* defino como lista1 al mas grande y 2 al mas chico*/
    if (superior(oper) == 2)
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

    while (lista1 != NULL || lista2 != NULL) /*mientras alguno de los 2 sea distinto de null*/
    {
        if (lista1 == NULL){ /* si se acabo lista1 */
            resultado = lista2->val - lista2->val - lista2->val - borrow;
            lista2=lista2->ant;
        }
        else if (lista2==NULL){ /* si se acabo lista2 */
            resultado = lista1->val-borrow;
            lista1=lista1->ant;
        }
        else{
            resultado = lista1->val - lista2->val - borrow;
            lista2=lista2->ant;
            lista1=lista1->ant;
        }
        if (resultado < 0) { /* borrow */
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
    while (ans != NULL) { /*recorre hasta el final*/
        i++;
        ans=ans->sig;
    }
    if (i>precision) /*condicion de overflow*/
        return OFW;
    return OK;
}
/*-----RESTA-------*/
result_state_t substraction(operation_t* oper,int precision)
{

    (*oper).signAns = FALSE; /*asumo resultado positivo*/
    switch (superior(oper))
    {
        /*defino los signos*/
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
    if (lista1Length+lista2Length-1>precision) /* una condicion de overflow */
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
        else
            return OFW;
    }
    for(i=0;i<precision;i++){
        addValue(&ans,bufferAns[i]);
    }
    while (ans->sig != NULL) { /*recorre hasta el final*/
        ans=ans->sig;
    }
    while (ans->val == 0 && ans->ant !=NULL) {
        ans=ans->ant;
        free(ans->sig);
        ans->sig = NULL;
    }
    (*oper).ans = ans;
    free(bufferAns);
    return OK;
}

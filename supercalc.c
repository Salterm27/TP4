#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "supercalc.h"
#include "main.h"
/*----------SUPERCALC---------------*/
int superCalc(int precision,char* input,char* output)
{

    operation_t** operations;
    int cantOp=0,i;
    int bufferLength;
    FILE* fp=NULL;
    char* buffer;
    if (precision*2+5<strlen(CALCULATE_TOLKEN))
        bufferLength = strlen(CALCULATE_TOLKEN)+2; /* length + \0+\n*/
    else
        bufferLength = precision*2+5;
    buffer=(char*)calloc(bufferLength,sizeof(char));

    if (buffer==NULL)
    {
        printf(MSG_ERROR_MEMORY);
        return EXIT_FAILURE;
    }
    if (input != NULL)
    {
        fp = fopen(input,"r");
        while (strncmp(fgets(buffer,bufferLength,fp),CALCULATE_TOLKEN,strlen(CALCULATE_TOLKEN)) != 0) /*num1+num2+signos+op+\n*/
        {
            if (addOperation(&operations,&cantOp) == NULL)
            {
                printf(MSG_ERROR_MEMORY);
            }
            if (buffer == NULL)
                (*(operations[cantOp-1])).st = OFW;
            else
                parseOperation(buffer,operations,cantOp,precision);
        }
        fclose(fp);
    }
    else
    {
        while (strncmp(fgets(buffer,bufferLength,stdin),CALCULATE_TOLKEN,strlen(CALCULATE_TOLKEN)) != 0)
        {
            if (addOperation(&operations,&cantOp) == NULL)
            {
                printf(MSG_ERROR_MEMORY);
            }
            if (buffer == NULL)
                (*(operations[cantOp-1])).st = OFW;
            else
                parseOperation(buffer,operations,cantOp,precision);
        }
    }
    free(buffer);
    /*SOLVE OPERATIONS*/
    for(i=0;i<cantOp;i++)
    {
        solveOperation(operations[i],precision);
    }
    if(output != NULL)
    {
        fp = fopen(output,"w");
        if (fp == NULL)
        {
            printf(MSG_ERROR_ARG);
            return EXIT_FAILURE;
        }
    }
    else
        fp = stdout;
    for(i=0;i<cantOp;i++)
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
    fclose(fp);
    for(i=0;i<cantOp;i++)
    {
        freeList((*(operations[i])).num1);
        freeList((*(operations[i])).num2);
        freeList((*(operations[i])).ans);
        free(operations[i]);
    }
    free(operations);
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
    if (buffer[i] == '-')
    {
        (*(operations[cantOp-1])).sign1 = TRUE;
        i++;
    }
    /*PRIMER NUMERO*/
    for(j=0;buffer[i] !='+' && buffer[i] !='-' && buffer[i] !='*';j++)
    {
        addValue(&(*(operations[cantOp-1])).num1, buffer[i++]-'0');
    }
    /*OPERACION*/
    switch(buffer[i])
    {
        case '+':
        {
            (*(operations[cantOp-1])).op = ADD;
            break;
        }
        case '-':
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
    if (buffer[i] == '-')
    {
        (*(operations[cantOp-1])).sign2 = TRUE;
        i++;
    }
    /*SEGUNDO NUMERO*/
    for(j=0;i<strlen(buffer)-1 ;j++)
    {
        addValue(&(*(operations[cantOp-1])).num2, buffer[i++]-'0');
    }
    return EXIT_SUCCESS;
}

void printList(t_nodo* list,FILE* file)
{
    do{
        fprintf(file,"%hi",list->val);
        list=list->sig;
    }while (list != NULL);
}
void printListBackwards(t_nodo* list,FILE* file)
{
    while (list->sig != NULL) /*recorre hasta el final*/
        list=list->sig;
    while (list->val == 0 && list->ant != NULL)
        list = list->ant;
    while (list != NULL)
    {
        fprintf(file,"%hi",list->val);
        list=list->ant;
    }
}

void freeList(t_nodo* list)
{
    if (list!=NULL)
    {
        while(list->sig != NULL) /*recorre hasta el final*/
        {
            list=list->sig;
        }
        while(list->ant != NULL) /*recorre hasta el principio y libera*/
        {
            list = list->ant;
            free(list->sig);
        }
        free(list);
    }
}

int addValue(t_nodo** listPointer,short value)
{
    t_nodo* list;
    list = *listPointer;
    if (list == NULL) /* Lista creada?*/
    {
        list = (t_nodo*)calloc(1,sizeof(t_nodo));
        if (list == NULL)
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
        list->sig =(t_nodo*)calloc(1,sizeof(t_nodo));
        if (list->sig == NULL)
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
        {/*cambio el signo de la segunda operacion*/
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
result_state_t addNumbers(operation_t* oper,int precision)
{
    int i,carry=0, resultado;
    t_nodo *lista1, *lista2, *ans=NULL;
    lista1=(*oper).num1;
    lista2=(*oper).num2;
    while (lista1->sig != NULL) /*recorre hasta el final*/
        lista1=lista1->sig;
    while (lista2->sig != NULL) /*recorre hasta el final*/
        lista2=lista2->sig;
    while (lista1 != NULL || lista2 != NULL)
    {
        if (lista1 == NULL){
            resultado = lista2->val+carry;
            lista2=lista2->ant;
        }
        else if (lista2==NULL){
            resultado = lista1->val+carry;
            lista1=lista1->ant;
        }
        else{
            resultado = lista1->val + lista2->val + carry;
            lista2=lista2->ant;
            lista1=lista1->ant;
        }
        if (resultado > 9) {
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
    if (i>precision)
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

    while (lista1 != NULL || lista2 != NULL)
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
    while (ans != NULL) { /*recorre hasta el final*/
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
    bufferAns = (short*)calloc(precision+1,sizeof(short));
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
    for(i=0;i<precision+1;i++){
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

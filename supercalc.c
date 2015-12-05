#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "supercalc.h"
#include "main.h"
/*----------SUPERCALC---------------*/
void superCalc(int precision,char* input,char* output)
{
    operation_t** operations;
    int cantOp=0,i,j;
    FILE* fp=NULL;
    char* buffer=(char*)calloc(precision*2+5,sizeof(char));
    if (buffer==NULL)
    {
        printf(MSG_ERROR_MEMORY);
    }
    if (input != NULL)
    {
        fp = fopen(input,"r");
        while (strcmp(fgets(buffer,precision*2+5,fp),CALCULATE_TOLKEN) != 0) /*num1+num2+signos+op+\n*/
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
        while (strcmp(fgets(buffer,precision*2+5,stdin),CALCULATE_TOLKEN) != 0)
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
    for (i=0; i<cantOp;i++) {
    	printList((*(operations[i])).num1);
    }
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
        aux = (operation_t**)malloc(sizeof(operation_t*)); /* Creo memoria*/
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
    aux2 = (operation_t*)malloc(sizeof(operation_t));  /*agrego una operacion y la guardo en la memoria expandida*/
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

void printList(t_nodo* list)
{
    do{
        printf("%hi",list->val);
        list=list->sig;
    }while (list != NULL);
}
void printListBackwards(t_nodo* list)
{
    while (list->sig != NULL) /*recorre hasta el final*/
        list=list->sig;
    while (list != NULL)
    {
        printf("%hi",list->val);
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
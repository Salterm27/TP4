#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "supercalc.h"
#include "main.h"

/*----------SUPERCALC---------------*/
void superCalc(int precision)
{
    /*VARIABLES*/
    char* buffer=NULL;
    operation_t** operations=NULL;
    int cantOp=0,i,j;
    /*BUFFER*/
    buffer = (char*)malloc(sizeof(char)*((2*precision)+2)); /* Op1 + Op2 + Operacion + \0  buffer*/
    if (buffer==NULL)
    {
        printf(MSG_ERROR_MEMORY);
    }
    /*WHILE NOT TOLKEN*/
    while (strcmp(CALCULATE_TOLKEN,gets(buffer)) != 0 && buffer != NULL) /*Mientras no escriba #Calculate*/
    {
        if (addOperation(&operations,&cantOp) == NULL) /*Agrego operacion y me fijo si hubo error de memoria*/
            printf(MSG_ERROR_MEMORY);
        else
            if (buffer == NULL)     /*Si buffer es NULL*/
                (*(operations[cantOp-1])).st = OFW; /* Hubo OverFlow ya que string > buffer*/
            else
                parseOperation(buffer,operations,cantOp,precision); /* interpreto el buffer */
    }
    free(buffer); /* Libero el buffer, no lo uso mas*/
    /*SOLVE OPERATIONS*/
    for(i=0;i<cantOp;i++)
    {
        solveOperation(operations[i],precision);
    }
    /*PRINT OPERATIONS ANSWERS*/
    for(i=0;i<cantOp;i++)
    {
        if((*operations[i]).st == OFW)
            printf(MSG_INFINITY);
        else
        {
            if((*operations[i]).st == ERR)
                printf(MSG_ERROR_MEMORY);
            else
            {
                if((*operations[i]).signAns == TRUE)
                {
                    printf("-");
                }
                for(j=(*operations[i]).ans.numSize-1;j>=0;j--)
                {
                    printf("%d",(*operations[i]).ans.num[j]);
                }
                printf("\n");
            }
        }
    }
    /*FREE MEMORY*/
    for(i=0;i<cantOp;i++)
    {
        free((*operations[i]).num1.num);
        free((*operations[i]).num2.num);
        free((*operations[i]).ans.num);
        free(operations[i]);
    }
    free(operations);
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
    short* bufferNum;
    /*INICIALIZO OPERACION*/
    (*(operations[cantOp-1])).num1.num = NULL;
    (*(operations[cantOp-1])).num1.numSize = 0;
    (*(operations[cantOp-1])).num2.num = NULL;
    (*(operations[cantOp-1])).num2.numSize = 0;
    (*(operations[cantOp-1])).ans.num = NULL;
    (*(operations[cantOp-1])).ans.numSize = 0;
    (*(operations[cantOp-1])).sign1 = FALSE;
    (*(operations[cantOp-1])).sign2 = FALSE;
    (*(operations[cantOp-1])).signAns = FALSE;
    /*MEMORIA PARA NUM1*/
    (*(operations[cantOp-1])).num1.num=(short*)calloc(precision,sizeof(short));
    if ((*(operations[cantOp-1])).num1.num == NULL)
    {
        printf(MSG_ERROR_MEMORY);
        return EXIT_FAILURE;
    }
    /*MEMORIA PARA NUM2*/
    (*(operations[cantOp-1])).num2.num=(short*)calloc((precision),sizeof(short));
    if ((*(operations[cantOp-1])).num2.num == NULL)
    {
        printf(MSG_ERROR_MEMORY);
        return EXIT_FAILURE;
    }
    /*MEMORIA PARA BUFFERNUM*/
    bufferNum=(short*)calloc(precision,sizeof(short));
    if ((*(operations[cantOp-1])).num1.num == NULL)
    {
        printf(MSG_ERROR_MEMORY);
        return EXIT_FAILURE;
    }
    /*SIGNO PRIMER NUMERO*/
    if (buffer[i] == '-')
    {
        (*(operations[cantOp-1])).sign1 = TRUE;
        i++;
    }
    /*PRIMER NUMERO*/
    for(j=0;buffer[i] !='+' && buffer[i] !='-' && buffer[i] !='*';j++)
    {
        bufferNum[j] = buffer[i++]-'0';
        (*(operations[cantOp-1])).num1.numSize++;
    }
    for (j=0;j<(*(operations[cantOp-1])).num1.numSize;j++)
    {
        (*(operations[cantOp-1])).num1.num[j] = bufferNum[(*(operations[cantOp-1])).num1.numSize-1-j];
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
    for(j=0;i<strlen(buffer) ;j++)
    {
        bufferNum[j] = buffer[i]-'0';
        ++(*(operations[cantOp-1])).num2.numSize;
        i++;
    }
    for (j=0;j<(*(operations[cantOp-1])).num2.numSize;j++)
    {
        (*(operations[cantOp-1])).num2.num[j] = bufferNum[(*(operations[cantOp-1])).num2.numSize-1-j];
    }
    free(bufferNum); /* Libero bufferNum, no lo uso mas*/
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
            if (((*oper).sign1 == TRUE && (*oper).sign2 == FALSE) || ((*oper).sign1 == FALSE && (*oper).sign2 == TRUE))
                (*oper).signAns = TRUE;
            (*oper).ans.numSize = (*oper).num1.numSize +(*oper).num2.numSize;
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
/*-----MULTIPLICACION----*/
result_state_t multiply(operation_t* oper,int precision)
{
    /*VARIABLES*/
    int i=0,j=0;
    short suma=0;
    short *bufferAns;
    /*PRECISION*/
    (*oper).ans.numSize=precision;
    if ((*oper).num1.numSize + (*oper).num2.numSize  > precision+1)
    {
        return OFW;
    }
    bufferAns = (short*)calloc(precision,sizeof(short));
    if (bufferAns == NULL) /*si hay error de alojamiento de memoria*/
    {
        printf(MSG_ERROR_MEMORY);
        return ERR;
    }
    for (i=0;i<(*oper).num2.numSize;i++) /*RECORRO VECTOR 2*/
    {
        suma = 0;
        for (j=0;j<(*oper).num1.numSize;j++)/*RECORRO VECTOR 1*/
        {
            suma = (*oper).num1.num[j]*(*oper).num2.num[i] + bufferAns[i+j] + suma/10;
            bufferAns[i+j] = suma%10;
        }
        if (i<(*oper).num2.numSize-1)
            bufferAns [j+i] = suma/10;
    }
    if (suma/10)
    {
        if ((*oper).num1.numSize +(*oper).num2.numSize-1 < precision)
            bufferAns[(*oper).num1.numSize + (*oper).num2.numSize -1] = suma/10;
        else
            return OFW;
    }
    (*oper).ans.num = bufferAns;
    for (i=0;i<(*oper).ans.numSize && (*oper).ans.num[precision-1-i] == 0;i++) /* redefino tamaño */
        (*oper).ans.numSize--;

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
}

/*-----RESTA-------*/
result_state_t substraction(operation_t* oper,int precision)
{
    (*oper).ans.num = (short*)calloc(precision,sizeof(short));
    if ((*oper).ans.num == NULL)
    {
        printf(MSG_ERROR_MEMORY);
        return ERR;
    }
    (*oper).signAns = FALSE; /*asumo resultado positivo*/
    switch (superior((*oper).num1,(*oper).num2))
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
            (*oper).ans.numSize = 1;
            (*oper).ans.num[0]=0;
            return OK;
        }
    }
}

/*-----IDENTIFICAR NUMERO MAS GRANDE-----------*/
int superior(bignum_t num1,bignum_t num2)
{
    int i;
    if (num1.numSize < num2.numSize)
        return 2;
    if (num1.numSize > num2.numSize)
        return 1;
    for (i=0;i<num1.numSize;i++)
    {
    if (num1.num[num1.numSize-1-i] < num2.num[num1.numSize-1-i])
        return 2;
    if (num1.num[num1.numSize-1-i] > num2.num[num1.numSize-1-i])
        return 1;
    }

    return 0;
}

/*-----RESTAR NUMEROS---------*/
result_state_t subNumbers(operation_t* oper,int precision)
{
    int i,borrow=0,size;
    (*oper).ans.numSize = precision;
    if (superior((*oper).num1,(*oper).num2) == 2)
    {
        for(i=0;i<(*oper).num2.numSize;i++)
        {
            (*oper).ans.num[i] = (*oper).num2.num[i]-(*oper).num1.num[i]-borrow;
            if ((*oper).ans.num[i] < 0)
            {
                (*oper).ans.num[i] += 10;
                borrow = 1;
            }
            else
                borrow = 0;
        }
    }
    else
    {
        for(i=0;i<(*oper).num1.numSize;i++)
        {
            (*oper).ans.num[i] = (*oper).num1.num[i]-(*oper).num2.num[i]-borrow;
            if ((*oper).ans.num[i] < 0)
            {
                (*oper).ans.num[i] += 10;
                borrow = 1;
            }
            else
                borrow = 0;
        }
    }

    for (i=0,size=(*oper).ans.numSize;i<(*oper).ans.numSize && (*oper).ans.num[precision-1-i] == 0;i++)
    {
        size--;
    }
    (*oper).ans.numSize = size;
    return OK;
}

/*---------SUMAR NUMEROS-------*/
result_state_t addNumbers(operation_t* oper,int precision)
{
    int i;
    bool_t endFor = FALSE;
    (*oper).ans.num = (short*)calloc(precision+1,sizeof(short));
    if ((*oper).ans.num == NULL)
    {
        printf(MSG_ERROR_MEMORY);
        return ERR;
    }
    for (i=0;i<precision;i++)
    {
        (*oper).ans.num[i] += (*oper).num1.num[i]+(*oper).num2.num[i];
        if ((*oper).ans.num[i] > 9)
        {
            (*oper).ans.num[i+1] = (*oper).ans.num[i]/10;
            (*oper).ans.num[i] = (*oper).ans.num[i]%10;
        }
    }

    if ((*oper).ans.num[precision] != 0)
    {
        return OFW;
    }
    (*oper).ans.numSize = precision;
    for (i=precision-1,endFor=FALSE;i>=0 && endFor == FALSE;i--)
    if ((*oper).ans.num[i] == 0)
    {
            (*oper).ans.numSize--;
    }
    else
    {
        endFor=TRUE;
    }
    return OK;
}

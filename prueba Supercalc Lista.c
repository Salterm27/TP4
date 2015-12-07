#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_ERROR_MEMORY "Error de memoria\n"
#define CALCULATE_TOLKEN "#calculate"
#define MSG_ERROR_OPERATION "Solo se puede multiplicar, restar o sumar"
#define MSG_INFINITY "Infinito\n"
typedef struct
{
    int numSize;
    short *num;
}bignum_t;

typedef enum{TRUE=1,FALSE=0}bool_t;

typedef enum{ADD='+',SUB='-',MUX='*'}op_t;

typedef enum{OK,NAN,OFW,ZERO,ERR}result_state_t;

typedef struct operation
{
    bool_t sign1,sign2,signAns;
    bignum_t num1,num2,ans;
    op_t op;
    result_state_t st;
}operation_t;

typedef struct nodo{
    struct nodo *sig;
    struct nodo *ant;
    short val;
}t_nodo;

void freeList(t_nodo* list);
int addValue(t_nodo** listPointer,short value);
void printList(t_nodo* list);
void printListBackwards(t_nodo* list);
operation_t** addOperation(operation_t*** operations,int* cantOp);
void supercalc(int precision,char* input,char* output);
int parseOperation(char* buffer,operation_t** operations,int cantOp,int precision);
void solveOperation(operation_t*,int);
result_state_t multiply(operation_t*,int);
result_state_t addition(operation_t*,int);
result_state_t substraction(operation_t*,int);
result_state_t addNumbers(operation_t* oper,int precision);
result_state_t subNumbers(operation_t* oper,int precision);
int superior(bignum_t,bignum_t);
bignum_t multiply2(bignum_t a,bignum_t b,int precision);

int main(void)
{
    char *input=NULL,*output=NULL;
    t_nodo *n1=NULL;
    int i,qDigits=0;
    short digit;
 /*   scanf("%d",&qDigits);
    for (i=0;i<qDigits;i++)
    {
        scanf("%hi",&digit);
        if (addValue(&n1,digit) == 1)
        {
            printf(MSG_ERROR_MEMORY);
            return EXIT_FAILURE;
        }
    }
    printList(n1);
    printf("\n");
    printListBackwards(n1);
    freeList(n1);
    return EXIT_SUCCESS;
    */
    supercalc(100,input,output);
    return EXIT_SUCCESS;
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
        list->ant = NULL;
    }
    else
    {
        while (list->sig != NULL)
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
    list->sig = NULL;
    list->val = value;
    while (list->ant != NULL)
        list=list->ant;
    *listPointer = list;
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
    while (list->sig != NULL)
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
        while(list->sig != NULL)
        {
            list=list->sig;
        }
        while(list->ant != NULL)
        {
            list = list->ant;
            free(list->sig);
        }
        free(list);
    }
}

void supercalc(int precision,char* input,char* output)
{
    operation_t** operations;
    int cantOp=0,i,j;
    FILE* fp=NULL;
    char* buffer=(char*)calloc(precision*2+2,sizeof(char));
    if (buffer==NULL)
    {
        printf(MSG_ERROR_MEMORY);
    }
    if (input != NULL)
    {
        fp = fopen(input,"r");
        while (strcmp(fgets(buffer,precision,fp),CALCULATE_TOLKEN) != 0)
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
        while (strcmp(fgets(buffer,precision,stdin),CALCULATE_TOLKEN) != 0)
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
}
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

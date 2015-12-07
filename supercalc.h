#ifndef __SUPERCALC_H__
#define __SUPERCALC_H__

#define CALCULATE_TOLKEN "#calculate"
#define MSG_ERROR_OPERATION "Solo se puede multiplicar, restar o sumar"
#define MSG_INFINITY "Infinito\n"
#define MIN_BUFFER_LENGTH 10 /*MIN LENGTH  "#calculate" */

#ifndef __BOOL_T__
#define __BOOL_T__
typedef enum{TRUE=1,FALSE=0}bool_t;
#endif

#ifndef __BIGNUM_T__
#define __BIGNUM_T__
typedef struct
{
    int numSize;
    short *num;
}bignum_t;
#endif
#ifndef __NODO_T__
#define __NODO_T__
typedef struct nodo {
	struct nodo *sig, *ant;
	short val;
}t_nodo;
#endif
#ifndef __OPERATION_T__
#define __OPERATION_T__
typedef enum{ADD='+',SUB='-',MUX='*'}op_t;

typedef enum{OK,NAN,OFW,ZERO,ERR}result_state_t;

typedef struct operation
{
    bool_t sign1,sign2,signAns;
    t_nodo *num1, *num2, *ans;
    op_t op;
    result_state_t st;
}operation_t;

#endif

int superCalc(int, char*, char*);
int parseOperation(char*,operation_t**,int,int);
operation_t** addOperation(operation_t***,int*);
void solveOperation(operation_t*,int);
result_state_t multiply(operation_t*,int);
result_state_t addition(operation_t*,int);
result_state_t substraction(operation_t*,int);
result_state_t addNumbers(operation_t* ,int );
result_state_t subNumbers(operation_t* ,int );
int superior(operation_t *);
bignum_t multiply2(bignum_t a,bignum_t b,int);
int addValue(t_nodo** listPointer,short );
void printList(t_nodo*,FILE*);
void printListBackwards(t_nodo*,FILE*);
void freeList(t_nodo* );
#endif

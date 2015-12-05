#ifndef __SUPERCALC_H__
#define __SUPERCALC_H__

#define CALCULATE_TOLKEN "#calculate"
#define MSG_ERROR_OPERATION "Solo se puede multiplicar, restar o sumar"
#define MSG_INFINITY "Infinito\n"

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

#ifndef __OPERATION_T__
#define __OPERATION_T__
typedef enum{ADD='+',SUB='-',MUX='*'}op_t;

typedef enum{OK,NAN,OFW,ZERO,ERR}result_state_t;

typedef struct operation
{
    bool_t sign1,sign2,signAns;
    bignum_t num1,num2,ans;
    op_t op;
    result_state_t st;
}operation_t;

#endif

void superCalc(int);
int parseOperation(char*,operation_t**,int,int);
operation_t** addOperation(operation_t***,int*);
void solveOperation(operation_t*,int);
result_state_t multiply(operation_t*,int);
result_state_t addition(operation_t*,int);
result_state_t substraction(operation_t*,int);
result_state_t addNumbers(operation_t* oper,int precision);
result_state_t subNumbers(operation_t* oper,int precision);
int superior(bignum_t,bignum_t);
bignum_t multiply2(bignum_t a,bignum_t b,int precision);
#endif

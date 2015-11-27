#ifndef __SIMPLECALC_H__
#define __SIMPLECALC_H__

#define CALC_INTRO1 "\
|===============================================|\n\
|                  Calculadora                  |\n\
|===============================================|\n\
|   1) suma     |   2) resta    | 3) division   |\n\
|     R1+R2     |     R1-R2     |     R1/R2     |\n\
|===============|===============|===============|\n\
|  4) producto  |  5) potencia  | 6) factorial  |\n\
|     R1*R2     |      R^Z      |       N!      |\n"
#define CALC_INTRO2 "\
|===============|===============|===============|\n\
|7) combinatorio|   8) Random   | 9) Graficador |\n\
|     (N K)     |     [R1,R2]   |               |\n\
|===============================================|\n\
|  Nota: R=numero real, Z=Numero entero         |\n\
|                N=Numero natural               |\n\
|===============================================|\n\
|                                               |\n\
|===============================================|\n\n"

#ifndef __BOOL_T__
#define __BOOL_T__
typedef enum{TRUE=1,FALSE=0}bool_t;
#endif

#ifndef __OPTION_T__
#define __OPTION_T__
typedef enum{OPT_ADD=1,OPT_SUB=2,OPT_DIV=3,OPT_PROD=4,OPT_EXP=5,OPT_FACT=6,OPT_BIN=7,OPT_RAND=8,OPT_GRAPH=9}option_t;
#endif

void printMenu();
int askOption();
long unsigned longMenu(option_t,bool_t*);
float floatMenu(option_t,bool_t*);
void printAnsLong(long unsigned);
void printAnsFloat(float);

#endif

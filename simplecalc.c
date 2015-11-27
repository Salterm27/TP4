#include <stdio.h>
#include <stdlib.h>
#include "simplecalc.h"
#include "main.h"
#include "numeric.h"

/*------------------------------------------*/
void printMenu(void)
{
/* no entraba en 1 solo DEFINE, formato ansi tira un error por excedente de caracteres en un define*/
    printf(CALC_INTRO1);
    printf(CALC_INTRO2);
    printf("\n\n");
}
/*------------------------------------------*/
int askOption(void)
{
    int opt;
    printf(MSG_ASK_OPTION);
    scanf("%d",&opt);
    printf ("\n\n");
    return opt;
}
/*------------------------------------------*/
void printAnsLong(long unsigned ans)
{
    printf(MSG_ANS);
    printf("%u\n", ans);      /* RESPUESTA = LONG*/
}
/*------------------------------------------*/
void printAnsFloat(float ans)
{
    printf(MSG_ANS);
    printf("%.4g\n", ans);      /* RESPUESTA = FLOAT*/
}
/*------------------------------------------*/
long unsigned longMenu(option_t opt, bool_t* errorFlagPointer)
{
    int inputN,inputK;
    if (opt == OPT_FACT)
    {
        printf(MENU_SEPARATOR);
        printf (MENU_OP6);
        printf(MENU_SEPARATOR);
        printf(MSG_INPUT_N);
        scanf("%d" , &inputN);    /*Pido N*/
        return factorial(inputN, errorFlagPointer);
    }
	else
    {
        printf(MENU_SEPARATOR);
        printf (MENU_OP7);
        printf(MENU_SEPARATOR);
        printf(MSG_INPUT_N);    /*Pido N*/
        scanf("%d", &inputN);
        printf(MSG_INPUT_K);    /*Pido K*/
        scanf("%d", &inputK);
        return binomial(inputN, inputK, errorFlagPointer);
    }

}
/*------------------------------------------*/
float floatMenu(option_t opt, bool_t* errorFlagPointer)
{
    float inputR1, inputR2;
    float ansFloat = 0;
    int inputZ;
    switch(opt)          /*Voy a la opcion que eligio el usuario */
    {
        case OPT_ADD:
            printf(MENU_SEPARATOR);
            printf (MENU_OP1);
            printf(MENU_SEPARATOR);
            printf(MSG_INPUT_R1);
            scanf ("%f", &inputR1); /*Pido R1*/
            printf(MSG_INPUT_R2);
            scanf("%f", &inputR2);  /*Pido R2*/
            ansFloat = add(inputR1, inputR2);
            break;
        case OPT_SUB:
            printf(MENU_SEPARATOR);
            printf (MENU_OP2);
            printf(MENU_SEPARATOR);
            printf(MSG_INPUT_R1);
            scanf ("%f" , &inputR1); /*pido R1*/
            printf(MSG_INPUT_R2);
            scanf("%f", &inputR2);  /*Pido R2*/
            ansFloat = subs(inputR1, inputR2);
            break;
        case OPT_DIV:
            printf(MENU_SEPARATOR);
            printf (MENU_OP3);
            printf(MENU_SEPARATOR);
            printf(MSG_INPUT_R1);
            scanf ("%f" , &inputR1); /*Pido R1*/
            printf(MSG_INPUT_R2);
            scanf("%f" , &inputR2);  /*Pido R2*/
            ansFloat = division(inputR1, inputR2, errorFlagPointer);
            break;
        case OPT_PROD:
            printf(MENU_SEPARATOR);
            printf (MENU_OP4);
            printf(MENU_SEPARATOR);
            printf(MSG_INPUT_R1);
            scanf ("%f", &inputR1); /*Pido R1*/
            printf(MSG_INPUT_R2);
            scanf("%f", &inputR2);  /*Pido R2*/
            ansFloat = product(inputR1, inputR2);
            break;
        case OPT_EXP:
            printf(MENU_SEPARATOR);
            printf (MENU_OP5);
            printf(MENU_SEPARATOR);
            printf(MSG_INPUT_R);
            scanf ("%f", &inputR1); /*Pido R*/
            printf(MSG_INPUT_Z);
            scanf("%d", &inputZ);   /*Pido Z*/
            ansFloat = exponent(inputR1, inputZ, errorFlagPointer);
            break;
        case OPT_RAND:
            printf(MENU_SEPARATOR);
            printf (MENU_OP8);
            printf(MENU_SEPARATOR);
            printf(MSG_INPUT_R1);
            scanf ("%f", &inputR1); /*Pido Limite Inferior*/
            printf(MSG_INPUT_R2);
            scanf("%f", &inputR2);  /*Pido Limite Superior*/
            ansFloat = random(inputR1, inputR2, errorFlagPointer);
            break;
        default:   /* Caso que no sea ninguna opcion */
            printf(MENU_ERROR);
            *errorFlagPointer = TRUE;
    }
    return ansFloat;
}

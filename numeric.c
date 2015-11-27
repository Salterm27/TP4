#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "numeric.h"
#include "main.h"

/*--------------------------------------------------Funciones----------------------------------------------*/
float add(float numA, float numB)
{
    return numA + numB;
}
/*------------------------------------------*/
float subs(float numA, float numB)
{
    return numA-numB;
}
/*------------------------------------------*/
float product(float numA, float numB)
{
    return numA * numB;
}
/*------------------------------------------*/
float division(float numA, float numB, bool_t* errorFlagPointer)
{
    if(numB == 0)                     /*Error si el dividendo es 0*/
    {
        printf(MSG_ERROR_DIV);
        *errorFlagPointer = TRUE;
        return 0;
    }
    return numA / numB;
}
/*------------------------------------------*/
float exponent(float numA, int numN,bool_t* errorFlagPointer)
{
    float total = 1;
    int i;
    if(numN == 0)                     /* Si exponente es 0, devolver 1*/
    {
        return 1;
    }
    if(numA == 0 && numN < 0)       /* Error 0^-N es como dividir por 0*/
    {
        printf(MSG_ERROR_POT);
        *errorFlagPointer = TRUE;
        return 0;
    }
    if(numN < 0)                          /* si exponente negativo, dar vuelta R y cambiarle el signo a N*/
    {
        numN =- numN;
        numA = division(1, numA, errorFlagPointer);    /* en esta subfuncion, errorFlag ya es una direccion, no necesita *.*/
    }
    for(i = 1 ; i <= numN ; i++)                /*total=total*R tantas veces como lo dice el exponente*/
    {
        total = product(total, numA);
    }
    return total;
}
/*------------------------------------------*/
long unsigned factorial(long unsigned numN,bool_t* errorFlagPointer)
{

    long unsigned total = 1;
    long unsigned i;
    if (numN < 0)       /* si el numero es negativo, activar el flag y salir*/
    {
        *errorFlagPointer = TRUE;
        printf(MSG_ERROR_FACT1);
        return 0;
    }
    else                    /* si el numero es positivo, hacer el factorial*/
    {
        for(i = numN; i > 0; i--)
        {
            total *= i;
        }
        return total;
    }
}
/*------------------------------------------*/
long unsigned binomial(int numberN, int numberK, bool_t* errorFlagPointer)
{
    long unsigned fn, fnmenosk, fk;
    float total;
    if (numberK < 0 || numberN < 0)     /* Primer error binomial K<0 o N<0*/
    {
        printf(MSG_ERROR_COMB1);
        *errorFlagPointer = TRUE;
        return 0;
    }
    if (numberK > numberN)                    /*Segundo error K>N*/
    {
        if (numberN == 0)                     /*propiedad combinatorio(0 K)=0 con K>0*/
        {
            return 0;
        }
        printf(MSG_ERROR_COMB2);
        *errorFlagPointer = TRUE;
        return 0;
    }
    if (numberK == numberN || numberK == 0)  /* propiedad combinatorio (N N)=(N 0)=1 */
    {
        return 1;
    }
    fn = factorial(numberN, errorFlagPointer);
    fnmenosk = factorial((numberN-numberK), errorFlagPointer);
    fk = factorial(numberK, errorFlagPointer);
    total = (fn / (fnmenosk * fk) );
    /* total= N!/((N_K)!K!)*/
    return total;
}
/*------------------------------------------*/
float random(float limitInf, float limitSup, bool_t* errorFlagPointer)
{
    float randomNumber;
    int aux1 = 0;
    float mod;
    if (limitInf > limitSup)              /* Error Limite inferior > Limite superior */
    {
        printf(MSG_ERROR_RAND);
        *errorFlagPointer = TRUE;
        return 0;
    }
    else
    {
        srand(time(NULL));
        mod = subs(limitSup, limitInf);
        while (aux1 == 0)
        {
            aux1 = rand()%RND_MAX;
        }
        randomNumber = add(limitInf, product(mod,division(aux1 , RND_MAX, errorFlagPointer)));
    }
    return randomNumber;
}

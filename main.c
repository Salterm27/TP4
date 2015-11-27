#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "main.h"
#include "simplecalc.h"
#include "graphic.h"
#include "supercalc.h"


int main(int argc,char* argv[])
{
    /*--definicion de Variables--*/
    float ansFloat = 0;
    long unsigned ansLong = 0;
    t_graph_opt optGraph;
    t_func funcion;
    option_t option;
    bool_t errorFlag = FALSE;
    t_arg arg;
    if (argc == 0)
    {
        printf(MSG_ERROR_ARG);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1],"superCalc") == 0)
    {
        arg.presition = 100;
        if (atoi(argv[3]) != 0)
            arg.presition = atoi(argv[3]);
        if (arg.presition < 1)
        {
            printf(MSG_ERROR_ARG_PRESITION);
            errorFlag = TRUE;
        }
        superCalc(arg.presition);
    }
    else if ((strcmp(argv[1],"simpleCalc") == 0))
    {
        printMenu();
        option = askOption();      /* Le pido la opcion al usuario */
        if(option == OPT_GRAPH)                    /* si la opcion es graficador */
        {
            optGraph = askFunction(&errorFlag);               /* pedir los parametros*/
            funcion = SolveFunction(optGraph,&errorFlag);      /* resolver la funcion pedida */
            functionToGraph(funcion,&errorFlag);               /* graficarla */
        }
        else if(option == OPT_FACT || option == OPT_BIN)      /* Si la opcion entrega un long como resultado ir al menu long */
        {
            ansLong = longMenu(option, &errorFlag);
            if (errorFlag == TRUE)              /* si hay error en las operaciones: Salir */
            {
                return EXIT_FAILURE;
            }
            else                                /*si no hay error, imprimir el resultado y salir */
            {
                printAnsLong(ansLong);
                return EXIT_SUCCESS;
            }
        }
        else if(option>9 ||option<1)
        {
            errorFlag = TRUE;
            printf(MSG_ERROR_OPTION);
            return EXIT_FAILURE;
        }
        else
        {
            ansFloat = floatMenu(option, &errorFlag);
            if (errorFlag == TRUE)      /* si hay error en las operaciones: Salir */
            {
                return EXIT_FAILURE;
            }
            else
            {
                printAnsFloat(ansFloat);        /*si no hay error, imprimir el resultado y salir */
                return EXIT_SUCCESS;
            }
        }
    }
    else
    {
        printf(MSG_ERROR_INPUT);
    }
return EXIT_SUCCESS;
}

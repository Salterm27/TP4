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
    int count;
    if (argc == 0)
    {
        printf(MSG_ERROR_ARG);
        return EXIT_FAILURE;
    }
    for (count = 1; count<argc;count++) {
        /*Para agregar los nombres o rutas de archivos necesito modificar primero las funciones, para saber como transmitir*/
        /*la informacion*/
        if (strcmp(argv[count],"-p") == 0){
            arg.presition = 100;
            if (atoi(argv[count+1]) <= 0)
            {
                printf(MSG_ERROR_ARG_PRESITION);
                errorFlag = TRUE;
            }
            else arg.presition = atoi(argv[count+1]);
        }
        if (strcmp(argv[count],"superCalc") == 0){
            superCalc(arg.presition);
            break; /*una vez que encuentra este argumento quiero salir del loop, ya que no tiene sentido seguir recorriendo*/
        }
        if ((strcmp(argv[count],"simpleCalc") == 0))
        {
            printMenu();
            option = askOption();      /* Le pido la opcion al usuario */
            if(option == OPT_GRAPH)                    /* si la opcion es graficador */
            {
                optGraph = askFunction(&errorFlag);               /* pedir los parametros*/
                funcion = SolveFunction(optGraph,&errorFlag);      /* resolver la funcion pedida */
                functionToGraph(funcion,&errorFlag);               /* graficarla */
            }
            if(option == OPT_FACT || option == OPT_BIN)      /* Si la opcion entrega un long como resultado ir al menu long */
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
            break; /*una vez que encuentra este argumento quiero salir del loop, ya que no tiene sentido seguir recorriendo*/
        }
        else if (count == argc)
        {
            printf(MSG_ERROR_INPUT);
        }
    }
    return EXIT_SUCCESS;
}
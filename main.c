#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "supercalc.h"
#include "main.h"
#include "simplecalc.h"
#include "graphic.h"
int main(int argc,char* argv[]) {
    /*--definicion de Variables--*/
    float ansFloat = 0;
    long unsigned ansLong = 0;
    t_graph_opt optGraph;
    t_func funcion;
    option_t option;
    bool_t errorFlag = FALSE;
    t_arg arg;
    int count;
    arg.input=NULL;
    arg.output=NULL;
    if (argc == 1) {    /* pocos parametros de entrada*/
        printf(MSG_ERROR_ARG);
        return EXIT_FAILURE;
    }
    arg.presition = 100; /*default precision*/
    arg.calcType = NO_ASIGN;/*no se asigno ninguna calculadora valida*/
    for (count = 1; count<argc;count++) {
        /*Para agregar los nombres o rutas de archivos necesito modificar primero las funciones, para saber como transmitir*/
        /*la informacion*/
        if (strcmp(argv[count],"-p") == 0){

            if (atoi(argv[count+1]) <= 0) {
                printf(MSG_ERROR_ARG_PRESITION);
                errorFlag = TRUE;
            }
            else arg.presition = atoi(argv[count+1]);
        }
        if (strcmp(argv[count],"superCalc") == 0){
            arg.calcType = SUPER_SEL;
        }
        if ((strcmp(argv[count],"simpleCalc") == 0)) {
            arg.calcType = SIMPLE_SEL;
        }
        else if (count == argc) {
            printf(MSG_ERROR_INPUT);
        }
        if (strcmp(argv[count],"-o") == 0){
            arg.output = (char*)malloc (sizeof(char)*(strlen(argv[count+1])+1));
            if (arg.output==NULL){
                printf(MSG_ERROR_MEMORY);
            }
            else strcpy (arg.output, argv[count+1]);
        }
        if (strcmp(argv[count],"-i") == 0){
            arg.input = (char*)malloc (sizeof(char)*(strlen(argv[count+1])+1));
            if (arg.input==NULL){
                printf(MSG_ERROR_MEMORY);
            }
            else strcpy (arg.input, argv[count+1]);
        }
    }
    if (arg.calcType == NO_ASIGN){
        printf(MSG_ERROR_ARG);
        free (arg.output);
        free (arg.input);
        return EXIT_FAILURE;
    }
    if (arg.calcType == SUPER_SEL){
        superCalc(arg.presition,arg.input, arg.output);
        free (arg.output);
        free (arg.input);
    }
    if (arg.calcType == SIMPLE_SEL){
        printMenu();
        option = askOption();      /* Le pido la opcion al usuario */
        if(option == OPT_GRAPH) {                   /* si la opcion es graficador */
            optGraph = askFunction(&errorFlag);               /* pedir los parametros*/
            funcion = SolveFunction(optGraph,&errorFlag);      /* resolver la funcion pedida */
            functionToGraph(funcion,&errorFlag,arg.output);               /* graficarla */
            free (arg.output);
            free (arg.input);
        }
        else if(option == OPT_FACT || option == OPT_BIN) {     /* Si la opcion entrega un long como resultado ir al menu long */
            ansLong = longMenu(option, &errorFlag);
            if (errorFlag == TRUE) {             /* si hay error en las operaciones: Salir */
                free (arg.output);
                free (arg.input);
                return EXIT_FAILURE;
            }
            else {                               /*si no hay error, imprimir el resultado y salir */
                printAnsLong(ansLong);
                free (arg.output);
                free (arg.input);
                return EXIT_SUCCESS;
            }
        }
        else if(option>9 ||option<1) {
            errorFlag = TRUE;
            printf(MSG_ERROR_OPTION);
            free (arg.output);
            free (arg.input);
            return EXIT_FAILURE;
        }
        else {
            ansFloat = floatMenu(option, &errorFlag);
            if (errorFlag == TRUE) {     /* si hay error en las operaciones: Salir */
                free (arg.output);
                free (arg.input);
                return EXIT_FAILURE;
            }
            else {
                printAnsFloat(ansFloat);        /*si no hay error, imprimir el resultado y salir */
                free (arg.output);
                free (arg.input);
                return EXIT_SUCCESS;
            }
        }
    }
    return EXIT_SUCCESS;
}

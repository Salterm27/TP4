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
    if (argc == 1) {    /* Si hay pocos parametros de entrada*/
        printf(MSG_ERROR_ARG);
        return EXIT_FAILURE;
    }
    arg.presition = 100; /*default: precision*/
    arg.calcType = NO_ASIGN;/*default: no se asigno ninguna calculadora valida*/
    for (count = 1; count<argc;count++) { /* circulo por los argumentos*/
        if (strcmp(argv[count],PRESITION_TOLKEN) == 0){ /* si hay -p*/
            if (atoi(argv[count+1]) <= 0) { /*el siguiente contiene la precision, veo que sea mayor a 0*/
                printf(MSG_ERROR_ARG_PRESITION);
                return EXIT_FAILURE;
            }
            else arg.presition = atoi(argv[count+1]);
        }
        if (strcmp(argv[count],SUPERCALC_TOLKEN) == 0){ /* si contiene superCalc, defino la calculadora*/
            arg.calcType = SUPER_SEL;
        }
        if ((strcmp(argv[count],SIMPLECALC_TOLKEN) == 0)) {/* si contiene simpleCalc, defino la calculadora*/
            arg.calcType = SIMPLE_SEL;
        }
        else if (count == argc) { /*si no hay ninguna calculadora asignada, mensaje de error*/
            printf(MSG_ERROR_INPUT);
            return EXIT_FAILURE;
        }
        if (strcmp(argv[count],OUTPUT_TOLKEN) == 0){ /* si contiene -o, el siguiente argumento sera el nombre del archivo*/
            arg.output = (char*)malloc (sizeof(char)*(strlen(argv[count+1])+1));
            if (arg.output==NULL){
                printf(MSG_ERROR_MEMORY);
                return EXIT_FAILURE;
            }
            else strcpy (arg.output, argv[count+1]);
        }
        if (strcmp(argv[count],INPUT_TOLKEN) == 0){/* si contiene -i, el siguiente argumento sera el nombre del archivo*/
            arg.input = (char*)malloc (sizeof(char)*(strlen(argv[count+1])+1));
            if (arg.input==NULL){
                printf(MSG_ERROR_MEMORY);
                return EXIT_FAILURE;
            }
            else strcpy (arg.input, argv[count+1]);
        }
    }
    if (arg.calcType == NO_ASIGN){ /* si nunca asignamos un tipo, tira error y sale del programa*/
        printf(MSG_ERROR_ARG);
        free (arg.output);
        free (arg.input);
        return EXIT_FAILURE;
    }
    if (arg.calcType == SUPER_SEL){ /* entro en la etapa de supercalc*/
        if (superCalc(arg.presition,arg.input, arg.output) == EXIT_SUCCESS){ /*hago supercalc, si sale algo mal, salgo*/
            free (arg.output);                                               /*con exit failure, si no con success*/
            free (arg.input);
            return EXIT_SUCCESS;
        }
        else{
            free (arg.output);
            free (arg.input);
            return EXIT_FAILURE;
        }
    }
    if (arg.calcType == SIMPLE_SEL){ /* entro en la etapa de simplecalc*/
        printMenu();
        option = askOption();      /* Le pido la opcion al usuario */
        if(option == OPT_GRAPH) {                   /* si la opcion es graficador */
            optGraph = askFunction(&errorFlag);               /* pedir los parametros*/
            funcion = SolveFunction(optGraph,&errorFlag);      /* resolver la funcion pedida */
            functionToGraph(funcion,&errorFlag,arg.output);               /* graficarla */
            free (arg.output);
            free (arg.input);
            return EXIT_SUCCESS;
        }
        free (arg.output); /* a partir de aca no voy a usar ningun argumento -i -o, los libero */
        free (arg.input);
        if(option == OPT_FACT || option == OPT_BIN) {     /* Si la opcion entrega un long como resultado ir al menu long */
            ansLong = longMenu(option, &errorFlag);
            if (errorFlag == TRUE) {             /* si hay error en las operaciones: Salir */
                return EXIT_FAILURE;
            }
            else {                               /*si no hay error, imprimir el resultado y salir */
                printAnsLong(ansLong);
                return EXIT_SUCCESS;
            }
        }
        else if(option>9 ||option<1) {
            errorFlag = TRUE;
            printf(MSG_ERROR_OPTION);
            return EXIT_FAILURE;
        }
        else {
            ansFloat = floatMenu(option, &errorFlag);
            if (errorFlag == TRUE) {     /* si hay error en las operaciones: Salir */
                return EXIT_FAILURE;
            }
            else {
                printAnsFloat(ansFloat);        /*si no hay error, imprimir el resultado y salir */
                return EXIT_SUCCESS;
            }
        }
    }
    return EXIT_SUCCESS;
}

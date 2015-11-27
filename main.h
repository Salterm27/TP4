#ifndef __MAIN_H__
#define __MAIN_H__

/*---Mensaje del menu--------------------------------------------------------------------------------------------*/
#define MENU_TITULO "\t\tMENU\n"
#define MENU_SEPARATOR "==============\n"
#define MENU_OP1 "1 - Suma\n"
#define MENU_OP2 "2 - Resta\n"
#define MENU_OP3 "3 - Division\n"
#define MENU_OP4 "4 - Multiplicacion\n"
#define MENU_OP5 "5 - Potencia de exponente entero\n"
#define MENU_OP6 "6 - Factorial\n"
#define MENU_OP7 "7 - Numero combinatorio\n"
#define MENU_OP8 "8 - Numero Aleatorio \n"
#define MENU_OP9 "9 - Graficador \n"
#define MENU_ERROR "\nLa opcion seleccionada no es valida\n"

/*------Mensajes-------------------------------------------------------------------------------------------------*/
#define MSG_ASK_OPTION "Ingrese la opcion que desee: "
#define MSG_INPUT_R1   "Ingrese el numero R1: "
#define MSG_INPUT_R2   "Ingrese el numero R2: "
#define MSG_INPUT_Z    "Ingrese el numero Z: "
#define MSG_INPUT_N    "Ingrese el numero N: "
#define MSG_INPUT_K    "Ingrese el numero K: "
#define MSG_INPUT_R    "Ingrese el numero R: "
#define MSG_ANS        "Resultado: "

/*--Error--*/
#define MSG_ERROR_OPTION "La opcion ingresada es incorrecta\n\n"
#define MSG_ERROR_DIV   "No se puede dividir por 0.\n\n"
#define MSG_ERROR_N     "El numero N no puede ser negativo.\n\n"
#define MSG_ERROR_COMB1 "K y N deben ser positivos.\n\n"
#define MSG_ERROR_COMB2 "el numero K tiene que ser menor a N.\n\n"
#define MSG_ERROR_POT   "No se puede elevar 0 a un exponente negativo.\n\n"
#define MSG_ERROR_FACT1  "El numero N no puede ser negativo\n\n"
#define MSG_ERROR_FACT2  "El numero N no puede ser mayor que 12\n\n"
#define MSG_ERROR_RAND  "La cota inferior debe ser menor a la superior\n\n"
#define MSG_ERROR_GRAPH_INPUT "La opcion ingresada es incorrecta.\n\n"
#define MSG_ERROR_GRAPH_LOG "No se puede hacer el logaritmo e de un numero negativo.\n\n"
#define MSG_ERROR_INPUT "Parametro de entrada mal ingresado"
#define MSG_ERROR_ARG "error de argumento de entrada"
#define MSG_ERROR_ARG_PRESITION "Error en la precision del argumento"
#define MSG_ERROR_MEMORY "error de memoria"

/*--Tamaño--*/
#ifndef __PIXEL_LENGTH__
#define X_LENGTH_PIXELS 600
#define Y_LENGTH_PIXELS 600
#endif

#ifndef __OPTION_T__
#define __OPTION_T__
typedef enum{OPT_ADD=1,OPT_SUB=2,OPT_DIV=3,OPT_PROD=4,OPT_EXP=5,OPT_FACT=6,OPT_BIN=7,OPT_RAND=8,OPT_GRAPH=9}option_t;
#endif

#ifndef __OPTION_GRAPH_T__
#define __OPTION_GRAPH_T__
typedef enum{OPT_TRI=1,OPT_POL=2,OPT_ESP=3}option_graph_t;
typedef enum{OPT_COSENO=2,OPT_SENO=1}option_tri_t;
typedef enum{OPT_GRAD_0=0,OPT_GRAD_1=1,OPT_GRAD_2=2,OPT_GRAD_3=3}option_pol_t;
typedef enum{OPT_LOG=1,OPT_EXPO=2}option_esp_t;
#endif

#ifndef __BOOL_T__
#define __BOOL_T__
typedef enum{TRUE=1,FALSE=0}bool_t;
#endif

#ifndef __GRAPH_OPTION_T__
#define __GRAPH_OPTION_T__
typedef struct graphOption
{
    option_graph_t option;
    int subOption;
    float paramA;
    float paramB;
    float paramC;
    float paramD;
}t_graph_opt;
#endif

#ifndef __FUNC_T__
#define __FUNC_T__
typedef struct func
{
    float x[X_LENGTH_PIXELS];
    float y[Y_LENGTH_PIXELS];
}t_func;
#endif

#ifndef __ARGC_T__
#define __ARGV_T__
typedef struct arg
{
    int presition;
    int calcType;
}t_arg;
#endif

#endif

#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#define OPT_LENGTH 6

#ifndef __PIXEL_LENGTH__
#define X_LENGTH_PIXELS 600
#define Y_LENGTH_PIXELS 600
#endif

/*--Tamaños--*/
#define X_SUP 10.
#define X_INF 0.
#define Y_SUP 5.
#define Y_INF -5.
#define NORM_LENGTH 5

/*--Transformaciones--*/
#define APX (X_SUP-X_INF)/(X_LENGTH_PIXELS)
#define BPX (X_INF)
#define AGX (X_LENGTH_PIXELS)/(X_SUP-X_INF)
#define BGX -AGX*(X_INF)
#define APY (Y_INF-Y_SUP)/(Y_LENGTH_PIXELS)
#define BPY (Y_SUP)
#define AGY (Y_LENGTH_PIXELS)/(Y_INF-Y_SUP)
#define BGY -AGY*(Y_SUP)

/*--Menu--*/
#define GRAPH_TRI "1-Trigonometricas \n"
#define GRAPH_POL "2-Polinomicas \n"
#define GRAPH_ESP "3-Funciones Especiales \n"
#define GRAPH_TRI_COS "2-Coseno \n"
#define GRAPH_TRI_SIN "1-Seno \n"
#define GRAPH_POL_0 "0-Grado 0 \n"
#define GRAPH_POL_1 "1-Grado 1 \n"
#define GRAPH_POL_2 "2-Grado 2 \n"
#define GRAPH_POL_3 "3-Grado 3 \n"
#define GRAPH_ESP_LOG "1-Logaritmica \n"
#define GRAPH_ESP_EXP "2-Exponencial \n"

#define MSG_INPUT_AMP    "Ingrese la Amplitud: "
#define MSG_INPUT_PH    "Ingrese la Fase: "
#define MSG_INPUT_PER    "Ingrese el Periodo: "
#define MSG_INPUT_A    "Ingrese a: "
#define MSG_INPUT_B    "Ingrese b: "
#define MSG_INPUT_C    "Ingrese c: "
#define MSG_INPUT_D    "Ingrese d: "

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


t_graph_opt askFunction(bool_t*);
t_func initializeXY(void);
t_func SolveFunction(t_graph_opt,bool_t*);
void functionToGraph(t_func,bool_t*);
void printMatrizPMB(bool_t[Y_LENGTH_PIXELS][X_LENGTH_PIXELS]);
/*--Transformaciones--*/
float xPixToGraph(int X);
int xGraphToPix(float X);
float yPixToGraph(int Y);
int yGraphToPix(float Y);

#endif

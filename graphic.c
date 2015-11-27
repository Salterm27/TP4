#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphic.h"
#include "main.h"
#include "simplecalc.h"

t_graph_opt askFunction(bool_t* errorFlagPointer)
{
    struct graphOption opt;
    printf(MENU_SEPARATOR);
    printf(MENU_OP9);
    printf(MENU_SEPARATOR);
    printf(GRAPH_TRI);
    printf(GRAPH_POL);
    printf(GRAPH_ESP);
    opt.option = askOption();
    switch (opt.option)
    {
        case OPT_TRI:
            printf(MENU_SEPARATOR);
            printf(GRAPH_TRI);
            printf(MENU_SEPARATOR);
            printf(GRAPH_TRI_SIN);
            printf(GRAPH_TRI_COS);
            opt.subOption = askOption();
            if (opt.subOption>3 ||  opt.subOption<1)  /*Chequeo error de input*/
            {
                *errorFlagPointer = TRUE;
                printf(MSG_ERROR_GRAPH_INPUT);
                break;
            }
            printf(MSG_INPUT_AMP);
            scanf("%f", &opt.paramA);
            printf(MSG_INPUT_PER);
            scanf("%f", &opt.paramB);
            printf(MSG_INPUT_PH);
            scanf("%f", &opt.paramC);
            break;
        case OPT_POL:
            printf(MENU_SEPARATOR);
            printf(GRAPH_POL);
            printf(MENU_SEPARATOR);
            printf(GRAPH_POL_0);
            printf(GRAPH_POL_1);
            printf(GRAPH_POL_2);
            printf(GRAPH_POL_3);
            opt.subOption = askOption();
            printf ("\n\n");
            if (opt.subOption>3 ||  opt.subOption<0)  /*Chequeo error de input*/
            {
                *errorFlagPointer = TRUE;
                printf(MSG_ERROR_GRAPH_INPUT);
                break;
            }
            printf(MSG_INPUT_A);
            scanf("%f", &opt.paramA);
            if (opt.subOption<1)
                break;
            printf(MSG_INPUT_B);
            scanf("%f", &opt.paramB);
            if ( opt.subOption<2)
                break;
            printf(MSG_INPUT_C);
            scanf("%f", &opt.paramC);
            if ( opt.subOption<3)
                break;
            printf(MSG_INPUT_D);
            scanf("%f", &opt.paramD);
            break;
        case OPT_ESP:
            printf(MENU_SEPARATOR);
            printf(GRAPH_ESP);
            printf(MENU_SEPARATOR);
            printf(GRAPH_ESP_EXP);
            printf(GRAPH_ESP_LOG);
             opt.subOption = askOption();
            if (opt.subOption>2 ||  opt.subOption<1)  /*Chequeo error de input*/
            {
                *errorFlagPointer = TRUE;
                printf(MSG_ERROR_GRAPH_INPUT);
                break;
            }
            printf(MSG_INPUT_A);
            scanf("%f", &opt.paramA);
            printf(MSG_INPUT_B);
            scanf("%f", &opt.paramB);
            printf(MSG_INPUT_C);
            scanf("%f", &opt.paramC);
            break;
        default:
            printf(MSG_ERROR_GRAPH_INPUT);
            break;
    }
return opt;
}
/*------------------------------------------*/
t_func initializeXY(void)
/*inicializa f.x(i)= equidistantes y f.y(i)=0*/
{
    struct func funcion;
    int i;

    for (i = 0 ; i < X_LENGTH_PIXELS ; i++)
    {
        funcion.x[i]=xPixToGraph(i);
        funcion.y[i]=0;
    }
    return funcion;
}
/*------------------------------------------*/

t_func SolveFunction(t_graph_opt opt,bool_t* errorFlagPointer)
/*resuelve la funcion y lo guarda en una estructura [f.x(600),f.y(600)] */
{
    int i;
    struct func funcion;

    funcion = initializeXY();

    switch (opt.option)
    {
        case OPT_TRI:
           if (opt.subOption == OPT_SENO)
            {
                for (i=0;i<X_LENGTH_PIXELS;i++)
                {
                    funcion.y[i] = opt.paramA*sin((funcion.x[i]/opt.paramB)+opt.paramC);
                }
            }
            else
            {
                for (i=0;i<X_LENGTH_PIXELS;i++)
                {
                    funcion.y[i] = opt.paramA*cos((funcion.x[i]/opt.paramB)+opt.paramC);
                }
            }
            break;
        case OPT_POL:
            if (opt.subOption == OPT_GRAD_0)
            {
                for (i=0;i<X_LENGTH_PIXELS;i++)                                           /* Funcion polinomica de grado 0 */
                {
                    funcion.y[i] = opt.paramA;
                }
            }
            else if (opt.subOption == OPT_GRAD_1)
            {
                for (i=0;i<X_LENGTH_PIXELS;i++)
                {
                    funcion.y[i] = (opt.paramA*funcion.x[i]) + opt.paramB;                /* Funcion polinomica de grado 1 */
                }
            }
            else if (opt.subOption == OPT_GRAD_2)
            {
                for (i=0;i<X_LENGTH_PIXELS;i++)
                {
                    funcion.y[i] = (opt.paramA*(funcion.x[i]*funcion.x[i])) +             /* Funcion polinomica de grado 2 */
                                    (opt.paramB*funcion.x[i]) + opt.paramC;
                }
            }
            else
            {
                for (i=0;i<X_LENGTH_PIXELS;i++)
                {
                    funcion.y[i] = (opt.paramA*(funcion.x[i]*funcion.x[i]*funcion.x[i])) + /* Funcion polinomica de grado 3 */
                                    (opt.paramB*(funcion.x[i]*funcion.x[i])) +
                                    (opt.paramC*(funcion.x[i])) + opt.paramD;
                }
            }
            break;
        case OPT_ESP:
            if (opt.subOption == OPT_LOG)
            {
                for (i=0;i<X_LENGTH_PIXELS;i++)
                {
                    funcion.y[i] = (opt.paramA*log((opt.paramA*funcion.x[i])+opt.paramB))+opt.paramC;
                }
            }
            else
            {
                for (i=0;i<X_LENGTH_PIXELS;i++)
                {
                    funcion.y[i] = (opt.paramA*exp((opt.paramA*funcion.x[i])+opt.paramB))+opt.paramC;
                }
            }
            break;
        default:
            *errorFlagPointer = TRUE;
            printf(MSG_ERROR_GRAPH_INPUT);
            break;
    }
    return funcion;
}
/*------------------------------------------*/
void functionToGraph(t_func funcion,bool_t* errorFlagPointer)
{

    int x,y,i;
    bool_t matGraph[Y_LENGTH_PIXELS][X_LENGTH_PIXELS];

    for (y=0;y<Y_LENGTH_PIXELS;y++)
        for (x=0;x<X_LENGTH_PIXELS;x++)             /*inicializo matGraph en 0*/
        {
                matGraph[x][y] = FALSE;
        }

    for (x=0;x<X_LENGTH_PIXELS;x++)
    {
        if (yGraphToPix(funcion.y[x])<Y_LENGTH_PIXELS && yGraphToPix(funcion.y[x])>0) /*veo que no se pase de rango*/
        {
            matGraph[yGraphToPix(funcion.y[x])][x] = TRUE;  /*le asigno 1 al espacio [y][x]*/
        }
        matGraph[yGraphToPix(0)][x] = TRUE; /* Eje x */
    }
    for (y=0;y<Y_LENGTH_PIXELS;y++)
        matGraph[y][xGraphToPix(0)] = TRUE; /* Eje y */

    for (y=yGraphToPix(0)-NORM_LENGTH;y<=yGraphToPix(0)+NORM_LENGTH;y++)
        for (x=xGraphToPix(0),i=X_INF ; i<=X_SUP ; i++)
        {
            x=xGraphToPix(i);
            if ((x<X_LENGTH_PIXELS && x>0) && (y<Y_LENGTH_PIXELS && y>0))
                matGraph[y][x]=TRUE;
        }


    for (x=xGraphToPix(0)-NORM_LENGTH;x<=xGraphToPix(0)+NORM_LENGTH;x++)
        for (y=yGraphToPix(0),i=Y_INF;i<=Y_SUP;i++)
        {
            y=yGraphToPix(i);
            if ((x<X_LENGTH_PIXELS && x>0) && (y<Y_LENGTH_PIXELS && y>0))
                matGraph[y][x]=TRUE;
        }


    printMatrizPMB(matGraph);
    return;
}
/*------------------------------------------*/
void printMatrizPMB(bool_t mat[Y_LENGTH_PIXELS][X_LENGTH_PIXELS])
{
    int x,y;
    fprintf(stderr,"P1");       /* Header */
    fprintf(stderr,"\n");
    fprintf(stderr,"%d %d",Y_LENGTH_PIXELS,X_LENGTH_PIXELS); /* dimensiones de la matriz */
    fprintf(stderr,"\n");
    for (y=0;y<Y_LENGTH_PIXELS;y++)
    {
        for (x=0;x<X_LENGTH_PIXELS;x++)
        {
            fprintf(stderr,"%d ",mat[y][x]);
        }
    fprintf(stderr,"\n");
    }

    return;
}
/*------------------------------------------*/
float xPixToGraph(int X)
{
    float ans;
    ans = APX*X+BPX;
    return ans;
}
/*------------------------------------------*/
int xGraphToPix(float X)
{
    float ans;
    return AGX*X+BGX;
    return ans;
}
/*------------------------------------------*/
float yPixToGraph(int Y)
{
    float ans;
    return APY*Y+BPY;
    return ans;
}
/*------------------------------------------*/
int yGraphToPix(float Y)
{
    float ans;
    return AGY*Y+BGY;
    return ans;
}
/*------------------------------------------*/

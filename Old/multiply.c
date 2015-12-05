/*-----MULTIPLICACION----*/
result_state_t multiply(operation_t* oper,int precision)
{
    /*VARIABLES*/
    int i=0,j=0;
    short suma=0;
    short *bufferAns;
    /*PRECISION*/
    (*oper).ans.numSize=precision;
    if ((*oper).num1.numSize + (*oper).num2.numSize  > precision+1)
    {
        return OFW;
    }
    bufferAns = (short*)calloc(precision,sizeof(short));
    if (bufferAns == NULL) /*si hay error de alojamiento de memoria*/
    {
        printf(MSG_ERROR_MEMORY);
        return ERR;
    }
    for (i=0;i<(*oper).num2.numSize;i++) /*RECORRO VECTOR 2*/
    {
        suma = 0;
        for (j=0;j<(*oper).num1.numSize;j++)/*RECORRO VECTOR 1*/
        {
            suma = (*oper).num1.num[j]*(*oper).num2.num[i] + bufferAns[i+j] + suma/10;
            bufferAns[i+j] = suma%10;
        }
        if (i<(*oper).num2.numSize-1)
            bufferAns [j+i] = suma/10;
    }
    if (suma/10)
    {
        if ((*oper).num1.numSize +(*oper).num2.numSize-1 < precision)
            bufferAns[(*oper).num1.numSize + (*oper).num2.numSize -1] = suma/10;
        else
            return OFW;
    }
    (*oper).ans.num = bufferAns;
    for (i=0;i<(*oper).ans.numSize && (*oper).ans.num[precision-1-i] == 0;i++) /* redefino tamaño */
        (*oper).ans.numSize--;

    return OK;
}
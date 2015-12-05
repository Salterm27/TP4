/*---------SUMAR NUMEROS-------*/
result_state_t addNumbers(operation_t* oper,int precision)
{
    int i;
    bool_t endFor = FALSE;
    (*oper).ans.num = (short*)calloc(precision+1,sizeof(short));
    if ((*oper).ans.num == NULL)
    {
        printf(MSG_ERROR_MEMORY);
        return ERR;
    }
    for (i=0;i<precision;i++)
    {
        (*oper).ans.num[i] += (*oper).num1.num[i]+(*oper).num2.num[i];
        if ((*oper).ans.num[i] > 9)
        {
            (*oper).ans.num[i+1] = (*oper).ans.num[i]/10;
            (*oper).ans.num[i] = (*oper).ans.num[i]%10;
        }
    }

    if ((*oper).ans.num[precision] != 0)
    {
        return OFW;
    }
    (*oper).ans.numSize = precision;
    for (i=precision-1,endFor=FALSE;i>=0 && endFor == FALSE;i--)
    if ((*oper).ans.num[i] == 0)
    {
            (*oper).ans.numSize--;
    }
    else
    {
        endFor=TRUE;
    }
    return OK;
}
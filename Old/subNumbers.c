/*-----RESTAR NUMEROS---------*/
result_state_t subNumbers(operation_t* oper,int precision)
{
    int i,borrow=0,size;
    (*oper).ans.numSize = precision;
    if (superior((*oper).num1,(*oper).num2) == 2)
    {
        for(i=0;i<(*oper).num2.numSize;i++)
        {
            (*oper).ans.num[i] = (*oper).num2.num[i]-(*oper).num1.num[i]-borrow;
            if ((*oper).ans.num[i] < 0)
            {
                (*oper).ans.num[i] += 10;
                borrow = 1;
            }
            else
                borrow = 0;
        }
    }
    else
    {
        for(i=0;i<(*oper).num1.numSize;i++)
        {
            (*oper).ans.num[i] = (*oper).num1.num[i]-(*oper).num2.num[i]-borrow;
            if ((*oper).ans.num[i] < 0)
            {
                (*oper).ans.num[i] += 10;
                borrow = 1;
            }
            else
                borrow = 0;
        }
    }

    for (i=0,size=(*oper).ans.numSize;i<(*oper).ans.numSize && (*oper).ans.num[precision-1-i] == 0;i++)
    {
        size--;
    }
    (*oper).ans.numSize = size;
    return OK;
}
/*-----RESTA-------*/
result_state_t substraction(operation_t* oper,int precision)
{
    (*oper).ans.num = (short*)calloc(precision,sizeof(short));
    if ((*oper).ans.num == NULL)
    {
        printf(MSG_ERROR_MEMORY);
        return ERR;
    }
    (*oper).signAns = FALSE; /*asumo resultado positivo*/
    switch (superior((*oper).num1,(*oper).num2))
    {
        case 1: /* num1 > num2*/
        {
            if ((*oper).sign1 == TRUE)
                (*oper).signAns = TRUE;
            return subNumbers(oper,precision);
        }
        case 2: /* num2>num1 */
        {
            if ((*oper).sign2 == TRUE)
                (*oper).signAns = TRUE;
            return subNumbers(oper,precision);
        }
        default: /* iguales */
        {
            (*oper).signAns = FALSE;
            (*oper).ans.numSize = 1;
            (*oper).ans.num[0]=0;
            return OK;
        }
    }
}
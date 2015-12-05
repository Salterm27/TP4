/*-----RESTA-------*/
result_state_t substraction(operation_t* oper,int precision)
{

    (*oper).signAns = FALSE; /*asumo resultado positivo*/
    switch (superior(*oper))
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
            addValue(&((*oper).ans),0);
            return OK;
        }
    }
}
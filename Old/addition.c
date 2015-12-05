/*----------SUMAS--------*/
result_state_t addition(operation_t* oper,int precision)
{

    /*si los 2 son - o +, se suman los numeros y se pone el signo que corresponda*/
    if ((*oper).sign1 == TRUE && (*oper).sign2 == TRUE)
    {
        (*oper).signAns = TRUE;
        return addNumbers(oper,precision);
    }
    if ((*oper).sign1 == FALSE && (*oper).sign2 == FALSE)
    {
        (*oper).signAns = FALSE;
        return addNumbers(oper,precision);
    }
    /*si no se restan*/
    return substraction(oper,precision);
}
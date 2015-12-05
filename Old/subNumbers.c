/*-----RESTAR NUMEROS---------*/
result_state_t subNumbers(operation_t* oper,int precision)
{
    int i,borrow=0;
    t_nodo *lista1, *lista2, *ans;
    if (superior((*oper).num1,(*oper).num2) == 2)
        {
        lista1=(*oper).num2;
        lista2=(*oper).num1;
    else {
        lista1=(*oper).num1;
        lista2=(*oper).num2;
    }
    while (lista1->sig != NULL) /*recorre hasta el final*/
        lista1=lista1->sig;
    while (lista2->sig != NULL) /*recorre hasta el final*/
        lista2=lista2->sig;
    while (lista1 != NULL || lista2 != NULL)
    {
        if (lista1 == NULL){
            resultado = lista2->val-borrow;
            lista2=lista2->ant;
        }
        else if (lista2==NULL){
            resultado = lista1->val-borrow;
            lista1=lista1->ant;
        }
        else{
            resultado = lista1->val - lista2->val - borrow;
            lista2=lista2->ant;
            lista1=lista1->ant;
        }
        if (resultado < 0) {
            borrow = 1;
            resultado = resultado + 10;
        }
        else
            borrow = 0;
        if (addValue(&ans,resultado) == EXIT_FAILURE)
            return ERR;
    }
    (*oper).ans = ans;
    while (ans != NULL) { /*recorre hasta el final*/
        i++;
        ans=ans->sig;
    }
    if (i>precision)
        return OFW;
    return OK;
}
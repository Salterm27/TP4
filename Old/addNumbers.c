result_state_t addNumbers(operation_t* oper,int precision)
{
    int i,carry=0, resultado;
    t_nodo *lista1, *lista2, *ans=NULL;
    lista1=(*oper).num1;
    lista2=(*oper).num2;
    while (lista1->sig != NULL) /*recorre hasta el final*/
        lista1=lista1->sig;
    while (lista2->sig != NULL) /*recorre hasta el final*/
        lista2=lista2->sig;
    while (lista1 != NULL || lista2 != NULL)
    {
        if (lista1 == NULL){
            resultado = lista2->val+carry;
            lista2=lista2->ant;
        }
        else if (lista2==NULL){
            resultado = lista1->val+carry;
            lista1=lista1->ant;
        }
        else{
            resultado = lista1->val + lista2->val + carry;
            lista2=lista2->ant;
            lista1=lista1->ant;
        }
        if (resultado > 9) {
            carry = resultado / 10;
            resultado = resultado % 10;
        }
        else
            carry = 0;
        if (addValue(&ans,resultado) == EXIT_FAILURE)
            return ERR;
    }
    if (carry != 0)
        addValue(&ans,carry);
    (*oper).ans = ans;
    while (ans != NULL) { /*recorre hasta el final*/
        i++;
        ans=ans->sig;
    }
    if (i>precision)
        return OFW;
    return OK;
}
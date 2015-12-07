int suma,i,j;
int num1Length=1,num2Length=1;
short* bufferAns=NULL;
while (num1->sig != NULL) { /*recorre hasta el final*/
    num1=num1->sig;
    num1Length++;
    }
while (num2->sig != NULL) { /*recorre hasta el final*/
    num2=num2->sig;
    num2Lenght++;
    }
if (num1Length+num2Length>precision)
    return OFW;
bufferAns = (short*)calloc(precision,sizeof(short));
if (bufferAns == NULL)
    return ERR;
for (j=0; num2!=NULL;j++,num2=num2->ant) /*RECORRO VECTOR 2*/
    {
        suma = 0;
        for (i=0;num1!=NULL;j++,num1=num1->ant)/*RECORRO VECTOR 1*/
        {
            suma = num1->val*num2->val + bufferAns[i+j] + suma/10;
            bufferAns[i+j] = suma%10;
        }
        if (num2->ant != NULL)
            bufferAns [j+i] = suma/10;
    }
    if (suma/10)
    {
        if (num1Length+num2Length-1 < precision)
            bufferAns[num1Length+num2Length -1] = suma/10;
        else
            return OFW;
    }

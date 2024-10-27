#include<stdio.h>
#include<string.h>
#include<malloc.h>

#include"y.tab.h"
#include"fpcstr.h"

/* extern int NBIDENT,NBFUNCTION; */

int AllocIdent(ListIdent *ptr,char *n)
{
 ptr->nom =(char *)(calloc(1,strlen(n)));
 if( !ptr->nom )
    return(0);

 strcpy(ptr->nom,n);
 NBIDENT++;

 return(1);
}

void FreeIdent(ListIdent *ptr)
{
 if( ptr)
    free((void *)ptr->nom);
}

int InsertIdent(ListIdent *list,char *n,int t1,int t2,int fct)
{
 int i,res;

 for(i=0 ; i<NBIDENT ; i++ )
     if( strcmp(list[i].nom,n) == 0 )
       { 
        if( t1==CONSTANTE_ENTIERE||t1==CONSTANTE_REELLE )
           return(i);   /* Constante is Already here: Don't Insert a New One */

        if( list[i].fct == fct || list[i].fct==0)
           return(-1); /* Var is in fonction or in global declarations=>ERROR */
        
       } 

 res = AllocIdent(&list[i],n);
 list[i].type = t1;
 list[i].opertype= t2;
 list[i].fct=fct;

 if( res == 0 )
    return(-1);
 else
    return(NBIDENT-1);
}

int FindIdent(ListIdent *list,char *n,int fct)
{
 int i;
 
 for(i=0 ; i<NBIDENT ; i++ )
    if( strcmp(list[i].nom,n) == 0 )
       if( list[i].fct==fct || list[i].fct==0 ) /* local or global var =>OK */
          return(i);
 
 return(-1);
}


/* Same Declarations But for The Functions */

int AllocFunction(Function *ptr,char *n,int addr)
{
 ptr->nom =(char *)(calloc(1,strlen(n)));
 if( !ptr->nom )
    return(0);

 strcpy(ptr->nom,n);
 ptr->addr = addr;
 return(1);
}

int InsertFunction(Function *list,char *n,int addr)
{
 int i,res;

 for(i=0 ; i<NBFUNCTION ; i++ )
     if( strcmp(list[i].nom,n) == 0 )
           return(-1); /* Var is in fonction or in global declarations=>ERROR */

 res = AllocFunction(&list[i],n,addr);
 if( res == 0 )
    return(-1);
 
 return(1);
}

int FindFunction(Function *list,char *n)
{
 int i;

 for(i=0 ; i<NBFUNCTION ; i++ )
    if( strcmp(list[i].nom,n) == 0 )
          return(i);

 return(-1);
}

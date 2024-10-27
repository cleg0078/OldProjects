#ifndef __LISTIDENT_H
#define __LISTIDENT_H

#include<stdio.h>
#include"fpcstr.h"

int AllocIdent(ListIdent *,char *);
int InsertIdent(ListIdent *,char *,int,int,int);
int FindIdent(ListIdent *,char *,int);

int AllocFunction(Function *,char *,int);
int InsertFunction(Function *,char *,int);
int FindFunction(Function *,char *);


#endif

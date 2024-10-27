#ifndef __FPC_STRUCT_H
#define __FPC_STRUCT_H
#define CONSTANTE_ENTIERE 1
#define CONSTANTE_REELLE 2
#define CONSTANTE_BOOL 3
#define InterSymbol Symbole

typedef struct listident
{
 char *nom;
 char type;
 int fct;    /* Fonction Place: 0=Main; >0 Other Function or procedure */ 
 int place,opertype;
 int usedtime;   /* For Code Optimization: Delete Unsused Code */
 int eval;
 float fval;
}ListIdent;

typedef struct fpcval
{
 int place,type,opertype;
 char nom[32];
 int eval;
 float fval;
}FPCval;

typedef struct symbole
{
 int Oper;
 int Oper1,Oper2,Res;
}Symbole;

typedef struct interident
{
 int opertype;      /* INT,REAL,BOOL */
 int eval;
 float fval;
 char nom[32]; 
 char nbf;
}InterIdent;

typedef struct function
{
 char *nom;
 int num,addr,ret; /* return is set to 0 If Procedure Or 1 if Function */
 int nbarg,retargtype;
 int arg[32];   /* Limitation: 32 Arguments Per Function */
}Function;

#endif


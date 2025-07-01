%{
#include<stdio.h>
#include<stdlib.h>
#include"string.h"
#include"listid.h"
#include"fpcstr.h"
#define BestType(a,b) (a>b?a:b)
#define Push(a) if(stackptr<200) Stack[stackptr++]=a;
#define Pop() stackptr>0?Stack[--stackptr]:-1

#define YYSTYPE FPCval

YYSTYPE yylval;
int tmp,tmp2,actoper_type;
int nbtmp,nbligne,nbfct;
int NBFUNCTION,NBIDENT,NBSYMBOL;
int nberror,opt;
ListIdent IdentHead[1000];
Symbole Symtab[3500];
Function FctHead[256];
int Stack[200],stackptr;
int StackAddr[200],stackptrAddr;

void error(int,char *,char *);
void warning(int,char *,char *);

void BuildPseudo(int,int,int,int);
int GenerateTMP(int);
int BuildOper(int,int,int);
int BuilUnaryOper(int,int,int);

%}

%token OR AND NOT AFFECTATION INFERIEUR SUPERIEUR INFERIEUR_EGAL
%token SUPERIEUR_EGAL EGALITE DIFFERENCE PLUS MOINS UNARY_PLUS UNARY_MOINS
%token MULTIPLIER DIVISER MODULO TYPE_BOOL TYPE_ENTIER TYPE_REAL PBEGIN PEND
%token PARENTHESE_OUVERTE PARENTHESE_FERMEE FIN_INSTRUCTION VIRGULE
%token DOUBLE_POINT IF THEN ELSE GOTO WHILE PROGRAM FUNCTION PROCEDURE VAR
%token CONSTANTE IDENTIFICATEUR WRITE WRITELN 
%token JUMP_EQ COS SIN LN EXP SQRT ABS POINT PUSHADDR POPJUMP PUSHARG
%token POPARG RETURN UNARY

%left NOT AND OR MOINS PLUS UNARY_PLUS UNARY_MOINS
%left MULTIPLIER DIVISER MODULO
%left SUPERIEUR INFERIEUR INFERIEUR_EGAL 
%left SUPERIEUR_EGAL EGALITE DIFFERENCE
%left UNARY
%right AFFECTATION 
%start program

%%

start_declarations:
             | start_declarations declarations
/*             | start_declarations error 
*/
;

declarations:
              TYPE_ENTIER { actoper_type=TYPE_ENTIER; } declarations_var FIN_INSTRUCTION
             |TYPE_REAL { actoper_type=TYPE_REAL; } declarations_var FIN_INSTRUCTION  
             |TYPE_BOOL { actoper_type=TYPE_BOOL; } declarations_var FIN_INSTRUCTION  
;

declarations_var: IDENTIFICATEUR { if( InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION) < 0 ) error(nbligne,"Duplicate Id ",yylval.nom); }
| declarations_var VIRGULE 
IDENTIFICATEUR                   { if( InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION) < 0 )  error(nbligne,"Duplicate Id ",yylval.nom); }
;


start_input_parameters:
               | start_input_parameters input_parameters
               | start_input_parameters error
;

input_parameters:
            TYPE_ENTIER { actoper_type=TYPE_ENTIER; } IDENTIFICATEUR {
                                                                      if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 ) 
                                                                          error(nbligne,"Duplicate Id ",yylval.nom);  
                                                                       FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
             
                                                                     }
 	   | TYPE_REAL { actoper_type=TYPE_REAL; } IDENTIFICATEUR { 
                                                                   if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 ) 
	                                                               error(nbligne,"Duplicate Id ",yylval.nom); 
  	                                                            FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
                                                                  }
           | TYPE_BOOL { actoper_type=TYPE_BOOL; } IDENTIFICATEUR {
                                                                  if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 )
                                                                     error(nbligne,"Duplicate Id ",yylval.nom);
                                                                   FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
                                                                  }
           | input_parameters VIRGULE input_parameters
;
            
start_getparameters:
	| start_getparameters getparameters
        | start_getparameters error
;

getparameters:
         expressions_logiq { BuildPseudo(PUSHARG,-1,-1,$1.place); }
       | getparameters VIRGULE expressions_logiq { BuildPseudo(PUSHARG,-1,-1,$3.place); }
;
	
start_instructions:
            | start_instructions instructions
            | start_instructions error
;

then_else:
	     | THEN instructions { tmp2=0; } 
             | THEN instructions 
               ELSE { tmp2=1; BuildPseudo(GOTO,-1,-1,-1); Push(NBSYMBOL);
                    } 
               instructions { Push(NBSYMBOL); }
;

instructions:
               FIN_INSTRUCTION 
             | expressions_arith 
             | IF expressions_logiq  { Push(NBSYMBOL); BuildPseudo(JUMP_EQ,$2.place,-1,-1); }
               then_else {
                          int res1,res2,res3;
                          if( tmp2==1 ) /* ELSE */
                            {
                             res1=Pop(); res2=Pop(); res3=Pop();
                             Symtab[res2-1].Res=res1;
                             Symtab[res3].Res=res2;
                            }
                          else
                            { 
                             res1=Pop(); 
                             Symtab[res1].Res = NBSYMBOL;
                            }
                         }
             | WHILE { Push(NBSYMBOL); } expressions_logiq { Push(NBSYMBOL); BuildPseudo(JUMP_EQ,$2.place,-1,-1); }
               instructions { int res=Pop();  
                              Symtab[res].Res=NBSYMBOL+1;
                              Symtab[res].Oper1 = Symtab[res-1].Res;
                              BuildPseudo(GOTO,-1,-1,Pop());
                            } 
             | write 
             | writeln
             | PBEGIN start_instructions PEND
             | IDENTIFICATEUR PARENTHESE_OUVERTE start_getparameters PARENTHESE_FERMEE 
               {
                tmp=FindFunction(FctHead,$1.nom);
                if( tmp < 0 )
                   error(nbligne,"Undeclared Function:",$1.nom);

                BuildPseudo(PUSHADDR,-1,-1,NBSYMBOL+2);
                BuildPseudo(GOTO,-1,-1,FctHead[tmp].addr);
               }
             | RETURN { if( FctHead[NBFUNCTION-1].ret != 0 )
                           error(nbligne,"Missing Return Value",NULL);
                        BuildPseudo(POPJUMP,-1,-1,-1); 
                      } 
             | RETURN PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE
                      {
                        if( FctHead[NBFUNCTION-1].ret == 0 )
                           error(nbligne,"Procedure Can't Use A Return Value",NULL);
                        BuildPseudo(PUSHARG,-1,-1,$3.place);
                       }
;

writeln:    WRITELN { BuildPseudo(WRITELN,-1,-1,-1); }
;

write:      WRITE PARENTHESE_OUVERTE 
            write_expr                
            PARENTHESE_FERMEE 
;

write_expr: expressions_logiq   { 
                                  if( $1.type==PROCEDURE )
			             error(nbligne,"Invalid Procedure Affectation",NULL);
                                  BuildPseudo(WRITE,$1.place,-1,-1); 
                                }
           | write_expr VIRGULE expressions_logiq
                                {
                                 if( $3.type == PROCEDURE ) 
                                    error(nbligne,"Invalide Procedure Affectation",NULL);
                                 BuildPseudo(WRITE,$3.place,-1,-1);
                                }
;

expressions_arith:
             terme AFFECTATION expressions_logiq { if( $1.type != IDENTIFICATEUR )
                                                       error(nbligne,"Invalid lvalue Assignment",NULL);
                                                    else
                                                      {/*             ORIGINAL CODE                     */
                                                       /* BuildPseudo(AFFECTATION,$3.place,0,$1.place); */
                                                       /*     But We've include a small Optimization    */
                                                       if( $3.type== PROCEDURE )
                                                          error(nbligne,"Invalid Procedure Assignment\n",NULL);

                                                       if( NBSYMBOL>1 && strncmp(IdentHead[$3.place].nom,"#tmp",4) == 0 && opt)
                                                          Symtab[NBSYMBOL-1].Res = $1.place;  
                                                       else
                                                          BuildPseudo(AFFECTATION,$3.place,-1,$1.place);
                                                      }
                                                  }
            | terme { $$.type = $1.type; $$.place=$1.place; }
;

expressions_logiq: 
              PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE { $$.place=$2.place; }
            | expressions_logiq PLUS expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); } 
            | expressions_logiq MOINS expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); } 
            | expressions_logiq MULTIPLIER expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | expressions_logiq DIVISER expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | expressions_logiq MODULO expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | expressions_logiq DIFFERENCE expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | expressions_logiq OR expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | expressions_logiq AND expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | expressions_logiq INFERIEUR expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | expressions_logiq SUPERIEUR expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | expressions_logiq INFERIEUR_EGAL expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | expressions_logiq SUPERIEUR_EGAL expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | expressions_logiq EGALITE expressions_logiq { $$.place=BuildOper($2.type,$1.place,$3.place); }
            | MOINS expressions_logiq %prec UNARY { $$.place=BuildUnaryOper(UNARY_MOINS,$2.place); }
            | PLUS expressions_logiq %prec UNARY { $$.place=BuildUnaryOper(UNARY_PLUS,$2.place); } 
            | NOT expressions_logiq %prec UNARY { $$.place=BuildUnaryOper(NOT,$2.place); }
            | COS PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE { $$.place=BuildOper($1.type,$3.place,-1); }
            | SIN PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE { $$.place=BuildOper($1.type,$3.place,-1); }
            | LN PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE { $$.place=BuildOper($1.type,$3.place,-1); }
            | SQRT PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE { $$.place=BuildOper($1.type,$3.place,-1); }
            | EXP PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE { $$.place=BuildOper($1.type,$3.place,-1); }  
            | ABS PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE { $$.place=BuildOper($1.type,$3.place,-1); }
            | terme  { $$.type = $1.type; $$.place=$1.place;}
            | IDENTIFICATEUR PARENTHESE_OUVERTE start_getparameters PARENTHESE_FERMEE
              {
               tmp=FindFunction(FctHead,$1.nom);
               if( tmp < 0 )
                  error(nbligne,"Undeclared Function or Procedure",$1.nom);

               BuildPseudo(PUSHADDR,-1,-1,NBSYMBOL+2);
               BuildPseudo(GOTO,-1,-1,FctHead[tmp].addr);
               if( FctHead[tmp].ret !=0 )
                 { 
                  int tmp2;
                  tmp2=GenerateTMP(TYPE_REAL);
                  BuildPseudo(POPARG,-1,-1,tmp2);
                  if(tmp2<0 ) error(nbligne,"Can't Alloc TMP\n","");
                  $$.place=tmp2;
                  $$.type = FUNCTION; 
                 }
               else 
                {
                 $$.type=PROCEDURE;     
                 $$.place = tmp;
                }
              }
;

terme: IDENTIFICATEUR { $$.type=IDENTIFICATEUR; if( ($$.place=FindIdent(IdentHead,yylval.nom,NBFUNCTION)) < 0 ) error(nbligne,"Undeclared Id ",yylval.nom); }
      | CONSTANTE { $$.type=$1.type;  
                    tmp=InsertIdent(IdentHead,yylval.nom,$1.type,$1.type==CONSTANTE_ENTIERE?TYPE_ENTIER:TYPE_REAL,NBFUNCTION); 
                    if( tmp >-1 )                         
                       if($1.type == CONSTANTE_ENTIERE )
                          IdentHead[tmp].eval = $1.eval;
                       else
                          IdentHead[tmp].fval = $1.fval;
                    $$.place=tmp;
                  }
      | PARENTHESE_OUVERTE terme PARENTHESE_FERMEE { $$.type=$2.type; $$.place=$2.place; }

;

procedure:
	PROCEDURE 
        IDENTIFICATEUR { 
                         tmp=FindFunction(FctHead,yylval.nom);
                         if( tmp>=0 ) error(nbligne,"Duplicate Procedure Name: ",yylval.nom);
                         tmp=InsertFunction(FctHead,yylval.nom,NBSYMBOL);
                         FctHead[NBFUNCTION].ret=0;
                         NBFUNCTION++;
                       }
        PARENTHESE_OUVERTE
        start_input_parameters
        PARENTHESE_FERMEE
           { /* Now Build The Argument Stack In INVERSE ORDER */
             for(tmp=FctHead[NBFUNCTION].nbarg-1; tmp>=0 ; tmp-- )
                 BuildPseudo(POPARG,-1,-1,FctHead[NBFUNCTION].arg[tmp]);
           }  
        FIN_INSTRUCTION 
        VAR start_declarations
        PBEGIN start_instructions
        PEND FIN_INSTRUCTION { BuildPseudo(POPJUMP,-1,-1,-1); }
;

function:
	FUNCTION IDENTIFICATEUR
                {
                 tmp=FindFunction(FctHead,yylval.nom);
                 if( tmp>=0 )
                    error(nbligne,"Duplicate Procedure Name: ",yylval.nom);                         tmp=InsertFunction(FctHead,yylval.nom,NBSYMBOL);
                 FctHead[NBFUNCTION].ret=1;
                 NBFUNCTION++;
                }
        PARENTHESE_OUVERTE
        start_input_parameters
        PARENTHESE_FERMEE 
           { /* Now Build The Argument Stack In INVERSE ORDER */
             for(tmp=FctHead[NBFUNCTION].nbarg-1; tmp>=0 ; tmp-- )
                 BuildPseudo(POPARG,-1,-1,FctHead[NBFUNCTION].arg[tmp]);
           }
        FIN_INSTRUCTION
        VAR start_declarations
        PBEGIN start_instructions
        PEND
        FIN_INSTRUCTION { /* Assuming No Fonction return: It's For Stack underflow */
                               BuildPseudo(POPJUMP,-1,-1,-1);
                             }
;


start_proc_fct:
	| start_proc_fct procedure
        | start_proc_fct function
        | start_proc_fct error
;

prog_bloc:
        VAR start_declarations start_proc_fct PBEGIN { Symtab[0].Res=NBSYMBOL; }
        start_instructions PEND
;
 
program:
       prog_bloc POINT
      |
       PROGRAM
       IDENTIFICATEUR
       FIN_INSTRUCTION
       prog_bloc
       POINT
;

%%

#include"lex.yy.c"
#include"listid.c"

void BuildPseudo(int oper,int oper1,int oper2,int res)
{
 Symtab[NBSYMBOL].Oper=oper;
 Symtab[NBSYMBOL].Res=res;
 Symtab[NBSYMBOL].Oper1=oper1;
 Symtab[NBSYMBOL].Oper2=oper2;
/*
 fprintf(stderr,"Oper=%d  RES=%d  NBSYMBOL=%d\n",oper,res,NBSYMBOL);
*/
 NBSYMBOL++;
}

int GenerateTMP(int type)
{
 char buff[16];
 sprintf(buff,"#tmp%d",nbtmp++);
 return(InsertIdent(IdentHead,buff,IDENTIFICATEUR,type,NBFUNCTION) );
}

int BuildOper(int oper,int pl1,int pl2)
{
 int res;
 res=GenerateTMP(BestType(IdentHead[pl1].opertype,
                          IdentHead[pl2].opertype));   
 BuildPseudo(oper,pl1,pl2,res);
 return(res);
}

int BuildUnaryOper(int oper,int pl1)
{
 int res;
 res=GenerateTMP(IdentHead[pl1].opertype);
 BuildPseudo(oper,pl1,-1,res);
 return(res);
}

void CheckUsedTime()
{
 int i,oper,oper1,oper2,res;

 for(i=0;i<NBSYMBOL;i++)
    {
     oper = Symtab[i].Oper;
     oper1=Symtab[i].Oper1; oper2=Symtab[i].Oper2;
     res=Symtab[i].Res;

     if( oper1!=-1 && oper!=GOTO )
        IdentHead[oper1].usedtime++;

     if( oper2!=-1 && oper!=GOTO && oper!=JUMP_EQ)
        IdentHead[oper2].usedtime++;
 
     if( res !=-1 && oper !=GOTO && oper!=JUMP_EQ )
        IdentHead[res].usedtime++;
    }
}

void yyerror(char *str)
{
 nberror++;

 fprintf(stderr,"\n%d: %s Syntax error\n",nbligne,yytext);

 if(nberror>4)
    exit(0);
}

void warning(int nb,char *s1,char *s2)
{
 fprintf(stderr,"\n%d: Warning %s",nb,s1);
 if( s2 )
    fprintf(stderr,"%s",s2);
  fprintf(stderr,"\n");
}

void error(int nb,char *s1,char *s2)
{
 fprintf(stderr,"\n%d: %s",nb,s1);
 if( s2 )
    fprintf(stderr,"%s",s2);

 fprintf(stderr,"\n");
 nberror++;
 if(nberror>4)
    exit(0);
}

void DestroyIdent(int pl)
{
 int i;

 if( NBIDENT >0 )
    NBIDENT--;

 strcpy(IdentHead[pl].nom,IdentHead[NBIDENT].nom);
 IdentHead[pl].type = IdentHead[NBIDENT].type;
 
 IdentHead[pl].usedtime = IdentHead[NBIDENT].usedtime;
 IdentHead[pl].eval = IdentHead[NBIDENT].eval;
 IdentHead[pl].fval = IdentHead[NBIDENT].fval;
 IdentHead[pl].opertype = IdentHead[NBIDENT].opertype;
 
 for(i=0;i<NBSYMBOL;i++)
     if( Symtab[i].Oper != GOTO && Symtab[i].Oper != JUMP_EQ )
       {
        if(Symtab[i].Oper1 == NBIDENT ) 
           Symtab[i].Oper1=pl; 

        if(Symtab[i].Oper2 == NBIDENT )
           Symtab[i].Oper2=pl; 
        if(Symtab[i].Res == NBIDENT )
           Symtab[i].Res = pl;
       }
     else
       if( Symtab[i].Oper == JUMP_EQ )
         if(Symtab[i].Oper1 == NBIDENT )
            Symtab[i].Oper1=pl;
}

void main(int argc,char **argv)
{
 extern char *optarg;
 extern int optind;
 int bflag, ch, fd;
 FILE *in,*out;
 char *name;
 InterIdent inter_ident;
 int i;

 fprintf(stderr,"Fast Pascal Compiler @1996-1998 C.Legendre\n");
 if( argc < 2)
   {
    fprintf(stderr,"No Input File\n");
    exit(0);
   }

 bflag = 0;
 opt=1;
 name=argv[1];

 in = fopen(name,"rt");
 if( !in )
   {
    fprintf(stderr,"Unable To Open Source File: %s\n",argv[1]);
    exit(0);
   }

 NBFUNCTION=0;
 NBSYMBOL=NBIDENT=0;
 nbligne=1;
 nberror=stackptr=stackptrAddr=0;
 nbtmp=nbfct=0;

 memset(IdentHead,0,1000*sizeof(ListIdent));
 memset(Symtab,0,3500*sizeof(Symbole));
 memset(FctHead,0,256*sizeof(Function));

 yyin = in;

 /* Prepare the Jump To The Main Function */
 BuildPseudo(GOTO,-1,-1,-1);

 yyparse();

 fclose(in);
 if( nberror )
   {
    fprintf(stderr,"\nCompiling Error.\n");
    exit(0);
   }
 else
   fprintf(stderr,"\nAll Ok\n");

 out =fopen("FPC.obj","wb");
 if( !out )
   {
    fprintf(stderr,"\nUnable To Generate Object File\n");
    exit(0);
   } 
 
 if( opt )
   {
    fprintf(stderr,"Optimizing...\nPhase I:  Deleting Unused ID\n");
    CheckUsedTime();
    for(i=0;i<NBIDENT;i++)
       {
    /*    fprintf(stderr,"NOM=%s   USEDTIME=%d\n",IdentHead[i].nom,IdentHead[i].usedtime);
    */     
         if( IdentHead[i].usedtime == 0 )
           DestroyIdent(i--);
       }
   }

 fwrite(&NBIDENT,1,sizeof(int),out);
 fwrite(&NBSYMBOL,1,sizeof(int),out);

 for(i=0;i<NBIDENT;i++)
    {
     inter_ident.opertype = IdentHead[i].opertype; 
     inter_ident.eval = IdentHead[i].eval;
     inter_ident.fval = IdentHead[i].fval;
     inter_ident.nbf = IdentHead[i].fct;
     strncpy(inter_ident.nom,IdentHead[i].nom,32);
     fwrite(&inter_ident,1,sizeof(InterIdent),out);
    }       

 for(i=0;i<NBSYMBOL;i++)
    {
     fwrite(&Symtab[i],1,sizeof(InterSymbol),out);
/*
     fprintf(stderr,"Oper=%d  Oper1=%d Oper2=%d Res=%d\n",Symtab[i].Oper,Symtab[i].Oper1,Symtab[i].Oper2,Symtab[i].Res);
*/
    }
 fclose(out);
 fprintf(stderr,"Now Generating Object File...OK\n");
 fprintf(stderr,"Stat:  %d Data Lines    %d Code Lines\n",NBIDENT,NBSYMBOL);

 exit(0); 
}


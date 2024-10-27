#include<stdio.h>
#include<math.h>
#include"y.tab.h"
#include"fpcstr.h"
#define MAXSTACKADDR 3500
#define MAXARGSTACK 1000

InterSymbol *quadtab;
InterIdent  *identtab;
FILE *in,*pout;
static int nbquad,nbident,eval,eval2;
int stackptr;
float fval,fval2;
int StackAddr[MAXSTACKADDR],stackptrAddr;
InterIdent ArgStack[MAXARGSTACK];
int ArgStackPtr;

void GetValue(int *pt1,float *pt2,int pl)
{
 *pt1 = identtab[pl].eval;
 *pt2 = identtab[pl].fval;
 if( identtab[pl].opertype == TYPE_REAL )
    *pt1 = (int)*pt2;
 else
    *pt2 = (float)*pt1;
}

void AssignValue(float f,int pl)
{
 if( identtab[pl].opertype != TYPE_BOOL )
   {
    identtab[pl].fval=f;
    identtab[pl].eval=(int)f;
   }
 else
  {
   if( f!=0.0 )
     {
      identtab[pl].fval=1.0;
      identtab[pl].eval=1;
     }
   else
     {
      identtab[pl].fval=0.0;
      identtab[pl].eval=0;
     }
  }
}  

void GenerateSymTab()
{
 int i,max,max2;

 fprintf(pout,"Symbol Table:\n\n");
 
 for(max=i=0;i<nbident;i++)
    if( (max2=strlen(identtab[i].nom)) >max)
       max=max2;
      
 for(i=0; i<nbident ; i++)
    {
     if( identtab[i].opertype == TYPE_ENTIER )
        fprintf(pout,"%*s ---> %12d [INTEGER]\n",max,identtab[i].nom,identtab[i].eval);
     if( identtab[i].opertype == TYPE_BOOL )
        fprintf(pout,"%*s ---> %12d [BOOLEAN]\n",max,identtab[i].nom,identtab[i].eval);
     if( identtab[i].opertype == TYPE_REAL )
        fprintf(pout,"%*s ---> %6.6f [REAL]\n",max,identtab[i].nom,identtab[i].fval);
    }

 fprintf(pout,"\n\n\n");
}

void GenerateReadablePseudo()
{
 pout = fopen("Pseudo.txt","wt");
 if( pout == NULL )
    fprintf(stderr,"Can't Create Readable Pseudo-Code\n");
    
 fprintf(pout,"/*********************************/\n");
 fprintf(pout,"/*         FPC Pseudo Code       */\n");
 fprintf(pout,"/*********************************/\n");
 fprintf(pout,"/*           C.LEGENDRE          */\n");
 fprintf(pout,"/*********************************/\n\n\n");

 GenerateSymTab();
     
 for(stackptr=0; stackptr <nbquad ;stackptr++)
    {
     switch( quadtab[stackptr].Oper )
           {
            case(WRITELN):
                 fprintf(pout,"%3d: WRITELN\n",stackptr);
                 break;
            case(WRITE):
                 fprintf(pout,"%3d: WRITE %s\n",stackptr,identtab[quadtab[stackptr].Oper1].nom);
                 break;
            case(AFFECTATION):
                 fprintf(pout,"%3d: %s := %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,identtab[quadtab[stackptr].Oper1].nom);
                 break;
            case(PLUS):
                 fprintf(pout,"%3d: %s := %s + %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(MOINS):
                 fprintf(pout,"%3d: %s := %s - %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(MULTIPLIER):
                 fprintf(pout,"%3d: %s := %s * %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(DIVISER):
                 fprintf(pout,"%3d: %s := %s / %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(MODULO):
                 fprintf(pout,"%3d: %s := %s %% %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(OR):
                 fprintf(pout,"%3d: %s := %s OR %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom); 
                 break;
            case(AND):
                 fprintf(pout,"%3d: %s := %s AND %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(DIFFERENCE):
                 fprintf(pout,"%3d: %s := %s <> %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(EGALITE):
                 fprintf(pout,"%3d: %s := %s = %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(INFERIEUR):
                 fprintf(pout,"%3d: %s := %s < %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(JUMP_EQ):   /* test If Oper1=0 */
                 fprintf(pout,"%3d: JUMP_EQ %s,%d\n",stackptr,identtab[quadtab[stackptr].Oper1].nom,quadtab[stackptr].Res);
                 break;
            case(SUPERIEUR):
                 fprintf(pout,"%3d: %s := %s > %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(INFERIEUR_EGAL):
                 fprintf(pout,"%3d: %s := %s <= %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(SUPERIEUR_EGAL):
                 fprintf(pout,"%3d: %s := %s >= %s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom,identtab[quadtab[stackptr].Oper2].nom);
                 break;
            case(UNARY_PLUS):
                 fprintf(pout,"%3d: %s := +%s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom);
                 break;
            case(UNARY_MOINS):
                 fprintf(pout,"%3d: %s := -%s\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                         identtab[quadtab[stackptr].Oper1].nom);
                 break;
             case(NOT):
                  fprintf(pout,"%3d: %s := NOT(%s)\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                          identtab[quadtab[stackptr].Oper1].nom);
                  break;
             case(GOTO):
                  fprintf(pout,"%3d: GOTO %d\n",stackptr,quadtab[stackptr].Res);
                  break;
             case(COS):
                  fprintf(pout,"%3d: %s := COS(%s)\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                          identtab[quadtab[stackptr].Oper1].nom);
                  break;
             case(SIN):
                  fprintf(pout,"%3d: %s := SIN(%s)\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                          identtab[quadtab[stackptr].Oper1].nom);
                  break;
             case(EXP):
                  fprintf(pout,"%3d: %s := EXP(%s)\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                          identtab[quadtab[stackptr].Oper1].nom);
                  break;
             case(LN):
                  fprintf(pout,"%3d: %s := LN(%s)\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                          identtab[quadtab[stackptr].Oper1].nom);
                  break;
             case(SQRT):
                  fprintf(pout,"%3d: %s := SQRT(%s)\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                          identtab[quadtab[stackptr].Oper1].nom);
                  break;
             case(ABS):
                  fprintf(pout,"%3d: %s := ABS(%s)\n",stackptr,identtab[quadtab[stackptr].Res].nom,
                          identtab[quadtab[stackptr].Oper1].nom);
                  break;
             case(POPJUMP):
                  fprintf(pout,"%3d: POPJUMP\n",stackptr);
                  break;
             case(PUSHADDR):
                  fprintf(pout,"%3d: PUSHADDR %d\n",stackptr,quadtab[stackptr].Res);
                  break;
             case(POPARG):
                  fprintf(pout,"%3d: POPARG %s\n",stackptr,
                          identtab[quadtab[stackptr].Res].nom);
                  break;
             case(PUSHARG):
                  fprintf(pout,"%3d: PUSHARG %s\n",stackptr,
                          identtab[quadtab[stackptr].Res].nom);
                  break;
           }
    }
 fprintf(pout,"%3d: END OF PROGRAM\n\n\n",stackptr);
}
 
void main(int nb,char **argv)
{
 fprintf(stderr,"Fast Pascal Interpreter @1996-1998 C.Legendre\n\n");

 if(nb<2)
   {
    fprintf(stderr,"No Object File\n");
    exit(0);
   }

 in=fopen(argv[1],"rb");
 if( !in )
   {
    fprintf(stderr,"No Input File %s\n",argv[1]);
    exit(0);
   }

 fread(&nbident,1,sizeof(int),in);
 fread(&nbquad,1,sizeof(int),in);

 identtab = (InterIdent *)malloc((nbident+1)*sizeof(InterIdent));
 quadtab = (InterSymbol *)malloc((nbquad+1)*sizeof(InterSymbol));

 if( !identtab || !quadtab )
   {
    fprintf(stderr,"Not Enough Memory For Interpreter\n");
    exit(0);
   }

 fread(identtab,nbident,sizeof(InterIdent),in);
 fread(quadtab,nbquad,sizeof(InterSymbol),in);

 GenerateReadablePseudo();

 fprintf(pout,"Now Executing Code....\n\n");
   
 for(stackptrAddr=stackptr=0; stackptr <nbquad ;stackptr++)
 {
/*
  fprintf(stderr,"Oper=%d  Oper1=%d  Oper2=%d   Res=%d\n",quadtab[stackptr].Oper,quadtab[stackptr].Oper1,quadtab[stackptr].Oper2,quadtab[stackptr].Res);
  fprintf(stderr,"StackPTR=%d\n",stackptr);
*/

 switch( quadtab[stackptr].Oper )
       {
        case(WRITELN):  
             fprintf(stderr,"\n");  
             break;

        case(WRITE): 
             if(identtab[quadtab[stackptr].Oper1].opertype == TYPE_REAL )
                fprintf(stderr,"%f ",identtab[quadtab[stackptr].Oper1].fval);  
             else
                fprintf(stderr,"%d ",identtab[quadtab[stackptr].Oper1].eval);
             break;

        case(AFFECTATION): 
             GetValue(&eval,&fval,quadtab[stackptr].Oper1); 
             AssignValue(fval,quadtab[stackptr].Res);
             break;

        case(PLUS):   
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval+fval2,quadtab[stackptr].Res);
             break;

        case(MOINS):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval-fval2,quadtab[stackptr].Res);
             break;

        case(MULTIPLIER):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval*fval2,quadtab[stackptr].Res);
             break;
        case(DIVISER):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             if( eval2 != 0 && fval2 !=0.0 )
                AssignValue(fval/fval2,quadtab[stackptr].Res);
             else
               {
                fprintf(stderr,"Dision By ZERO...\n");
                exit(0);
               }
             break;
        case(MODULO):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             if( eval2 != 0 && fval2 !=0.0 )
                AssignValue(fmod(fval,fval2),quadtab[stackptr].Res);
             else
               {
                fprintf(stderr,"Dision By ZERO (MODULO)...\n");
                exit(0);
               }
             break; 
        case(OR):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval||fval2,quadtab[stackptr].Res);
             break;
        case(AND):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval&&fval2,quadtab[stackptr].Res);
             break;
        case(DIFFERENCE):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval!=fval2,quadtab[stackptr].Res);
             break;
        case(EGALITE):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval==fval2,quadtab[stackptr].Res);
             break;
        case(INFERIEUR):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval<fval2,quadtab[stackptr].Res);
             break;             
        case(SUPERIEUR):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval>fval2,quadtab[stackptr].Res);
             break;
        case(INFERIEUR_EGAL):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval<=fval2,quadtab[stackptr].Res);
             break;              
        case(SUPERIEUR_EGAL):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             GetValue(&eval2,&fval2,quadtab[stackptr].Oper2);
             AssignValue(fval>=fval2,quadtab[stackptr].Res);
             break;
        case(UNARY_PLUS):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             AssignValue(fval,quadtab[stackptr].Res);
             break;
        case(UNARY_MOINS):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             AssignValue(-fval,quadtab[stackptr].Res);
             break;
        case(NOT):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             AssignValue(!fval,quadtab[stackptr].Res);
             break;
        case(JUMP_EQ):   /* test If Oper1=0 */
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             if( fval==0.0) 
                stackptr=quadtab[stackptr].Res-1;
             break;
        case(GOTO):
             stackptr=quadtab[stackptr].Res-1; 
             break;
        case(COS):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             AssignValue(cos(fval),quadtab[stackptr].Res);
             break;
        case(SIN):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             AssignValue(sin(fval),quadtab[stackptr].Res);
             break;
        case(EXP):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             AssignValue(exp(fval),quadtab[stackptr].Res);
             break;
        case(LN):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             if( fval<=0.0 )
               {
                fprintf(stderr,"LN expr <= 0\n");
                exit(0);
               }
             AssignValue(log(fval),quadtab[stackptr].Res);
             break;
        case(SQRT):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             if( eval<0 || fval<0.0 )
               {
                fprintf(stderr,"SQRT expr < 0\n");
                exit(0);
               }
             AssignValue(sqrt(fval),quadtab[stackptr].Res);
             break;
        case(ABS):
             GetValue(&eval,&fval,quadtab[stackptr].Oper1);
             if( eval < 0 )    eval=-eval;
             if( fval < 0.0 )  fval=-fval;
             AssignValue(fval,quadtab[stackptr].Res);    
             break;
        case(POPJUMP):
             if( stackptrAddr<=0 )
               {
                fprintf(stderr,"Stack Underflow\n");
	        exit(0);
               }
             stackptr = StackAddr[--stackptrAddr]-1;
             break;
         case(PUSHADDR):
              if( stackptrAddr >MAXSTACKADDR )
                {
                 fprintf(stderr,"Stack OverFlow\n");
                 exit(0);
                }
               
              StackAddr[stackptrAddr++] = quadtab[stackptr].Res;
              break;
         case(POPARG):
              if( ArgStackPtr <= 0 )
                {
                 fprintf(stderr,"Arg. Stack Underflow\n");
                 exit(0);
                }                
              
              ArgStackPtr--;
              identtab[quadtab[stackptr].Res].eval = ArgStack[ArgStackPtr].eval;
              identtab[quadtab[stackptr].Res].fval = ArgStack[ArgStackPtr].fval;
              if(identtab[quadtab[stackptr].Res].opertype == TYPE_BOOL )
                 AssignValue(identtab[quadtab[stackptr].Res].fval,quadtab[stackptr].Res);
              break;
         case(PUSHARG):
              if( ArgStackPtr >= MAXARGSTACK )
                {
                 fprintf(stderr,"Arg. Stack Overflow\n");
                 exit(0);
                }

              if( identtab[quadtab[stackptr].Res].opertype == TYPE_ENTIER || identtab[quadtab[stackptr].Res].opertype == TYPE_BOOL )  
                {
                 ArgStack[ArgStackPtr].eval = identtab[quadtab[stackptr].Res].eval;
                 ArgStack[ArgStackPtr].fval = ArgStack[ArgStackPtr].eval;
                }
              else
               {
                if( identtab[quadtab[stackptr].Res].opertype == TYPE_REAL )
                  {
                   ArgStack[ArgStackPtr].fval = identtab[quadtab[stackptr].Res].fval;
                   ArgStack[ArgStackPtr].eval = ArgStack[ArgStackPtr].fval;
                  }
               }

              ArgStackPtr++;
              break;
         default:
             fprintf(stderr,"Unknow Pseudo Code Instruction:%d\n",quadtab[stackptr].Oper);
       }
/*
  getch();
*/
 }
 
 GenerateSymTab(); 
 exit(0);
}

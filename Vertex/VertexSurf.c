/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                   Vertex Surface File                */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexOBJ.h"
#include"VertexDisplay.h"
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

/* Usefull Extern And Global Data */
extern XWidget *objlist,*msg;
extern XGUI gui;
extern int nbobj,nbsurf;
extern unsigned long Black,White;
extern SCENE3D global_scene;
extern XWidget *objlist,*msg;

/* Local Data: Used For the Dialog Box */
XShell *surfshell;
XWidget *surfname,*surffct,*sminx,*smaxx,*sminy,*smaxy;
XWidget *sstepx,*sstepy;


/* Local type <-- Parser Function */
typedef struct fct
{
 char *debut;
 int maxlen;
}FONCTION;

char chaine[1000];
FONCTION fct = { chaine,1000};

/* Local Parser Data */
int (*isv)(char *,int);
int lmax;
char *f_ana;
char *ar_form_cur;
char buffer[8];
float (*ar_val_fn)(int);
int ana_cur,i,j,k,errno;
float r;
int res;
float ar_glob_x,ar_glob_y;
char pourcte[30];
static char ar_nom_op_unaire[][8] = 
{
  "-","+",
  "SIN","COS","TAN","ARCSIN","ARCCOS","ARCTAN",
  "SH","CH","TH","ARGSH","ARGCH","ARGTH",
  "ABS","INT","FRAC","TACT","EXP","LOG","SQRT"
};

char vstab[] = { 'A','B','C','D','E','F','G','H','I','J','K',
               'L','M','N','O','P','Q','R','S','T','U'
             };


/*********************/
/* avance_parenthese */
/*********************/

int avance_parenthese(f,len)
char *f;
int *len;
{
 int i=0,np=0;

 do
 {
  switch(*f++)
        {
         case'(': 
                  np++; 
                  break;
         case')': 
                  np--;
                  break;
        }
  i++;
 }while( (np) && (i< *len) );

 f--;

 *len = i;

 return( !np );
}

/*********************/
/*    ins_unaire     */
/*-------------------*/
/*  operateur unaire */
/*       trouv‚      */
/*********************/

int ins_unaire(i)
int i;
{
 if( (ana_cur + 2) >=lmax)
   return 0; 

 f_ana[ana_cur++] = 'U';
 f_ana[ana_cur++] = (char)i;
 return 1;
}


/*********************/
/*    recherchee     */
/*-------------------*/
/*recherche de chaine*/
/*                   */
/*********************/

int analyse_chaine(char *,int);   /* defined below; forward-declared (2026) */

int recherche(ch,len,c1,c2)
char *ch,c1,c2;
int len;
{
 int np,i=len-1;

 do
 {
  if(ch[i] == ')' )
    {
     np = 0;
     do
     { 
      switch(ch[i--])
            {
             case '(': np--; break;
             case ')': np++; break;
            }
     }while( (np) && (i>=0) );
    
     i++;

     if(np)
       {
        res=0;
        return(1); 
       }
    }
  else
     if(((ch[i]==c1) ||(ch[i]==c2)) && (i>0) && i<(len-1))
       switch(ch[i-1])
             {
              case'(': 
              case'E': 
              case'+': 
              case'-': 
              case'*': 
              case'/': 
              case'^': 
               break;
              default:
              if(ana_cur+2 <lmax)
                {
                 f_ana[ana_cur++] = 'B';
                 f_ana[ana_cur++] = ch[i];
                 if(analyse_chaine(&ch[0],i))
                   res = analyse_chaine(&ch[i+1],len-i-1);
                 else
                   res = 0;
                }
                 return 1;
             }
 }while( --i >= 0 );
 return 0;
}

/*********************/
/*  analyse chaine   */
/*-------------------*/
/*                   */
/*********************/

int analyse_chaine(char *ch,int lc)
{
 if(lc == 0) return 1;


 /*************************************/
 /* recherche des operations binaires */
 /*        '-','+','/','*'            */
 /*************************************/

 if(recherche(ch,lc,'-','+')) return res;
 if(recherche(ch,lc,'/','*')) return res;


 /*************************************/ 
 /*  recherche des operations unaires */
 /*          '-','+'                  */
 /*************************************/

 if(ch[0] =='-')
   {
    ins_unaire('A');  /* FMOINS */
    return(analyse_chaine(&ch[1],lc-1));
   }

 if(ch[0] =='+')
    return(analyse_chaine(&ch[1],lc-1));


 /*************************************/
 /*recherche des operations puissances*/
 /*************************************/
 
 if(recherche(ch,lc,'^','^'))
   return res;


 /*************************************/
 /*     recherche des parentheses     */
 /*************************************/

 if(ch[0] == '(') 
   {
    i = lc;
    if( avance_parenthese(ch,&i) && (i==lc) )
      return(analyse_chaine(&ch[1],(lc-2)));
    else 
      return 0;
   }

 /*************************************/
 /*    recherche de la factorielle    */
 /*************************************/

 if(ch[lc-1] == '!')
   {
    ins_unaire('Q');   /* FFACT */
    return(analyse_chaine(&ch[0],lc-1));
   }


 /*************************************/ 
 /*  recherche des fonctions unaires  */
 /*************************************/

 for(k=2;k<21;k++)
    {
     j=0;
     j = strlen(ar_nom_op_unaire[k]);

      if( strncmp(ar_nom_op_unaire[k],ch,j) == 0)      
       {
        res = (float)0.0;
        if(ch[j] == '(')
           if( ins_unaire(vstab[k]) )
              res = analyse_chaine(&ch[j],lc-j);

         return res;
       }
    }

 /*************************************/
 /*      recherche des inconnues      */ 
 /*************************************/
 
 i = (*isv)(ch,lc);

 if(i>=0)
   {
    if(ana_cur+1+sizeof(int) >=lmax)
     return 0;

    f_ana[ana_cur++] = 'V';
    memcpy(&f_ana[ana_cur],(char *)&i,sizeof(int));
    ana_cur += sizeof(int);
    return 1;
   }

 /*************************************/
 /*      recherche des constantes     */ 
 /*************************************/

 if(ana_cur + sizeof(float) +1 >=lmax)
   return 0;

 i=0;

 strncpy(pourcte,ch,lc);
 pourcte[lc] = 0;

 if( strcmp(pourcte,"0") ==0)
   {
    r= (float)0;
    i=1;
   }

 if(i==0)
   if( (r = atof(pourcte)) != (float)0)
     i=1;
   else
     i=0;

 if(i==0)
   if(strcmp(pourcte,"PI") == 0)
     {
      i=1;
      r = (float)3.14159265;
     }

 if(i)
   {
    f_ana[ana_cur++] = 'C';
    memcpy(&f_ana[ana_cur],(char *)&r,sizeof(float));
    ana_cur += sizeof(float);
    return(1);
   }

 return 0;
}


/*********************/
/* analyse fonction  */
/*-------------------*/
/*                   */
/*********************/

int ar_analyse_fct(f,lm,isvar,form_ana,lr)
char *f,*form_ana;
int lm,*lr,(*isvar)(char *,int);
{
 int i,j;
 char *fb;

 isv = isvar;
 lmax = lm;
 f_ana = form_ana;
 ana_cur = 0;

 fb = (char *)malloc(strlen(f));
 if(fb == 0) return(0);

 for(i=j=0;i<strlen(f);i++)
    if(f[i] != ' ')
      fb[j++]=toupper(f[i]);

 i = analyse_chaine(fb,j);
 *lr = ana_cur;
 free(fb);
 return(i);
}

/*********************/
/*  num var          */
/*-------------------*/
/*                   */
/*********************/

int num_var(char *ch,int lc)
{
 if(lc != 1)
   return (-1);
 switch(ch[0])
       { 
        case 'X': return 1;
        case 'T': return 1;
        case 'Y': return 2;
       }
 return -1;
}

int ar_anal_fct(char *cc,FONCTION *fon)
{
 int lr;

 return(ar_analyse_fct(cc,fon->maxlen,num_var,fon->debut,&lr));
}

/*********************/
/*   EVALUATION      */
/*-------------------*/
/*                   */
/*********************/


float ar_evalue()
{
 float ra,s;
 int varia;
 char oper;

 switch(*(ar_form_cur++))
       {
        case 'C':
        memcpy((char *)&ra,ar_form_cur,sizeof(float));
        ar_form_cur += sizeof(float);
        return(ra);

        case 'V':
        memcpy((char *)&varia,ar_form_cur,sizeof(int));
        ra = ar_val_fn(varia);
        if(errno)
          return 0;
        ar_form_cur +=sizeof(int);
        return(ra);

        case 'B':
        oper = *(ar_form_cur++);
        ra = ar_evalue();
        if(errno)
          return 0;
        switch(oper)
              {
               case '+': return ((float)(ra+ar_evalue()));
               case '-': return ((float)(ra+ar_evalue()));
               case '*': return ((float)(ra*ar_evalue()));
               case '/':
                         s=ar_evalue();
                         if(fabs(s) < 1e-20)
                           errno = 1;
                         if(errno)
                           return 0;
                         return((float)(ra/s));
               case '^': s=ar_evalue();
                         if(errno)
                           return 0;
                         return((float)pow(ra,s));
               default:
                         errno = 1;
                         return 0;

               }

        case 'U': 
        oper = *(ar_form_cur++);    
        ra = ar_evalue();
        if(errno)
           return 0;

        switch(oper)
              {
               case 'A': return ((float)-ra);
               case 'B': return ((float)ra);
               case 'C': return ((float)sin(ra));
               case 'D': return ((float)cos(ra));
               case 'E': return ((float)tan(ra));
               case 'F': return ((float)asin(ra));
               case 'G': return ((float)acos(ra));
               case 'H': return ((float)atan(ra));
               case 'I': return ((float)sinh(ra));
               case 'J': return ((float)cosh(ra));
               case 'K': return ((float)tanh(ra));
               case 'L': return ((float)log(ra+sqrt(1+ra*ra)));
               case 'M': return ((float)(log(r+sqrt(-1+ra*ra))));
               case 'N': return ((float)(log((1+ra)/(1-ra))/2));
               case 'O': return ((float)fabs(ra));
               case 'P': return ((float)floor(ra));
               case 'Q': return ((float)(ra-floor(ra)));
               case 'R': if(fabs(ra) < 32)
                         for(s=1,i=1;i<=(int)ra;i++) s*= i;
                         return(s);                 
               case 'S': return ((float)exp(ra));
               case 'T': if(ra > 0.0 )
                            return ((float)log(ra));
                         else
                            return(0.0);
               case 'U': if(ra >0.0 )
                            return ((float)sqrt(ra));
                         else
                            return(0.0);  
               default:
                         errno = 1;
                         return 0;
              }

        case '\0':       return 0;
        default:         errno = 1;
                         return 0;
       }
}

float ar_valeur(int i)
{
 errno = 0;
 switch(i)
       {
        case 1: return ((float)ar_glob_x);
        case 2: return ((float)ar_glob_y);
        default:
                errno =1;
                return ((float)0);
       }
}

float ar_evalfct(FONCTION f,float x,float y)
{
 ar_glob_x = x;
 ar_glob_y = y;
 ar_form_cur = f.debut;
 ar_val_fn = ar_valeur;
 return(ar_evalue()); 
}  


void SurfOK()
{
 OBJ3D *ob;          /* Our New Object Ptr */
 float *tmp,maxx,minx,maxy,miny,sx,sy;
 float X,Y,Z;
 int *ftmp;
 int nbpt,nbptx,nbpty,rnbfc;
 int i,j;
 char *name;
 
 name = GetTextString(surffct);
 if( ar_anal_fct(name,&fct) == 0 )
     return;   /* Parse Error, Try Again */


/* REcover The Data */
 name = GetTextString(sminx);
 minx = VertexATOF(name);
 name=GetTextString(smaxx);
 maxx = VertexATOF(name);
 name=GetTextString(sminy);
 miny = VertexATOF(name);
 name=GetTextString(smaxy);
 maxy = VertexATOF(name);

 name=GetTextString(sstepx);
 sx = VertexATOF(name);
 name=GetTextString(sstepy);
 sy = VertexATOF(name);
 
 name=GetTextString(surfname);
  
 if(minx>maxx || miny>maxy || sx <=0.0 || sy<=0.0 )
   return;   /* Invalid Parameters Values */
   
 nbsurf++;
 
 nbptx = (int)((maxx-minx)/sx);
 nbpty = (int)((maxy-miny)/sy);
 nbpt = nbptx * nbpty;

 /* searching the object pointer in the global object List*/
 if( (ob=FindNextPlace()) == 0 )
    return;

 if( AllocOBJ3D(ob,nbpt,2*nbpt) != 0 )        /* Alloc The Object Memory */
    SetTextString(msg,"Surface Creation: OK");
 else
   {
    SetTextString(msg,"Can Not Allocate Surface Memory");
    return;
   }


 tmp = (float *)ob->PTtable;

 for(j=0,Y=miny ; j<nbpty ; j++,Y+=sy )
     for(i=0,X=minx ; i<nbptx ; i++,X+=sx )
        {
         Z = ar_evalfct(fct,X,Y);
         *tmp++ = X;
         *tmp++ = Y;
         *tmp++ = Z;
        }

 ftmp = (int *)ob->FCtable;

 for(rnbfc=j=0;j<(nbpty-1);j++)
    for(i=0;i<(nbptx-1);i++) 
       {
        *ftmp++ = j*nbptx+i;
        *ftmp++ = (j+1)*nbptx+i;
        *ftmp++ = (j+1)*nbptx+i+1;
 
        *ftmp++ = j*nbptx+i;
        *ftmp++ = j*nbptx+i+1;
        *ftmp++ = (j+1)*nbptx+i+1;
        rnbfc += 2;
       }

 ob->NBFACE = rnbfc;

/* Add THe Object Name in the global object List */
 memset(ob->Name,0,25);
 strncpy(ob->Name,name,25);  /* Put the Name In The Structure */
 AddListItem(objlist,GetTextString(surfname));

/* Redisplay the scene with the new object */
 DestroyShell(&gui,surfshell);
 VertexReDisplay();
}
 
void SurfCancel()
{
 DestroyShell(&gui,surfshell);
}

void BuildSurfaceBox(char *name)
{
 XWidget *entry;
 char buff[25];

 surfshell = CreateShell(&gui,name,0,0,300,200);
 entry = CreateButton(surfshell,"Ok",10,160,70,20);
 AddCallBack(entry,SurfOK);
 entry = CreateButton(surfshell,"Cancel",220,160,70,20);
 AddCallBack(entry,SurfCancel);
 entry = CreateLabel(surfshell,"Surface Name",10,10,Black,FALSE);
 sprintf(buff,"SURF%d",nbsurf);
 surfname = CreateText(surfshell,buff,110,10,130,20);
 entry = CreateLabel(surfshell,"Function",10,40,Black,FALSE);
 surffct = CreateText(surfshell,"100*SIN(x)*COS(y)",110,40,130,20);
 entry = CreateLabel(surfshell,"MinX  MaxX",10,70,Black,FALSE);
 sminx = CreateText(surfshell,"-300.0",110,70,50,20);
 smaxx = CreateText(surfshell,"300.0",170,70,50,20);
 entry = CreateLabel(surfshell,"MinY  MaxY",10,100,Black,FALSE);
 sminy = CreateText(surfshell,"-300.0",110,100,50,20);
 smaxy = CreateText(surfshell,"300.0",170,100,50,20);
 entry = CreateLabel(surfshell,"StepX StepY",10,130,Black,FALSE);
 sstepx = CreateText(surfshell,"60.0",110,130,50,20);
 sstepy = CreateText(surfshell,"60.0",170,130,50,20);

 MapShell(surfshell);
 MapAllWidget(surfshell);
 LoopShellEvent(&gui,surfshell);
}

void VertexCreateSurface()
{
 BuildSurfaceBox("Vertex Surface Creation");
} 

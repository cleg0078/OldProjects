/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                  Vertex Mountain File                */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexMount.h"
#include"time.h"

/* Global Data Used */
extern XGUI gui;
extern XWidget *objlist,*msg;
extern int nbmount,nbobj;
extern unsigned long Black;

/* And now the local data */
XShell *mountshell;
XWidget *mountres,*mountname,*mminx,*stepx,*mminz,*stepz;

int TAILLE;
int frac[129][129];
float rug;

/********************************/
/*       RecoverOperData        */
/*------------------------------*/
/*  See The Name To understand  */
/*------------------------------*/
/* Parameter:                   */
/* IN:   ptr on the data        */
/* OUT:  NONE                   */
/*------------------------------*/
/* Comments:                    */  
/* this Funstion recovers Data  */ 
/*  From the widget dialog box  */
/********************************/

void MRecoverData(int *r,float *mx,float *sx,float *mz,float *sz)
{
 *r = atoi(GetTextString(mountres));
 *mx = VertexATOF(GetTextString(mminx));
 *sx = VertexATOF(GetTextString(stepx));
 *mz = VertexATOF(GetTextString(mminz));
 *sz = VertexATOF(GetTextString(stepz));
}

int aleaentier(int min,int max)
  {
  return((int)(min+(double)rand()*(max-min)/RAND_MAX));
  }

void initfrac(int ideb,int ifin,int jdeb,int jfin)
  {
  int dim,z1,z2,z3,z4,imil,jmil;
  dim=(int)(rug/TAILLE*(abs(ifin-ideb)));
  z1=frac[ideb][jdeb];
  z2=frac[ifin][jdeb];
  z3=frac[ideb][jfin];
  z4=frac[ifin][jfin];
  imil=(ideb+ifin)/2;
  jmil=(jdeb+jfin)/2;
  frac[imil][jdeb]=(z1+z2)/2+aleaentier(0,dim)-dim/2;
  frac[ideb][jmil]=(z1+z3)/2+aleaentier(0,dim)-dim/2;
  frac[ifin][jmil]=(z2+z4)/2+aleaentier(0,dim)-dim/2;
  frac[imil][jfin]=(z3+z4)/2+aleaentier(0,dim)-dim/2;
  frac[imil][jmil]=(z1+z2+z3+z4)/4+2*aleaentier(0,dim)-dim;
  }

void MountOK()
{
 OBJ3D *ob;          /* Our New Object Ptr */
 int i,j,n,nbr,ind,tailletmp,res;
 float X,Z,mz,sz,mx,sx,*ptmp;
 int *ftmp;

 MRecoverData(&res,&mx,&sx,&mz,&sz);

 if(res<1 || res>6)
   {
    SetTextString(msg,"Invalid Mountain Parameter");
    return;
   }

 TAILLE=1<<res;

 srand(time(0));  /* Init The Alea Number */ 

 for(i=0;i<=TAILLE;i++)
    for(j=0;j<=TAILLE;j++)
       frac[i][j]=0;

 rug=aleaentier(300,600);
 frac[0][TAILLE]=aleaentier(300,500);
/* frac[0][0]=-aleaentier(0,300);
 frac[TAILLE][TAILLE]=-aleaentier(0,300);*/
 
 for(nbr=0,tailletmp=TAILLE;(tailletmp%2)==0;tailletmp/=2)
     nbr++;
     
 for(n=1;n<=nbr;n++)
    {
     ind=TAILLE>>(n-1);
     for(i=0;i<TAILLE;i+=ind)
       for(j=0;j<TAILLE;j+=ind)
           initfrac(i,i+ind,j,j+ind);	   
     }
      
 for(i=0;i<=TAILLE;i++)
    for(j=0;j<=TAILLE;j++)
       if(frac[i][j]<0)
          frac[i][j]=0;

 frac[0][0]=0;
 frac[TAILLE][TAILLE]=0;
 
 /* searching the object pointer in the global object List*/
 ob = FindNextPlace();             /* Our Object Address */
 if(ob == NULL)
    return;

 if( AllocOBJ3D(ob,(TAILLE+1)*(TAILLE+1),2*TAILLE*TAILLE) != 0 )
   SetTextString(msg,"Mountain Creation: OK");
 else
  {
   SetTextString(msg,"Mountain Memory Allocation Error");
   return;
  }
 
  ptmp = (float *)ob->PTtable;
  
  for(Z=mz,i=0;i<=TAILLE;i++,Z+=sz)
     for(X=mx,j=0;j<=TAILLE;j++,X+=sx)
	 {
	  *ptmp++ = X;
	  *ptmp++ = (float)frac[i][j];
	  *ptmp++ = Z;
         } 
  
  ftmp =(int *)ob->FCtable;

  for(i=0;i<TAILLE;i++)
     for(j=0;j<TAILLE;j++)
	{
	 *ftmp++ = i*(TAILLE+1) + j;
	 *ftmp++ = (i+1)*(TAILLE+1) + j;
         *ftmp++ = (i+1)*(TAILLE+1) + j + 1;

         *ftmp++ = i*(TAILLE+1) + j + 1;
         *ftmp++ = i*(TAILLE+1) + j;
         *ftmp++ = (i+1)*(TAILLE+1) + j + 1;
        }

 strcpy(ob->Name,GetTextString(mountname));
 AddListItem(objlist,ob->Name);
 DestroyShell(&gui,mountshell);
 VertexReDisplay(); /* Redisplay The Scene */
}


/********************************/
/*          BuildShell          */
/*------------------------------*/
/*  Build The Dialog Box Shell  */
/*------------------------------*/
/* Parameter:                   */
/* IN:   NONE                   */
/* OUT:  NONE                   */
/*------------------------------*/
/*Comments: NO                  */
/********************************/

void MDialogCANCEL()
{
 DestroyShell(&gui,mountshell);
}

void BuildShell()
{
 XWidget *entry;
 char buff[30];
 
 mountshell = CreateShell(&gui,"Vertex Mountain Creation",0,0,300,170);
 entry = CreateButton(mountshell,"Ok",10,140,70,20);
 AddCallBack(entry,MountOK);
 entry = CreateButton(mountshell,"Cancel",220,140,70,20);
 AddCallBack(entry,MDialogCANCEL);

 entry = CreateLabel(mountshell,"Mountain Name",10,10,Black,FALSE);
 sprintf(buff,"MOUNT%d",nbmount);
 mountname= CreateText(mountshell,buff,110,10,100,20);                          
 entry = CreateLabel(mountshell,"Resolution",10,40,Black,FALSE);
 mountres= CreateText(mountshell,"4",110,40,50,20);     
 entry = CreateLabel(mountshell,"Min X , Min Z",10,70,Black,FALSE);
 mminx = CreateText(mountshell,"-600.0",110,70,50,20);
 mminz = CreateText(mountshell,"500.0",190,70,50,20);
 entry = CreateLabel(mountshell,"Step X,Step Z",10,100,Black,FALSE);
 stepx = CreateText(mountshell,"100.0",110,100,50,20);
 stepz = CreateText(mountshell,"100.0",190,100,50,20);
 
 MapShell(mountshell);
 MapAllWidget(mountshell);
 LoopShellEvent(&gui,mountshell);
}


void VertexCreateMountain()
{
 BuildShell();
}

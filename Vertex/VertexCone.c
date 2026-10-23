/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                     Vertex Cone File                 */
/*                  NO COPYRIGHT May 1995               */ 
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/

#include"VertexOBJ.h"
#include"VertexDisplay.h"
#include"stdio.h"

/* Extern Usefull Global Data */ 
extern XWidget *objlist,*msg;
extern int nbobj,nbcone;
extern XGUI gui;
extern unsigned long Black,White;

/* Local Data: Used For the Dialog Box */
XShell *coshell;
XWidget *codiam,*cores,*coname,*coheight;
XWidget *cocx,*cocy,*cocz;


/************************************/
/*          CCRecoverData           */
/*----------------------------------*/
/* Recover The Input Data From The  */
/*           Dialog Box             */
/*----------------------------------*/
/* Parameters:                      */
/* IN: res -> Resolution of the cone*/
/*     cx --> X Center of Object    */
/*     cy --> Y Center Of Object    */
/*     cz --> Z Center Of Object    */
/*   diam --> Diameter Of The Cone  */
/*      h --> Height Of The Cone    */
/*      n --> Name Of The Cone      */
/* OUT: NONE                        */
/*----------------------------------*/
/* Comments:                        */
/* Only XTOOLKIT Functions Are Used */
/************************************/

void CCRecoverData(int *res,float *cx,float *cy,float *cz,float *diam,float *h)
{
 *res = (int)atoi(GetTextString(cores));
 *res = (*res<2)?2:*res;

 *cx = VertexATOF(GetTextString(cocx));
 *cy = VertexATOF(GetTextString(cocy));
 *cz = VertexATOF(GetTextString(cocz));

 *diam = VertexATOF(GetTextString(codiam));
 *h = VertexATOF(GetTextString(coheight));
}


/************************************/
/*           BuildConeOBJ           */
/*----------------------------------*/
/*         Build The Object         */
/*----------------------------------*/
/* Parameters:                      */
/* IN: NONE                         */
/* OUT:NONE                         */
/*----------------------------------*/
/* Comments:                        */
/************************************/
  
void BuildConeOBJ()
{
 OBJ3D *ob;          /* Our New Object Ptr */
 float diam,*tmp,pas,height;
 float cX,cY,cZ,teta,rayon,h;
 int j;
 int *ftmp;
 int res;

 /* New Cone So Add One Please... */
 nbcone++;
 
 /* Recover The data */
 CCRecoverData(&res,&cX,&cY,&cZ,&diam,&height);

 /* searching the object pointer in the global object List*/
 if( (ob = FindNextPlace()) == 0 )
    return;

 if( AllocOBJ3D(ob,res+1,res) != 0)    /* Alloc The Object Structure */
   SetTextString(msg,"Cone Creation: OK");
 else
  { 
   SetTextString(msg,"Can Not Allocate Cone Memory");
   return;
  }

 tmp = (float *)ob->PTtable;

 rayon = diam/2.0; 

 ob->Center.X = cX;
 ob->Center.Y = cY;
 ob->Center.Z = cZ;

 /* Build The Top Point Of The Cone */
 *tmp++ = cX;
 *tmp++ = cY + height/2.0;
 *tmp++ = cZ;

 pas=2.0*PI/(float)res;
 h=-height/2.0;

 /* Now Build All Other Points */
 for(teta=0.0,j=0; j<res ; j++,teta+=pas)
    {
     *tmp++ = cX + rayon*cos(teta);
     *tmp++ = h + cY ;
     *tmp++ = cZ +rayon*sin(teta);
    }


 /* Build Faces Table */
 ftmp = (int *)ob->FCtable;

 for(j=1;j<=res;j++)
    {
     *ftmp++ = j;
     *ftmp++ = 0;
     *ftmp++ = j%res+1;
    }

 strcpy(ob->Name,GetTextString(coname));
 AddListItem(objlist,GetTextString(coname));
 DestroyShell(&gui,coshell);
 VertexReDisplay(); /* Redisplay The Scene */
}


/* No Comment: We Won't the cone */
void ConeCancel()
{
 DestroyShell(&gui,coshell);
}


/************************************/
/*           BuildConerBox          */
/*----------------------------------*/
/*        Build The Dialog Box      */
/*----------------------------------*/
/* Parameters:                      */
/* IN: name --> Name Of The Shell   */
/* OUT: NONE                        */
/*----------------------------------*/
/* Comments:                        */
/* The New Shell Is Global.         */
/* For The XT Functions: See XMAN   */
/************************************/

void BuildConeBox(char *name)
{
 XWidget *entry;
 char buff[50];

 coshell = CreateShell(&gui,name,0,0,300,200);
 entry = CreateButton(coshell,"Ok",10,160,70,20);
 AddCallBack(entry,BuildConeOBJ);
 entry = CreateButton(coshell,"Cancel",220,160,70,20);
 AddCallBack(entry,ConeCancel);
 entry = CreateLabel(coshell,"Cylinder Name",10,10,Black,FALSE);

 sprintf(buff,"CONE%d",nbcone);
 coname  = CreateText(coshell,buff,110,10,100,20);
 entry = CreateLabel(coshell,"Center X Y Z",10,40,Black,FALSE);
 cocx = CreateText(coshell,"0.0",110,40,50,20);
 cocy = CreateText(coshell,"0.0",170,40,50,20);
 cocz = CreateText(coshell,"0.0",230,40,50,20);

 entry = CreateLabel(coshell,"Diameter",10,70,Black,FALSE);                           
 codiam = CreateText(coshell,"400.0",110,70,50,20);
 entry = CreateLabel(coshell,"Height",10,100,Black,FALSE); 
 coheight = CreateText(coshell,"400.0",110,100,50,20);                         
 entry = CreateLabel(coshell,"Resolution",10,130,Black,FALSE);
 cores = CreateText(coshell,"15",110,130,50,20);


 /* And Now Map the Window ... */
 MapShell(coshell);
 MapAllWidget(coshell);
 LoopShellEvent(&gui,coshell);
}

/************************************/
/*          VertexCreateCone        */
/*----------------------------------*/
/*  Global Cone Creation Function   */
/*----------------------------------*/
/* Parameters:                      */
/* IN: NONE                         */
/* OUT:NONE                         */
/*----------------------------------*/
/* Comments:                        */
/*  This Function Was Called From   */
/*     One VertexGUI CallBack       */
/************************************/

void VertexCreateCone()
{
 BuildConeBox("Vertex Create Cone");
}

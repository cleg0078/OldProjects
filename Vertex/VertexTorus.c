/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                    Vertex Torus File                 */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexOBJ.h"
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

/* Extern Usefull Global Data */
extern XGUI gui;
extern XWidget *objlist,*msg;
extern PT3D tab[300];
extern int nbobj,nbtorus;
extern unsigned long Black;

/* Local Data: Used For the Dialog Box */
XShell *tshell;
XWidget *tname,*tmind,*tmaxd,*tcres,*ttres;
XWidget *tcx,*tcy,*tcz;

void TorusOK()
{
 OBJ3D *ob;          /* Our New Object Ptr */
 float *ptmp,maxd,mind;
 float x,y,z,cx,cy,cz,angle,pas;
 int *ftmp,cres,tres;
 int i,j;
 
 cx = VertexATOF(GetTextString(tcx));
 cy = VertexATOF(GetTextString(tcy));
 cz = VertexATOF(GetTextString(tcz));
 mind = VertexATOF(GetTextString(tmind));
 maxd = VertexATOF(GetTextString(tmaxd));
 cres = atoi(GetTextString(tcres));
 tres = atoi(GetTextString(ttres));
 
 if(mind>maxd || mind<=0.0 || maxd<=0.0 )
   return;   /* Invalid Parameters Values */
   
 nbtorus++;
 
 /* searching the object pointer in the global object List*/
 if( (ob = FindNextPlace()) == 0 )
   return;

 if( AllocOBJ3D(ob,cres*tres,2*tres*cres) == -1)
   SetTextString(msg,"Unable To Allocate Torus Memory");
 else
    SetTextString(msg,"Torus Creation: OK");

 
 /* Generate The Revolution points */
 pas = 2*3.14159265/(float)cres;
 for(angle=0.0,i=0;i<cres;i++,angle+=pas)
    {
      tab[i].X = cx + mind/2.0 + (maxd-mind)/2.0*(cos(angle));
      tab[i].Y = cy + (maxd-mind)/2.0*sin(angle);  
      tab[i].Z = 0.0;
    }

 pas = 2*3.14159265/(float)tres;
 ptmp = (float *)ob->PTtable;
 for(i=0;i<cres;i++)
    for(j=0,angle=0.0;j<tres;angle+=pas,j++)
       {
        x = tab[i].X;
        y = tab[i].Y;
        z = 0.0;
        *ptmp++ = x*cos(angle);
        *ptmp++ = y;
        *ptmp++ = -x*sin(angle);
       }

 ftmp = (int *)ob->FCtable;
 for(i=0; i<cres ;i++)
    for(j=0; j<tres ;j++)
       {
         if(j < (tres-1))
           {
            *ftmp++ = ((i+1)%cres)*tres+j;
            *ftmp++ = i*tres+j;
            *ftmp++ = i*tres+j+1;

            *ftmp++ = i*tres+j+1;
            *ftmp++ = ((i+1)%cres)*tres+j+1;
            *ftmp++ = ((i+1)%cres)*tres+j;
           }
         else
           {
            *ftmp++ = ((i+1)%cres)*tres+j;
            *ftmp++ = i*tres+j;
            *ftmp++ = i*tres;
  
            *ftmp++ = i*tres;
            *ftmp++ = ((i+1)%cres)*tres;
            *ftmp++ = ((i+1)%cres)*tres+j;
           }
       }

 memset(ob->Name,0,25);
 strcpy(ob->Name,GetTextString(tname));
 AddListItem(objlist,GetTextString(tname));
 DestroyShell(&gui,tshell);
 VertexReDisplay(); /* Redisplay The Scene */
}
 
void TorusCancel()
{
 DestroyShell(&gui,tshell);
}

void BuildTorusBox(char *name)
{
 XWidget *entry;
 char buff[25];

 tshell = CreateShell(&gui,name,0,0,300,200);
 entry = CreateButton(tshell,"Ok",10,160,70,20);
 AddCallBack(entry,TorusOK);
 entry = CreateButton(tshell,"Cancel",220,160,70,20);
 AddCallBack(entry,TorusCancel);
 entry = CreateLabel(tshell,"Torus Name",10,10,0,4);
 sprintf(buff,"TORUS%d",nbtorus);
 tname  = CreateText(tshell,buff,110,10,130,20);
 entry = CreateLabel(tshell,"Center X Y Z",10,40,Black,4);
 tcx = CreateText(tshell,"0.0",110,40,50,20);
 tcy = CreateText(tshell,"0.0",170,40,50,20);
 tcz = CreateText(tshell,"0.0",230,40,50,20);
 entry = CreateLabel(tshell,"Mini Max Diam.",10,70,Black,4);
 tmind = CreateText(tshell,"400",110,70,50,20);
 tmaxd = CreateText(tshell,"600",170,70,50,20); 
 entry = CreateLabel(tshell,"Circle Res.",10,100,Black,4);
 tcres = CreateText(tshell,"15",110,100,50,20);
 entry = CreateLabel(tshell,"Torus Res",10,130,Black,4);
 ttres = CreateText(tshell,"15",110,130,50,20);

 /* And Now Map the Window ... */
 MapShell(tshell);
 MapAllWidget(tshell);
 LoopShellEvent(&gui,tshell);
}

void VertexCreateTorus()
{
 BuildTorusBox("Vertex Torus Creation");
} 

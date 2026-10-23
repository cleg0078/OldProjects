/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                   Vertex Cylinder File               */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/

#include"VertexOBJ.h"
#include"VertexDisplay.h"
#include"stdio.h"

extern XWidget *objlist,*msg;
extern XGUI gui;
extern int nbobj,nbcyl;
extern unsigned long Black,White;

XShell *cylshell;
XWidget *cyldiam,*cylres,*cylname,*cylheight;
XWidget *cylcx,*cylcy,*cylcz;

void CRecoverData(int *res,float *cx,float *cy,float *cz,float *diam,float *h)
{
 *res = (int)atoi(GetTextString(cylres));
 *res = (*res<2)?2:*res;

 *cx = VertexATOF(GetTextString(cylcx));
 *cy = VertexATOF(GetTextString(cylcy));
 *cz = VertexATOF(GetTextString(cylcz));

 *diam = VertexATOF(GetTextString(cyldiam));
 *h = VertexATOF(GetTextString(cylheight));
}
  
void BuildCylOBJ()
{
 OBJ3D *ob;
 float diam,*tmp,pas,height;
 float cX,cY,cZ,teta,rayon,h;
 int i,j;
 int *ftmp;
 int res;

 /* New Cylinder */
 nbcyl++;
 
 /* Recover The data */

 CRecoverData(&res,&cX,&cY,&cZ,&diam,&height);

 /* searching the object pointer in the global object List*/
 if( (ob = FindNextPlace()) == 0 )
   return;

 if( AllocOBJ3D(ob,2*res,2*res) != 0)
   SetTextString(msg,"Create Cylinder: OK");
 else
  {
   SetTextString(msg,"Can Not Allocate Cylinder Memory!");
   return;
  }
 
 tmp = (float *)ob->PTtable;

 rayon = diam/2.0; 

 ob->Center.X = cX;
 ob->Center.Y = cY;
 ob->Center.Z = cZ;

 for(pas=2*PI/(float)res,i=0,h=-height/2.0; i<2 ; i++ ,h=height/2)
     for(teta=0.0,j=0; j<res ; j++,teta+=pas)
        {
          *tmp++ = cX + rayon*cos(teta);
          *tmp++ = h + cY ;
          *tmp++ = cZ +rayon*sin(teta);
        }

 ftmp = (int *)ob->FCtable;

 for(j=0;j<res;j++)
    {
     *ftmp++ = j+res;
     *ftmp++ = (j+1)%res;
     *ftmp++ = j;

     *ftmp++ = (j+res+1)%(res)+res;
     *ftmp++ = (j+1)%res;
     *ftmp++ = j+res;
    }

 strcpy(ob->Name,GetTextString(cylname));
 AddListItem(objlist,ob->Name);
 DestroyShell(&gui,cylshell);
 VertexReDisplay();
}

void CylCancel()
{
 DestroyShell(&gui,cylshell);
}

void BuildCylBox(char *name)
{
 XWidget *entry;
 char buff[50];

 cylshell = CreateShell(&gui,name,0,0,300,200);
 entry = CreateButton(cylshell,"Ok",10,160,70,20);
 AddCallBack(entry,BuildCylOBJ);
 entry = CreateButton(cylshell,"Cancel",220,160,70,20);
 AddCallBack(entry,CylCancel);
 entry = CreateLabel(cylshell,"Cylinder Name",10,10,Black,FALSE);

 sprintf(buff,"CYL%d",nbcyl);
 cylname  = CreateText(cylshell,buff,110,10,100,20);
 entry = CreateLabel(cylshell,"Center X Y Z",10,40,Black,FALSE);
 cylcx = CreateText(cylshell,"0.0",110,40,50,20);
 cylcy = CreateText(cylshell,"0.0",170,40,50,20);
 cylcz = CreateText(cylshell,"0.0",230,40,50,20);
 entry = CreateLabel(cylshell,"Diameter",10,70,Black,FALSE);
 cyldiam = CreateText(cylshell,"400.0",110,70,50,20);
 entry = CreateLabel(cylshell,"Height",10,100,Black,FALSE);
 cylheight = CreateText(cylshell,"400.0",110,100,50,20);
 entry = CreateLabel(cylshell,"Resolution",10,130,Black,FALSE);
 cylres = CreateText(cylshell,"15",110,130,50,20);

 /* And Now Map the Window ... */
 MapShell(cylshell);
 MapAllWidget(cylshell);
 LoopShellEvent(&gui,cylshell);
}


void VertexCreateCylinder()
{
 BuildCylBox("Vertex Create Cylinder");
}


/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                   Vertex Pyramid File                */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexOBJ.h"
#include"VertexDisplay.h"

extern XGUI gui;
extern XWidget *objlist,*msg;
extern int nbobj,nbpyra;
extern unsigned long Black;

XShell *pyrashell;
XWidget *pyradiam,*pyraheight,*pyraname;
XWidget *pyracx,*pyracy,*pyracz;

void PyraOK()
{
 OBJ3D *ob;
 float diameter,*ptmp;
 int cx,cy,cz;
 int *ftmp;
 float halfside;
 float halfheight;
 float height;

 /* Another Pyramid To the List!! */
 nbpyra++;

 /* Recover The data */

 height = VertexATOF(GetTextString(pyraheight));
 halfheight = height/2;
 
 cx = VertexATOF(GetTextString(pyracx));
 cy = VertexATOF(GetTextString(pyracy));
 cz = VertexATOF(GetTextString(pyracz));

 diameter = VertexATOF(GetTextString(pyradiam));
 halfside = diameter/2;

 /* searching the object pointer in the global object List*/
 if( (ob = FindNextPlace()) == 0)
   return;

 if( AllocOBJ3D(ob,5,6) != 0 )
    SetTextString(msg,"Pyramid Creation: OK");
 else
   {
    SetTextString(msg,"Pyramid Memory Allocation Error");
    return;
   }

 ptmp = (float *)ob->PTtable;
 ftmp = (int *)ob->FCtable;

 ob->Center.X=cx;
 ob->Center.Y=cy;
 ob->Center.Z=cz;

 *ptmp++ = cx;            *ptmp++ = cy + halfheight; *ptmp++ = cz;
 *ptmp++ = cx - halfside; *ptmp++ = cy - halfheight; *ptmp++ = cz + halfside;
 *ptmp++ = cx + halfside; *ptmp++ = cy - halfheight; *ptmp++ = cz + halfside;
 *ptmp++ = cx + halfside; *ptmp++ = cy - halfheight; *ptmp++ = cz - halfside;
 *ptmp++ = cx - halfside; *ptmp++ = cy - halfheight; *ptmp++ = cz - halfside;

 *ftmp++ = 2;   *ftmp++ = 0;   *ftmp++ = 1;
 *ftmp++ = 1;   *ftmp++ = 0;   *ftmp++ = 4;
 *ftmp++ = 4;   *ftmp++ = 0;   *ftmp++ = 3;
 *ftmp++ = 3;   *ftmp++ = 0;   *ftmp++ = 2;
 *ftmp++ = 2;   *ftmp++ = 1;   *ftmp++ = 4;
 *ftmp++ = 3;   *ftmp++ = 2;   *ftmp++ = 4;
 
 strcpy(ob->Name,GetTextString(pyraname));
 AddListItem(objlist,ob->Name);
 DestroyShell(&gui,pyrashell);
 VertexReDisplay();
} 

void PyraCANCEL()
{
 DestroyShell(&gui,pyrashell);
}

void VertexCreatePyramid()
{
 XWidget *entry;
 char buff[50];

 pyrashell = CreateShell(&gui,"Vertex Create Pyramid",0,0,300,200);
 entry = CreateButton(pyrashell,"Ok",10,160,70,20);
 AddCallBack(entry,PyraOK);
 entry= CreateButton(pyrashell,"Cancel",220,160,70,20);
 AddCallBack(entry,PyraCANCEL);
 entry = CreateLabel(pyrashell,"Pyramid Name",10,10,0,4);
 sprintf(buff,"PYRA%d",nbpyra);
 pyraname  = CreateText(pyrashell,buff,110,10,100,20);
 entry = CreateLabel(pyrashell,"Center X Y Z",10,40,Black,FALSE);
 pyracx = CreateText(pyrashell,"0.0",110,40,50,20);
 pyracy = CreateText(pyrashell,"0.0",170,40,50,20);
 pyracz = CreateText(pyrashell,"0.0",230,40,50,20);
 entry = CreateLabel(pyrashell,"Base Size",10,70,Black,FALSE);                         
 pyradiam = CreateText(pyrashell,"400.0",110,70,50,20);
 entry = CreateLabel(pyrashell,"Height",10,100,Black,FALSE); 
 pyraheight = CreateText(pyrashell,"400.0",110,100,50,20);

 /* And Now Map the Window ... */
 MapShell(pyrashell);
 MapAllWidget(pyrashell);
 LoopShellEvent(&gui,pyrashell);
}
 

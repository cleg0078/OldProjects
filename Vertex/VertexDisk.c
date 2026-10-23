/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                     Vertex Disk File                 */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/


#include"VertexOBJ.h"
#include"VertexDisplay.h"
#include"stdio.h"

extern XGUI gui;
extern XWidget *objlist,*msg;
extern int nbobj,nbdisk;
extern unsigned long Black;

XShell *diskshell;
XWidget *diskdiam,*diskres,*diskname;
XWidget *diskcx,*diskcy,*diskcz;
static char buff[25];

void DiskOK()
{
 OBJ3D *ob;
 float diameter,*tmp,angle,pas;
 float centerX,centerY,centerZ;
 int i;
 int *ftmp;
 int res;

 /* Another Disk To the List!! */
 nbdisk++;

 /* Recover The data */

 res = (int)atoi(GetTextString(diskres));
 res = (res<2)?2:res;   

 centerX  = (float)VertexATOF(GetTextString(diskcx));
 centerY  = (float)VertexATOF(GetTextString(diskcy));
 centerZ  = (float)VertexATOF(GetTextString(diskcz));
 diameter = (float)VertexATOF(GetTextString(diskdiam));

 /* searching the object pointer in the global object List*/

 if( (ob = FindNextPlace()) == 0 )
    return;

 if( AllocOBJ3D(ob,res+1,res) != 0)
    SetTextString(msg,"Disk Creation: OK");
 else 
   {
    SetTextString(msg,"Can Not Allocate Disk Memory!"); 
    return;
   }
 
 pas  = 2*PI/(float)res;
 tmp = (float *)ob->PTtable;


 ob->Center.X = centerX;
 ob->Center.Y = centerY;
 ob->Center.Z = centerZ;
 
 /* init The Object Name */
 memset(ob->Name,0,25);
 strcpy(ob->Name,GetTextString(diskname));

 *tmp++ = centerX;
 *tmp++ = centerY; 
 *tmp++ = centerZ;

 diameter /= 2.0;

 for(i=0,angle=0.0;i<res;i++,angle+=pas)
    {
     *tmp++ = centerX + diameter*cos(angle);
     *tmp++ = centerY + diameter*sin(angle);
     *tmp++ = centerZ;
    }

 ftmp = (int *)ob->FCtable;
 for(i=0;i<res;i++)
    {
     *ftmp++ = i+2;
     *ftmp++ = i+1;
     *ftmp++ = 0;
    }

 *(--ftmp) = 1;

 AddListItem(objlist,GetTextString(diskname));
 DestroyShell(&gui,diskshell);
 VertexReDisplay();
}

void DiskCANCEL()
{
 DestroyShell(&gui,diskshell);
}

void VertexCreateDisk()
{
 XWidget *entry;

 diskshell = CreateShell(&gui,"Vertex Create Disk",0,0,300,200);
 entry = CreateButton(diskshell,"OK",10,160,70,20);
 AddCallBack(entry,DiskOK);
 entry = CreateButton(diskshell,"CANCEL",220,160,70,20);
 AddCallBack(entry,DiskCANCEL);

 entry = CreateLabel(diskshell,"Disk Name",10,10,Black,FALSE);
 sprintf(buff,"DSK%d",nbdisk);
 diskname  = CreateText(diskshell,buff,110,10,50,20);
 entry = CreateLabel(diskshell,"Center X Y Z",10,40,Black,FALSE);
 diskcx = CreateText(diskshell,"0.0",110,40,50,20);
 diskcy = CreateText(diskshell,"0.0",170,40,50,20);
 diskcz = CreateText(diskshell,"0.0",230,40,50,20);

 entry = CreateLabel(diskshell,"Diameter",10,70,Black,FALSE);
 diskdiam = CreateText(diskshell,"400",110,70,50,20);
 entry = CreateLabel(diskshell,"Resolution",10,100,Black,FALSE);
 diskres = CreateText(diskshell,"15",110,100,50,20);
 /* And Now Map the Window ... */
 MapShell(diskshell); 
 MapAllWidget(diskshell);
 LoopShellEvent(&gui,diskshell);
}
 

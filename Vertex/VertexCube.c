/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                     Vertex Cube File                 */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/

#include"VertexOBJ.h"
#include"VertexDisplay.h"
#include"stdio.h"

/* Usefull Extern And Global Data */
extern XWidget *objlist,*msg;
extern XGUI gui;
extern int nbobj,nbcube;
extern unsigned long Black,White;

/* And Now The Local Data: Used For The Dialog Box */
XShell *cubeshell;
XWidget *cubediam,*cubename;
XWidget *cubecx,*cubecy,*cubecz;


/************************************/
/*              CubeOK              */
/*----------------------------------*/
/*          Build The Cube          */
/*----------------------------------*/
/* Parameters:                      */
/* IN: NONE                         */
/* OUT: NONE                        */
/*----------------------------------*/
/* Comments:                        */
/* Called By one Callback           */
/************************************/

void CubeOK()
{
 float halfside,cx,cy,cz;
 OBJ3D *ob;
 float *ptmp;
 float diameter;
 int *ftmp;
 char *buff;

 /* Another Cube To the List!! */
 nbcube++;

 /* Recover The data */

 cx = VertexATOF(GetTextString(cubecx));
 cy = VertexATOF(GetTextString(cubecy));
 cz = VertexATOF(GetTextString(cubecz));
 diameter = VertexATOF(GetTextString(cubediam));
 halfside = diameter/2;

 buff = GetTextString(cubename);
 
 if( (ob=FindNextPlace()) == 0 )
    return;

 if( AllocOBJ3D(ob,8,12) != 0 )        /* Alloc The Object Memory */
    SetTextString(msg,"Cube Creation: OK");
 else
   {
    SetTextString(msg,"Can Not Allocate Cube Memory");
    return;
   }

 /* Place The Center Of the Object */
 ob->Center.X=cx;
 ob->Center.Y=cy;
 ob->Center.Z=cz;

 /* Tempory Pointers for a faster Function */
 ptmp = (float *)ob->PTtable;
 ftmp = (int *)ob->FCtable;

/* Init the point coefficients in order X,Y,Z */
 *ptmp++ = cx-halfside;  *ptmp++ = cy+halfside;  *ptmp++ = cz+halfside;
 *ptmp++ = cx+halfside;  *ptmp++ = cy+halfside;  *ptmp++ = cz+halfside;
 *ptmp++ = cx+halfside;  *ptmp++ = cy-halfside;  *ptmp++ = cz+halfside;
 *ptmp++ = cx-halfside;  *ptmp++ = cy-halfside;  *ptmp++ = cz+halfside;
 *ptmp++ = cx-halfside;  *ptmp++ = cy+halfside;  *ptmp++ = cz-halfside;
 *ptmp++ = cx+halfside;  *ptmp++ = cy+halfside;  *ptmp++ = cz-halfside;
 *ptmp++ = cx+halfside;  *ptmp++ = cy-halfside;  *ptmp++ = cz-halfside;
 *ptmp++ = cx-halfside;  *ptmp++ = cy-halfside;  *ptmp++ = cz-halfside;

/*And now the face: I1,I2,I3 (see the VertexStruct.h) */
 *ftmp++=1;  *ftmp++=0;  *ftmp++=3;
 *ftmp++=2;  *ftmp++=1;  *ftmp++=3;
 *ftmp++=4;  *ftmp++=5;  *ftmp++=7;
 *ftmp++=7;  *ftmp++=5;  *ftmp++=6;
 *ftmp++=7;  *ftmp++=0;  *ftmp++=4;
 *ftmp++=3;  *ftmp++=0;  *ftmp++=7;
 *ftmp++=5;  *ftmp++=1;  *ftmp++=2;
 *ftmp++=6;  *ftmp++=5;  *ftmp++=2;
 *ftmp++=0;  *ftmp++=5;  *ftmp++=4;
 *ftmp++=0;  *ftmp++=1;  *ftmp++=5;
 *ftmp++=7;  *ftmp++=2;  *ftmp++=3;
 *ftmp++=2;  *ftmp++=7;  *ftmp++=6;

/* Add THe Object Name in the global object List */
 memset(ob->Name,0,25);
 strncpy(ob->Name,buff,25);  /* Put the Name In The Structure */
 AddListItem(objlist,GetTextString(cubename));

/* Redisplay the scene with the new object */
 DestroyShell(&gui,cubeshell);
 VertexReDisplay();
} 

/* We Want not a new cube */
void CubeCANCEL()
{
 DestroyShell(&gui,cubeshell);
}

/************************************/
/*          VertexCreateCube        */
/*----------------------------------*/
/*       Build The Dialog Shell     */
/*----------------------------------*/
/* Parameters:                      */
/* IN: NONE                         */
/* OUT: NONE                        */
/*----------------------------------*/
/* Comments:                        */
/*Called From One VertexGUI Callback*/
/************************************/

void VertexCreateCube()
{
 XWidget *entry;
 char buff[20];

/* Create The Dialog Box */
 cubeshell = CreateShell(&gui,"Vertex Create Cube",0,0,300,140);

 entry = CreateButton(cubeshell,"OK",10,110,70,20);
 AddCallBack(entry,CubeOK);
 entry = CreateButton(cubeshell,"Cancel",220,110,70,20);
 AddCallBack(entry,CubeCANCEL);

 sprintf(buff,"CUBE%d",nbcube);
 entry = CreateLabel(cubeshell,"Cube Name",10,10,Black,FALSE);
 cubename =CreateText(cubeshell,buff,110,10,110,20);

 entry = CreateLabel(cubeshell,"Center X Y Z",10,40,Black,FALSE);
 cubecx = CreateText(cubeshell,"0.0",110,40,50,20);
 cubecy = CreateText(cubeshell,"0.0",170,40,50,20);
 cubecz = CreateText(cubeshell,"0.0",230,40,50,20);
 entry = CreateLabel(cubeshell,"Ridge Size",10,70,Black,FALSE);                           
 cubediam = CreateText(cubeshell,"400",110,70,50,20);

 /* And Now Map the Window ... */
 MapShell(cubeshell);
 MapAllWidget(cubeshell);
 LoopShellEvent(&gui,cubeshell);
}
 

/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                    Vertex Sphere File                */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexOBJ.h"
#include"VertexDisplay.h"
#include"stdio.h"

extern XWidget *objlist,*msg;
extern XGUI gui;
extern int nbobj,nbsphere;
extern unsigned long Black;

XShell *sphshell;
XWidget *sphform,*sphdiam,*sphres,*sphname;
XWidget *sphcx,*sphcy,*sphcz;

void RecoverData(int *res,float *cx,float *cy,float *cz,float *diam)
{
 *res = (int)atoi(GetTextString(sphres));
 *res = (*res<2)?2:*res;
 *cx = VertexATOF(GetTextString(sphcx));
 *cy = VertexATOF(GetTextString(sphcy));
 *cz = VertexATOF(GetTextString(sphcz));
 *diam = VertexATOF(GetTextString(sphdiam));
}
  
void BuildSphereOBJ(BOOL Hemi)
{
 OBJ3D *ob;
 float diameter,*tmp,pas;
 float centerX,centerY,centerZ;
 int i,j,res;
 int *ftmp;
 float ztmp,zpas,teta,rayon;


 /* New Sphere */
 nbsphere++;
 
 /* Recover The data */
 RecoverData(&res,&centerX,&centerY,&centerZ,&diameter);

 /* searching the object pointer in the global object List*/
 if( (ob = FindNextPlace()) == 0 )
    return;

 AllocOBJ3D(ob,res*res+res,2*res*res);
 tmp = (float *)ob->PTtable;

 diameter /= 2.0; /* We win A Variable! */

 if(Hemi)
    zpas = diameter/(float)res;     /* The Z pas for the hemisphere */
 else
    zpas = 2*diameter/(float)res;   /* and for the sphere           */

 pas  = 2*PI/(float)res;

 ob->Center.X = centerX;
 ob->Center.Y = centerY;
 ob->Center.Z = centerZ;

 memset(ob->Name,0,25);
 strcpy(ob->Name,GetTextString(sphname)); 

 for(ztmp=diameter,i=0; i<=res; i++,ztmp-=zpas) 
    {
     if(i==1 || i==(res-1))
        ztmp += ztmp<0.0? -zpas/3 : +zpas/3;

     rayon = sqrt(diameter*diameter-ztmp*ztmp);

     for(teta=0,j=0; j<res ; j++,teta+=pas)
        {
          *tmp++ = centerX + rayon*cos(teta);
          *tmp++ = centerY + rayon*sin(teta);
          *tmp++ = centerZ + ztmp;
        }
     if(i==1 || i==(res-1) )
        ztmp -= ztmp<0.0 ? -zpas/3 : zpas/3;
    }

 ftmp = (int *)ob->FCtable;

 for(i=0;i<res;i++)
    for(j=0;j<res;j++)
       {
         if(j < res-1)
           {
            *ftmp++ = i*res+j+res;
            *ftmp++ = i*res+j+1;
            *ftmp++ = i*res+j;

            *ftmp++ = i*res+j+1;
            *ftmp++ = i*res+j+res;
            *ftmp++ = i*res+j+res+1;
           }
         else
           {
            *ftmp++ = i*res+j+res;
            *ftmp++ = i*res;
            *ftmp++ = i*res+j;

            *ftmp++ = i*res;
            *ftmp++ = (i+1)*res+j;
            *ftmp++ = (i+1)*res;
           }
       }

 AddListItem(objlist,GetTextString(sphname));
 DestroyShell(&gui,sphshell);
 VertexReDisplay();
}

void BuildHemiSphere()
{
 BuildSphereOBJ(1);
}

void BuildSphere()
{
 BuildSphereOBJ(0);
} 

void SphereCANCEL()
{
 DestroyShell(&gui,sphshell);
}

void BuildSphereBox(char *name,void fct())
{
 XWidget *entry;
 char buff[25];

 sphshell = CreateShell(&gui,name,0,0,300,200);
 entry = CreateButton(sphshell,"OK",10,160,70,20);
 AddCallBack(entry,fct);
 entry = CreateButton(sphshell,"CANCEL",220,160,70,20);
 AddCallBack(entry,SphereCANCEL);
 sprintf(buff,"SPH%d",nbsphere);
 entry = CreateLabel(sphshell,"Sphere Name",10,10,Black,FALSE);
 sphname  = CreateText(sphshell,buff,110,10,100,20);
 entry = CreateLabel(sphshell,"Center X Y Z",10,40,Black,FALSE);
 sphcx = CreateText(sphshell,"0.0",110,40,50,20);
 sphcy = CreateText(sphshell,"0.0",170,40,50,20);
 sphcz = CreateText(sphshell,"0.0",230,40,50,20);
 entry = CreateLabel(sphshell,"Diameter",10,70,Black,FALSE);                           
 sphdiam = CreateText(sphshell,"400.0",110,70,50,20);
 entry = CreateLabel(sphshell,"Resolution",10,100,Black,FALSE); 
 sphres = CreateText(sphshell,"15",110,100,50,20);                          

 /* And Now Map the Window ... */
 MapShell(sphshell);
 MapAllWidget(sphshell);
 LoopShellEvent(&gui,sphshell);
}


void VertexCreateSphere()
{
 BuildSphereBox("Vertex Create Sphere",BuildSphere);
}

void VertexCreateHSphere()
{
 BuildSphereBox("Vertex Create HemiSphere",BuildHemiSphere);
}

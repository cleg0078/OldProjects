/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                  Vertex Operation File               */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexOper.h"
#define MYABS(x) x>0?x:-x

/* Global Data Used */
extern SCENE3D global_scene;
extern XGUI gui;
extern XShell *toplevel;
extern XWidget *objlist,*msg;
extern int ViewType;
extern Display *dpy;
extern GC gc;
extern Window win;
extern Window win3D,winXY,winXZ,winYZ;
extern int win_maxx,win_maxy;
extern int TriView,nbobj;
extern float minx,maxx,miny,maxy,minz,maxz;
extern unsigned long Black,White,Pink;

/* And now the local data */
void VertexRTRotate(); 
void VertexRTZoom();
void VertexRTMove();
void VertexRTOper(void fct(float *,int,float,float,float),int oper);
void Torsion();
XShell *opershell;
XWidget *operx,*opery,*operz;
XWidget *dname;
XWidget *t1,*t2,*t3,*t4,*t5,*t6,*t7,*t8,*t9;

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

void RecoverOperData(float *cx,float *cy,float *cz)
{
 *cx = (float)VertexATOF(GetTextString(operx));
 if( opery != NULL)
    *cy = (float)VertexATOF(GetTextString(opery));
 else
    *cy = 0.0;
 
 if( operz != NULL)
    *cz = (float)VertexATOF(GetTextString(operz));
 else
    *cz = 0.0;
}

/********************************/
/*       VertexOperShell        */
/*------------------------------*/
/*   Build The Operation Shell  */
/*------------------------------*/
/* Parameter:                   */
/* IN:   The Messages           */
/* OUT:  NONE                   */
/*------------------------------*/
/*Comments: NO                  */
/********************************/

void DialogCANCEL()
{
 DestroyShell(&gui,opershell);
 VertexReDisplay();
}

void VertexOperShell(char *title,char *ms1,char *ms2,char *ms3,char *def,void fct(),int typeoper,int torsion)
{
 XWidget *entry;
 
 /* For Torsion Usage */
 operx = opery = operz = NULL;

 opershell = CreateShell(&gui,title,0,0,250,140);

 entry = CreateButton(opershell,"Ok",10,110,70,20);
 AddCallBack(entry,fct);
 entry = CreateButton(opershell,"Cancel",170,110,70,20);
 AddCallBack(entry,DialogCANCEL);

 if( torsion == 0 )
   { 
    entry = CreateButton(opershell,"Screen",90,110,70,20);
    switch( typeoper )
          {
           case 1: 
                AddCallBack(entry,VertexRTZoom);
                break;
           case 2:  
                AddCallBack(entry,VertexRTMove);
                break;
           case 3:
                AddCallBack(entry,VertexRTRotate);
                break;
         }
 
   opery = CreateText(opershell,def,110,35,60,20);
   operz = CreateText(opershell,def,110,55,60,20);
   entry = CreateLabel(opershell,ms2,30,35,Black,FALSE);
   entry = CreateLabel(opershell,ms3,30,55,Black,FALSE);
  }

 operx = CreateText(opershell,def,110,15,60,20);
 entry = CreateLabel(opershell,ms1,30,15,Black,FALSE);

 /* And Now Map the Window ... */
 MapShell(opershell);
 MapAllWidget(opershell);
 LoopShellEvent(&gui,opershell);
}

/********************************/
/*          VertexOper          */
/*------------------------------*/
/* execute The Desired Operation*/
/*   For An Object Or the Scene */
/*------------------------------*/
/* Parameter:                   */
/* IN:   the Operation Fct Ptr  */
/* OUT:  NONE                   */
/*------------------------------*/
/*Comments:                     */
/* All informations come from   */
/* the Dialog Box.              */
/********************************/


void VertexOper(void fct(float *,int,float,float,float),BOOL OperCenter,BOOL screen)
{
 TREE_OBJ3D *ltmp;
 OBJ3D *ob;
 float cx,cy,cz;
 char *buff;
 int pl;

 RecoverOperData(&cx,&cy,&cz);
 
 for(ltmp=global_scene.Head ; ltmp!=NULL ; ltmp = ltmp->Brother )
    {
     if(ltmp->Actual->MMObj1 == NULL)
       if( (ltmp->Actual->MMObj1 = BuildMinMaxOBJ(ltmp->Actual)) == NULL) 
          return;

     if(ltmp->Actual->MMObj2 == NULL )
       if( (ltmp->Actual->MMObj2 = BuildMinMaxOBJ(ltmp->Actual)) == NULL)
          return;

     CopyOBJ3D(ltmp->Actual->MMObj1,ltmp->Actual->MMObj2);
    }


 if( (pl=GetActiveListItem(objlist,&buff)) == -1)
   { 
     ltmp = global_scene.Head; 
     while(ltmp)
          { 
           ob = ltmp->Actual;
           if(ob)
             {
              if(screen==FALSE)
                {
                  fct((float *)ob->PTtable,ob->NBPT3D,cx,cy,cz);
                  fct((float *)ob->MMObj1->PTtable,ob->MMObj1->NBPT3D,cx,cy,cz);
                }
              else
                {
                  fct((float *)ob->MMObj2->PTtable,ob->MMObj2->NBPT3D,cx,cy,cz);
                  VertexRTDisplay(ob->MMObj2);
                }
  
              if(OperCenter) 
                 fct((float *)&ob->Center,1,cx,cy,cz);
             }
           ltmp = ltmp->Brother;
          }
   } 
   else
   {
    ob = FindNamedOBJ(buff,pl);
    if(ob)
      {
       if(screen == FALSE )
         {
           fct((float *)ob->PTtable,ob->NBPT3D,cx,cy,cz);
           fct((float *)ob->MMObj1->PTtable,ob->MMObj1->NBPT3D,cx,cy,cz);
         }
       else
          {
           fct((float *)ob->MMObj2->PTtable,ob->MMObj2->NBPT3D,cx,cy,cz);
           VertexRTDisplay(ob->MMObj2);
          }  
         
       if(OperCenter)
          fct((float *)&ob->Center,1,cx,cy,cz);
      }
   }
 
 DestroyShell(&gui,opershell);
 VertexReDisplay();
}


/********************************/
/*          VertexZoom          */
/*------------------------------*/
/*  Zoom an object or the scene */
/*------------------------------*/
/* Parameter:                   */
/* IN:   NONE                   */
/* OUT:  NONE                   */
/*------------------------------*/
/*Comments:                     */
/* All informations come from   */
/* the Dialog Box.              */
/********************************/

void ZoomPT3D(float *ptmp,int max,float cx,float cy,float cz)
{
 int i;

 for(i=max; i ; i--)
    {
     *ptmp++ *= cx; *ptmp++ *= cy; *ptmp++ *= cz;
    }
} 
 
void VertexZoomOK()
{ 
 VertexOper(ZoomPT3D,FALSE,FALSE);
}

void VertexZoom()
{
 VertexOperShell("VERTEX Free Zoom","Coeff X","Coeff Y",
                 "Coeff Z","1.0",VertexZoomOK,1,0);
}

/********************************/
/*          VertexMove          */
/*------------------------------*/
/*  Move an object or the scene */
/*------------------------------*/
/* Parameter:                   */
/* IN:   NONE                   */
/* OUT:  NONE                   */
/*------------------------------*/
/*Comments:                     */
/* All informations come from   */
/* the Dialog Box.              */
/********************************/

void MovePT3D(float *ptmp,int max,float cx,float cy,float cz)
{
 int i;

 for(i=max; i ; i--)
    {
     *ptmp++ += cx; *ptmp++ += cy; *ptmp++ += cz;
    }
}
 
void VertexMoveOK()
{
 VertexOper(MovePT3D,TRUE,FALSE);  /* Object Move With the Center */
}

void VertexMove()
{
 VertexOperShell("VERTEX Free Move","Move X","Move Y",
                 "Move Z","0.0",VertexMoveOK,2,0);
}


/********************************/
/*          VertexRotate        */
/*------------------------------*/
/*  Compute the rotation of an  */ 
/*      object or the scene     */
/*------------------------------*/
/* Parameter:                   */
/* IN:   NONE                   */
/* OUT:  NONE                   */
/*------------------------------*/
/*Comments:                     */
/* All informations come from   */
/* the Dialog Box.              */
/********************************/

void RotatePT3D(float *ptmp,int max,float cx,float cy,float cz)
{
 int i;
 float *tmp,x,y,z;
 double cox,six,coy,siy,coz,siz;

/* For Speed, We Perform the Cosinus and sinus */ 
 cox = cos((cx*3.14159265)/180.0);
 six = sin((cx*3.14159265)/180.0);
 coy = cos((cy*3.14159265)/180.0);
 siy = sin((cy*3.14159265)/180.0);
 coz = cos((cz*3.14159265)/180.0);
 siz = sin((cz*3.14159265)/180.0);
 
/* Now Rotate the Z Axe */

 if( cz != 0.0 )
    for(tmp=ptmp,i=max; i ; i--)
       {
        x = tmp[0]; y = tmp[1]; z=tmp[2]; 
        *tmp++ =(float)((double)x*coz-(double)y*siz); 
        *tmp++ =(float)((double)x*siz+(double)y*coz); 
        tmp++;
       }

/* Go on with the Y axe */
 if( cy != 0.0 )
    for(tmp=ptmp,i=max; i ; i--)
       {
        x = tmp[0]; y = tmp[1]; z=tmp[2];
        *tmp++ =(float)((double)x*coy+(double)z*siy); 
        tmp++; 
        *tmp++ =(float)((double)z*coy-(double)x*siy);
       }

/* I Dont know why but we finish with the X axe */
 if( cx != 0.0)
    for(tmp=ptmp,i=max; i ; i--)
       {
        x = tmp[0]; y = tmp[1]; z=tmp[2];
        tmp++;
        *tmp++ = (float)((double)y*cox-(double)z*six); 
        *tmp++ = (float)((double)y*six+(double)z*cox);
       }
}

void VertexRotateOK()
{
 VertexOper(RotatePT3D,TRUE,FALSE);  /* Object Rottate WithOut the Center */
}

void VertexRotate()
{
 VertexOperShell("VERTEX Free Rotation"," X Angle","Y Angle",
                 "Z Angle","0.0",VertexRotateOK,3,0);
}
  

void VertexRTOper(void fct(float *,int,float,float,float),int oper)
{
 Window evwin;
 TREE_OBJ3D *ltmp;
 OBJ3D *ob;
 int cont=1;
 char test[2];
 XComposeStatus compose;
 KeySym key;
 int pl,button=0,start,rotate,debx=0,deby=0;
 float angleX,angleY,angleZ,*ptmp;
 float coeffx=0.0,coeffy=0.0,coeffz=0.0;
 XEvent ev;
 char tmp[20],*buff;

 if( TriView == FALSE )
    XClearWindow(dpy,win);
 else
   {
    XClearWindow(dpy,winXY);
    XClearWindow(dpy,winXZ);
    XClearWindow(dpy,winYZ);
    XClearWindow(dpy,win3D);
   }
     
 start = TRUE;
 rotate = TRUE;
 angleZ = 0.0;

 switch(oper)
       {
        case(2):  coeffx = (maxx-minx)/2.0;
                  coeffy = (maxy-miny)/2.0;
                  coeffz = (maxz-minz)/2.0;
                  break;
        case(1):  coeffx = 35.0;
                  coeffy = 35.0;
                  coeffz = 35.0;
                  break;
        case(3):  coeffx = 90.0;
                  coeffy = 90.0;
                  coeffz = 90.0;
       }
            
 XSetForeground(dpy,gc,White);
 
 for(ltmp=global_scene.Head ; ltmp ; ltmp = ltmp->Brother )
    {
     if(ltmp->Actual->MMObj1 == NULL)
        ltmp->Actual->MMObj1 = BuildMinMaxOBJ(ltmp->Actual);
     if(ltmp->Actual->MMObj2 == NULL )
       ltmp->Actual->MMObj2 = BuildMinMaxOBJ(ltmp->Actual);
     CopyOBJ3D(ltmp->Actual->MMObj1,ltmp->Actual->MMObj2);
    }

 while(cont == TRUE )
      {
       if(rotate==TRUE)
         {
          for(ltmp=global_scene.Head ; ltmp ; ltmp = ltmp->Brother )
             {
               XSetForeground(dpy,gc,White); 
               XSetFunction(dpy,gc,GXcopy); 
               VertexRTDisplay(ltmp->Actual->MMObj2);
             } 
          rotate = FALSE;
         }

       XNextEvent(dpy,&ev);

       evwin = ev.xbutton.window;

       if( evwin==win || evwin==win3D || evwin==winXY || evwin==winXZ || evwin==winYZ )
         switch(ev.type)
             {
              case(ButtonPress):
                  if(start==TRUE)
                    {
                     start = FALSE;
                     debx = ev.xbutton.x;
                     deby = ev.xbutton.y;
                     button = ev.xbutton.button;
                    }
                    break;

              case(ButtonRelease):
                   start = TRUE;
                   break;

              case(MotionNotify):
                   rotate = FALSE;
                   if(start == FALSE )
                      switch(button)
                            {
                             case(1):      /* X Oper */
                                 if( MYABS(ev.xbutton.x-debx) )
                                   {
                                    angleX = coeffx*(float)(ev.xbutton.x-debx)/((win_maxx)/2.0);
                                    sprintf(tmp,"%f",angleX);
                                    SetTextString(operx,tmp); 
                                    rotate = TRUE;
                                   }
                                  break;
                             case(2):
                                 if( MYABS(deby-ev.xbutton.y) )
                                   {
                                    angleY = coeffy*(float)(deby-ev.xbutton.y)/((win_maxy)/2.0);
                                    sprintf(tmp,"%f",angleY);
                                    SetTextString(opery,tmp);  
                                    rotate = TRUE;
                                   }
                                 break;
                             case(3):
                                 angleZ = coeffz*(float)(ev.xbutton.x-debx)/((win_maxx-120.0)/2.0);  

                                 sprintf(tmp,"%f",angleZ);
                                 SetTextString(operz,tmp);
                                 rotate = TRUE;   
                            }
                     break;
                    
              case(KeyRelease):
                      XLookupString((XKeyEvent *)&ev,test,1,&key,&compose);
                      if( test[0] == ' ' )
                          cont = 0;
                       break;
            }
        

      if( (pl=GetActiveListItem(objlist,&buff)) == -1 ) 
        {
         if(rotate == TRUE )
           {
               for(ltmp=global_scene.Head ; ltmp ; ltmp = ltmp->Brother )
                 {
                  XSetFunction(dpy,gc,GXcopy);
                  XSetForeground(dpy,gc,Black);
                  VertexRTDisplay(ltmp->Actual->MMObj2);
                  memcpy(ltmp->Actual->MMObj2->PTtable,ltmp->Actual->MMObj1->PTtable,8*sizeof(PT3D));
                  ptmp = (float *)ltmp->Actual->MMObj2->PTtable;
                  RecoverOperData(&angleX,&angleY,&angleZ);
                  fct(ptmp,8,angleX,angleY,angleZ);
                 }
           }
        }
      else
        {
         if(rotate == TRUE)
           {
            ob = FindNamedOBJ(buff,pl); 
            if(ob == NULL)
               return; 
            XSetFunction(dpy,gc,GXcopy);
            XSetForeground(dpy,gc,Black);
            VertexRTDisplay(ob->MMObj2);
            memcpy(ob->MMObj2->PTtable,ob->MMObj1->PTtable,8*sizeof(PT3D));  
            ptmp = (float *)ob->MMObj2->PTtable;
            RecoverOperData(&angleX,&angleY,&angleZ);
            fct(ptmp,8,angleX,angleY,angleZ);
           }
       } 
    }

 XSetFunction(dpy,gc,GXcopy);
}


void VertexRTRotate()
{
 VertexRTOper(RotatePT3D,3);
}

void VertexRTZoom()
{
 VertexRTOper(ZoomPT3D,1);
}

void VertexRTMove()
{
 VertexRTOper(MovePT3D,2);
}


void DupOK()
{
 int pl;
 OBJ3D *dup,*ob;
 char *buff;
 
 if( (dup = FindNextPlace()) == 0 )
   {
    SetTextString(msg,"No Memory For Duplicate Object");
    return;
   }

 pl= GetActiveListItem(objlist,&buff);
 ob = FindNamedOBJ(buff,pl);

 AllocOBJ3D(dup,ob->NBPT3D,ob->NBFACE);
 memcpy(dup->PTtable,ob->PTtable,ob->NBPT3D*sizeof(PT3D));
 memcpy(dup->FCtable,ob->FCtable,ob->NBFACE*sizeof(FACE3D));
 memset(dup->Name,0,25);
 strcpy(dup->Name,GetTextString(dname));
 
 AddListItem(objlist,GetTextString(dname));
 DialogCANCEL();
}
 
void VertexDuplicate()
{
 XWidget *entry;
 OBJ3D *ob;
 int pl;
 char *buff,tmp[50];

 if( (pl=GetActiveListItem(objlist,&buff)) == -1)
   {
    SetTextString(msg,"Choose An Object For Duplicate Operation");
    return;
   }

 opershell = CreateShell(&gui,"VERTEX Duplication" ,0,0,280,100);

 entry = CreateButton(opershell,"Ok",10,70,70,20);
 AddCallBack(entry,DupOK);
 entry = CreateButton(opershell,"Cancel",200,70,70,20);
 AddCallBack(entry,DialogCANCEL);

 sprintf(tmp,"Duplicate %s To ",buff);
 entry = CreateLabel(opershell,tmp,10,20,Black,FALSE);
 nbobj++;
 sprintf(tmp,"OBJ%d",nbobj); 
 dname = CreateText(opershell,tmp,160,20,70,20);

 MapShell(opershell);
 MapAllWidget(opershell);
 LoopShellEvent(&gui,opershell);

}

void FreeOk(void)
{
 float v1,v2,v3,v4,v5,v6,v7,v8,v9;
 float x,y,z;
 int i,pl;
 OBJ3D *ob;
 PT3D *p; 
 char *buff;

 v1 = (float)VertexATOF(GetTextString(t1));
 v2 = (float)VertexATOF(GetTextString(t2));
 v3 = (float)VertexATOF(GetTextString(t3));
 v4 = (float)VertexATOF(GetTextString(t4));
 v5 = (float)VertexATOF(GetTextString(t5));
 v6 = (float)VertexATOF(GetTextString(t6));
 v7 = (float)VertexATOF(GetTextString(t7));
 v8 = (float)VertexATOF(GetTextString(t8));
 v9 = (float)VertexATOF(GetTextString(t9));

 pl= GetActiveListItem(objlist,&buff);
 ob = FindNamedOBJ(buff,pl);

 for(i=0,p=ob->PTtable;i<ob->NBPT3D;i++)
    {
      x = p[i].X;
      y = p[i].Y;
      z = p[i].Z;

      p[i].X = x*v1+y*v4+z*v7;
      p[i].Y = x*v2+y*v5+z*v8;
      p[i].Z = x*v3+y*v6+z*v9;
    }

 DialogCANCEL();
}

void VertexFreeOper(void)
{
 int pl;
 char *buff;
 XWidget *entry;

 if( (pl=GetActiveListItem(objlist,&buff)) == -1)
   {
    SetTextString(msg,"Choose An Object For Free Operation");
    return;
   }

 opershell = CreateShell(&gui,"VERTEX Free Operation" ,0,0,250,200);
 entry = CreateLabel(opershell,"Matrice Coefficients",55,10,Black,FALSE);
 t1 = CreateText(opershell,"1.0",10,40,70,20);
 t2 = CreateText(opershell,"0.0",90,40,70,20);
 t3 = CreateText(opershell,"0.0",170,40,70,20);
 t4 = CreateText(opershell,"0.0",10,70,70,20);
 t5 = CreateText(opershell,"1.0",90,70,70,20);
 t6 = CreateText(opershell,"0.0",170,70,70,20);
 t7 = CreateText(opershell,"0.0",10,100,70,20);
 t8 = CreateText(opershell,"0.0",90,100,70,20);
 t9 = CreateText(opershell,"1.0",170,100,70,20);

 entry = CreateButton(opershell,"Ok",10,170,70,20);
 AddCallBack(entry,FreeOk);
 entry = CreateButton(opershell,"Cancel",170,170,70,20);
 AddCallBack(entry,DialogCANCEL);

 MapShell(opershell);
 MapAllWidget(opershell);
 LoopShellEvent(&gui,opershell);

}

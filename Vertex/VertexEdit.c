/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                     Vertex Edit File                 */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexEdit.h"

extern Window win,rootwindow;
extern GC gc;
extern Display *dpy;
extern int win_maxx,win_maxy;
extern float maxx,maxy,maxz,minx,miny,minz;
extern int ViewType,TriView;
extern SCENE3D global_scene;
extern XGUI gui;
extern XShell *toplevel;
extern XWidget *objlist,*msg;
extern int nbobj;
extern unsigned long White,Black;

/*local data */
XShell *edshell;
OBJ3D *act;
PTList *ptl;
FCList *fcl,*strict_fcl;
int Creation;
int vefirst;
PT3D ptold;

void EditQuit()
{
 char buff[25];
 OBJ3D *ob;

 ReBuildOBJ(&act,ptl,fcl);

 if(Creation)
   {
     if(act->NBPT3D>0 || act->NBFACE>0 )
       {  
        sprintf(buff,"OBJ%d",nbobj);
        
        if( (ob = FindNextPlace()) == 0 )
           return;

        AllocOBJ3D(ob,act->NBPT3D,act->NBFACE);
        strcpy(ob->Name,buff);
        memcpy(ob->PTtable,act->PTtable,sizeof(PT3D)*act->NBPT3D);
        memcpy(ob->FCtable,act->FCtable,sizeof(FACE3D)*act->NBFACE);
        AddListItem(objlist,buff);
       }       
   }        

 DestroyPTList(&ptl);
 DestroyFCList(&fcl);
 DestroyShell(&gui,edshell);
 SetTextString(msg,"Quit Edit Mode");
 VertexReDisplay(); 
}

void RTFaceDisplay(PT3D *pt,int ind)
{
 int i,xc,yc;
 short x1,x2,x3,y1,y2,y3;
 int i1,i2,i3;
 XPoint ptl[4];
 float coeffx,coeffy;
 PT3D *ptmp;
 FACE3D *ftmp;

 ftmp = (FACE3D *)act->FCtable;
 ptmp = (PT3D *)act->PTtable;
 xc = win_maxx/2;
 yc = win_maxy/2;

 switch( ViewType )
       {
        case( PLAN_XY ):  
        case( PLAN_XZ ):        
          coeffx = (win_maxx)/(float)(maxx-minx+1);
             if(coeffx <= 0.0)
                coeffx = 1.0;
          break;

        case( PLAN_YZ ): 
             coeffx = (win_maxx)/(float)(maxy-miny+1);
             if(coeffx <= 0.0)
                coeffx = 1.0;
       }

  switch( ViewType )
       {
        case( PLAN_YZ ):
        case( PLAN_XZ ):
          coeffy = (win_maxy)/(float)(maxz-minz+1);
             if(coeffy <= 0.0)
                coeffy = 1.0;
          break;

        case( PLAN_XY ):
             coeffy = (win_maxy)/(float)(maxy-miny+1);
             if(coeffy <= 0.0)
                coeffy = 1.0;
       }
     
 for(i=0; i < act->NBFACE ; i++ )
   {
     i1=ftmp[i].I1;
     i2=ftmp[i].I2;
     i3=ftmp[i].I3; 
     if( i1==ind || i2==ind || i3==ind)
      {
        switch(ViewType)
              {
                case(PLAN_XY):
                    ptmp[ind].X = pt->X;
                    ptmp[ind].Y = pt->Y;
                    x1  = (short)(ptmp[i1].X*coeffx+xc); 
                    y1 = (short)(-ptmp[i1].Y*coeffy+yc);
                    x2 = (short)(ptmp[i2].X*coeffx+xc);  
                    y2 = (short)(-ptmp[i2].Y*coeffy+yc);
                    x3 = (short)(ptmp[i3].X*coeffx+xc);
                    y3 = (short)(-ptmp[i3].Y*coeffy+yc);   
                    ptl[0].x = x1; ptl[0].y = y1;   
                    ptl[1].x = x2; ptl[1].y = y2; 
                    ptl[2].x = x3; ptl[2].y = y3;   
                    ptl[3].x = x1; ptl[3].y = y1; 
                    XDrawLines(dpy,win,gc,ptl,4,CoordModeOrigin);
                    XSetForeground(dpy,gc,Black);
                    XFillRectangle(dpy,win,gc,x1-2,y1-2,4,4);
                    XSetForeground(dpy,gc,White);

                    if(vefirst == 1)
                      {
                       ptold.X = pt->X;
                       ptold.Y = pt->Y;
                       vefirst = 0;
                      }
                    else
                      {
                       ptmp[ind].X = ptold.X;   
                       ptmp[ind].Y = ptold.Y;
                       x1  = (short)(ptmp[i1].X*coeffx+xc);
                       y1 = (short)(-ptmp[i1].Y*coeffy+yc);
                       x2 = (short)(ptmp[i2].X*coeffx+xc);
                       y2 = (short)(-ptmp[i2].Y*coeffy+yc);
                       x3 = (short)(ptmp[i3].X*coeffx+xc);
                       y3 = (short)(-ptmp[i3].Y*coeffy+yc);
                       ptl[0].x = x1; ptl[0].y = y1;
                       ptl[1].x = x2; ptl[1].y = y2;
                       ptl[2].x = x3; ptl[2].y = y3;
                       ptl[3].x = x1; ptl[3].y = y1;
                       XDrawLines(dpy,win,gc,ptl,4,CoordModeOrigin);
                       XSetForeground(dpy,gc,Black);
                       XFillRectangle(dpy,win,gc,x1-2,y1-2,4,4);
                       XSetForeground(dpy,gc,White);
                       ptold.X = pt->X;
                       ptold.Y = pt->Y;
                      }
                   break;

                case(PLAN_XZ):
                    ptmp[ind].X = pt->X;
                    ptmp[ind].Z = pt->Z;
                    x1  = (short)(ptmp[i1].X*coeffx+xc);
                    y1 = (short)(-ptmp[i1].Z*coeffy+yc);
                    x2 = (short)(ptmp[i2].X*coeffx+xc);
                    y2 = (short)(-ptmp[i2].Z*coeffy+yc);
                    x3 = (short)(ptmp[i3].X*coeffx+xc);
                    y3 = (short)(-ptmp[i3].Z*coeffy+yc);
                    ptl[0].x = x1; ptl[0].y = y1;
                    ptl[1].x = x2; ptl[1].y = y2;
                    ptl[2].x = x3; ptl[2].y = y3;
                    ptl[3].x = x1; ptl[3].y = y1;
                    XDrawLines(dpy,win,gc,ptl,4,CoordModeOrigin);
                    XSetForeground(dpy,gc,4);
                    XFillRectangle(dpy,win,gc,x1-2,y1-2,4,4);
                    XSetForeground(dpy,gc,White);

                    if(vefirst == 1)
                      {
                       ptold.X = pt->X;
                       ptold.Z = pt->Z;
                       vefirst = 0;
                      }
                    else
                      {
                       ptmp[ind].X = ptold.X;
                       ptmp[ind].Z = ptold.Z;
                       x1  = (short)(ptmp[i1].X*coeffx+xc);
                       y1 = (short)(-ptmp[i1].Z*coeffy+yc);
                       x2 = (short)(ptmp[i2].X*coeffx+xc);
                       y2 = (short)(-ptmp[i2].Z*coeffy+yc);
                       x3 = (short)(ptmp[i3].X*coeffx+xc);
                       y3 = (short)(-ptmp[i3].Z*coeffy+yc);
                       ptl[0].x = x1; ptl[0].y = y1;
                       ptl[1].x = x2; ptl[1].y = y2;
                       ptl[2].x = x3; ptl[2].y = y3;
                       ptl[3].x = x1; ptl[3].y = y1;
                       XDrawLines(dpy,win,gc,ptl,4,CoordModeOrigin);
                       XSetForeground(dpy,gc,4);
                       XFillRectangle(dpy,win,gc,x1-2,y1-2,4,4);
                       XSetForeground(dpy,gc,White);
                       ptold.X = pt->X;
                       ptold.Z = pt->Z;
                      }

           }
     }
  }
}


void LocatePoint(PT3D *pt,int ind)
{
 BOOL found=0;
 int i,posx,posy,max;
 float sx,sy,mx,my;
 char bx[25],by[25];
 char test[2];
 XComposeStatus compose;
 KeySym key;
 XEvent ev;
 FCList *tmp;

 strict_fcl = NULL;
/*
 if(ind != -1)
    for(tmp=fcl,max=NbFace(fcl),i=0; i<max ; tmp=tmp->Next, i++)
       {
        if(tmp->Info.I1 == ind || tmp->Info.I2 == ind || tmp->Info.I3 == ind)
           if( AddFace(&strict_fcl,tmp->Info.I1,tmp->Info.I2,tmp->Info.I3) == -1 )
              return; 
       }
*/

 vefirst = 0;
 while(!found)
      {
       XNextEvent(dpy,&ev);
       posx = ev.xbutton.x;
       posy = ev.xbutton.y;
       switch(ViewType)
             {
              case(PLAN_XY):
              mx=minx; my=maxy; sx = maxx-minx; sy=maxy-miny;
              sprintf(bx,"X: %4.4f",minx+posx*sx/win_maxx);
              sprintf(by,"Y: %4.4f",maxy-posy*sy/win_maxy);
              break;
              case(PLAN_XZ):
              mx=minx; my=maxz; sx = maxx-minx; sy=maxz-minz;
              sprintf(bx,"X: %4.4f",minx+posx*sx/(win_maxx-120));
              sprintf(by,"Z: %4.4f",maxz-posy*sy/(win_maxy));
              break;
              case(PLAN_YZ):
              mx=minz; my=maxy; sy = maxy-miny; sx=maxz-minz;
              sprintf(bx,"Z: %4.4f",minz+posx*sx/win_maxx);
              sprintf(by,"Y: %4.4f",maxy-posy*sy/win_maxy);
              break;
             } 

       XSetForeground(dpy,gc,Black);
       XFillRectangle(dpy,win,gc,0,0,170,20);
       XSetForeground(dpy,gc,White);
       XDrawString(dpy,win,gc,10,10,bx,strlen(bx));
       XDrawString(dpy,win,gc,90,10,by,strlen(by));

       if(ev.xbutton.window == win)
          switch(ev.type)
                {
                 case(ButtonPress):
                     if(vefirst == 0)
                       {
                        pt->X = mx+posx*sx/win_maxx;
                        pt->Y = my-posy*sy/win_maxy;
 
                        vefirst = 1;
                        if(ind == -1 )
                          found = 1;
                       }
                 break;
                 case(KeyRelease):
                      XLookupString((XKeyEvent *)&ev,test,1,&key,&compose);
                      if( test[0] == ' ' )
                         found = 1;

                      vefirst = 1;
                      XSetFunction(dpy,gc,GXxor);
                      RTFaceDisplay(pt,ind);
                      XSetFunction(dpy,gc,GXcopy);
                      break;

                  case(MotionNotify):
                      switch( ViewType )
                            {
                             case( PLAN_XY ):
                                  pt->X = mx+posx*sx/win_maxx;
                                  pt->Y = my-posy*sy/win_maxy;
                                  break;
                             case( PLAN_XZ ):
                                  pt->X = mx+posx*sx/win_maxx;
                                  pt->Z = my-posy*sy/win_maxy;
                           }

                      XSetFunction(dpy,gc,GXxor);
                      RTFaceDisplay(pt,ind);
                      XSetFunction(dpy,gc,GXcopy);
                      break;
                  }

       XFlush(dpy);
      }

 XSetFunction(dpy,gc,GXcopy);
/*
 DestroyFCList(&strict_fcl);
*/
}

void EditAddPoint()
{
 PT3D pt;

 SetTextString(msg,"Pick The Position Of The Point");
 LocatePoint(&pt,-1);

 switch(ViewType)
       {
        case(PLAN_XY):  AddPoint(&ptl,pt.X,pt.Y,0.0); break;
        case(PLAN_XZ):  AddPoint(&ptl,pt.X,0.0,pt.Y); break;
        case(PLAN_YZ):  AddPoint(&ptl,0.0,pt.Y,pt.X); break;
       }

 ReBuildOBJ(&act,ptl,fcl);
 SetTextString(msg,"Add Point: OK");
 VertexEDITDisplay(act);
}

void EditGLOB()
{
 ReBuildOBJ(&act,ptl,fcl);
 VertexEDITDisplay(act);
}

void EditDelPoint()
{
 PT3D pt;
 int nb = 0,i;
 PTList *lpt;

 SetTextString(msg,"Choose The Point");
 LocatePoint(&pt,-1);

 switch(ViewType)
       {
        case(PLAN_XY):  nb=FindPointXY(ptl,pt.X,pt.Y,7.0); break;
        case(PLAN_XZ):  nb=FindPointXZ(ptl,pt.X,pt.Y,7.0); break;
        case(PLAN_YZ):  nb=FindPointYZ(ptl,pt.Y,pt.X,7.0); break;
       }

 if( nb>=0 )
   {
     for(i=0,lpt=ptl; i<nb ; lpt=lpt->Next,i++);
     
     DelPoint(&ptl,lpt->Info.X,lpt->Info.Y,lpt->Info.Z);
     DelAllFaceWith(&fcl,nb);
     SetTextString(msg,"Point Deleted");
     EditGLOB();
   }
 else
     SetTextString(msg,"No Point Found Here!");
}

void EditDelFace()
{
 PT3D pt[3];
 int ind[3],i;
 char tmp[20];

 for(i=0; i<3; i++ )
    {
      sprintf(tmp,"Choose Point No %d",i);
      SetTextString(msg,tmp);
      LocatePoint(&pt[i],-1);

      switch(ViewType)
            {
             case(PLAN_XY): ind[i]=FindPointXY(ptl,pt[i].X,pt[i].Y,7.0); break;
             case(PLAN_XZ): ind[i]=FindPointXZ(ptl,pt[i].X,pt[i].Y,7.0); break;
             case(PLAN_YZ): ind[i]=FindPointYZ(ptl,pt[i].Y,pt[i].X,7.0); break;
            }
    }

 if(ind[0]>=0 && ind[1]>=0 && ind[2]>=0 )
   {
    DelFace(&fcl,ind[0],ind[1],ind[2]);
    EditGLOB();
    SetTextString(msg,"Face Deleted");
   }
 else
    SetTextString(msg,"No Face Deleted");
}

void EditAddFace()
{
 PT3D pt[3];
 int ind[3],i;
 char tmp[20];

 for(i=0; i<3; i++ )
    {
      sprintf(tmp,"Choose Point No %d",i+1);
      SetTextString(msg,tmp);
      LocatePoint(&pt[i],-1);

      switch(ViewType)
            {
             case(PLAN_XY): ind[i]=FindPointXY(ptl,pt[i].X,pt[i].Y,7.0); break;
             case(PLAN_XZ): ind[i]=FindPointXZ(ptl,pt[i].X,pt[i].Y,7.0); break;
             case(PLAN_YZ): ind[i]=FindPointYZ(ptl,pt[i].Y,pt[i].X,7.0); break;
            }
    }

 if(ind[0]>=0 && ind[1]>=0 && ind[2]>=0 )
   {
    if( AddFace(&fcl,ind[0],ind[1],ind[2]) == 1 )
      { 
        EditGLOB();
        SetTextString(msg,"Add Face: OK");
      }
    }
    else
        SetTextString(msg,"No Face Added!");
}

void EditMovePoint()
{
 PT3D pto,pta; 
 int i,ind = 0;
 PTList *lpt;

 SetTextString(msg,"Choose The Point");
 LocatePoint(&pto,-1);

 switch(ViewType)
       {
        case(PLAN_XY): ind=FindPointXY(ptl,pto.X,pto.Y,7.0); 
                       /*pta.Z=0.0;  */
                       break;
        case(PLAN_XZ): ind=FindPointXZ(ptl,pto.X,pto.Y,7.0); 
                       /*pta.Z=pta.Y; pta.Y=0.0; */ 
                       break;  
        case(PLAN_YZ): ind=FindPointYZ(ptl,pto.Y,pto.X,7.0);
                       /* pta.Z=pta.X; pta.X=0.0;  */
                       break;
       }

 if( ind>=0 )
   {
     SetTextString(msg,"Pick The New Position");
     LocatePoint(&pta,ind);

     for(i=0,lpt=ptl; i<ind ; lpt=lpt->Next,i++);
     
     lpt->Info.X = pta.X;
     lpt->Info.Y = pta.Y;
     lpt->Info.Z = pta.Z;
     EditGLOB();
     SetTextString(msg,"Point Moved");
   }
 else
    SetTextString(msg,"Point Not Found Here");
}

 
void NewFaces()
{
 DestroyFCList(&fcl);
 ReBuildOBJ(&act,ptl,fcl);
 VertexEDITDisplay(act);
}

void NewPoints()
{
 DestroyPTList(&ptl);
 DestroyFCList(&fcl);
 ReBuildOBJ(&act,ptl,fcl);
 VertexEDITDisplay(act);
}
 
void VertexEdit()
{
 XWidget *entry;
 OBJ3D *ob=NULL;
 TREE_OBJ3D *ltmp;
 char *txt;
 int pl;
 int Turn3View;

 Turn3View = FALSE;

 if( TriView == TRUE )
   {
    VertexTriView();
    Turn3View = TRUE;
   } 

 if(ViewType == GLOBAL_3D)
   {
    SetTextString(msg,"Can't edit in 3D Mode, Go to 2D view!");
    if( Turn3View == TRUE )
       VertexTriView();
    return;     
   }

  ltmp = global_scene.Head;
  ptl = NULL;
  fcl = NULL;

  if( (pl=GetActiveListItem(objlist,&txt)) >= 0)
    {
      /* We Must find the object in the List: */
      /* Thanks to the object name to do this!*/
      
      ob = FindNamedOBJ(txt,pl);
      BuildPTList(&ptl,ob);
      BuildFCList(&fcl,ob->FCtable,ob->NBFACE);
      VertexEDITDisplay(ob);
      act = ob;
      Creation = 0;
    }
  else
    {
      XClearWindow(dpy,win);
      if( !(act =(OBJ3D *)calloc(1,sizeof(OBJ3D))) ) 
         return;
      Creation = 1; 
    }

 edshell = CreateShell(&gui,"Vertex Global Edition",0,0,195,100);                                
 entry = CreateButton(edshell,"Add Point",10,10,85,20);
 AddCallBack(entry,EditAddPoint);
 entry = CreateButton(edshell,"Add Face",95,10,85,20);
 AddCallBack(entry,EditAddFace);
 entry = CreateButton(edshell,"Erase Point",10,30,85,20);
 AddCallBack(entry,EditDelPoint);
 entry = CreateButton(edshell,"Erase Face",95,30,85,20);
 AddCallBack(entry,EditDelFace);
 entry = CreateButton(edshell,"New Points",10,50,85,20);
 AddCallBack(entry,NewPoints); 
 entry = CreateButton(edshell,"New Faces",95,50,85,20);
 AddCallBack(entry,NewFaces); 
 entry= CreateButton(edshell,"Move Point",10,70,85,20);
 AddCallBack(entry,EditMovePoint);
 entry = CreateButton(edshell,"Quit",95,70,85,20);
 AddCallBack(entry,EditQuit);

 MapShell(edshell);
 MapAllWidget(edshell);

 LoopShellEvent(&gui,edshell);

 if( Turn3View == TRUE )
    VertexTriView();
}



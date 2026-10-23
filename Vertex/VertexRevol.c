/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*              Vertex Revolution Object File           */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"stdio.h"
#include"VertexOBJ.h"
#include"Connect.h"
#define pi2 3.14159265

extern SCENE3D global_scene;
extern XWidget *objlist,*msg;
extern XShell *toplevel;
extern XGUI gui;
extern int nbobj,nbrevol;
extern Window win;
extern GC gc;
extern Display *dpy;
extern int ViewType,TriView;
extern int win_maxx,win_maxy;
extern float maxx,minx,maxy,miny;
extern PT3D tab[300];
extern int nbpt;
extern unsigned long Black,White,Pink;

XShell *revolshell,*rshell;
XWidget *revolname,*revolres;
int draw,first,cont,xdeb1,ydeb1,xold,yold;
int xdeb,ydeb;

void VertexRevolQuit()
{
 DestroyShell(&gui,revolshell);
}

void VRRedisplay()
{
 XSetForeground(dpy,gc,White);

  if( draw!=0 )
    {
     XDrawLine(dpy,win,gc,xdeb1,ydeb1,xdeb,ydeb);
     if( first == 1 )
       {
        first = 0;
        xold = xdeb;
        yold = ydeb;
       }
     else
       {
        XDrawLine(dpy,win,gc,xdeb1,ydeb1,xold,yold);
        xold = xdeb;
        yold = ydeb;
       }
    }
}
 
void VertexRevolAdd()
{
 XEvent ev;
 float sx,sy;
 int posx = 0,posy = 0;
 BOOL new = 1;
 char bx[25],by[25];
 char test[2];
 XComposeStatus compose;
 KeySym key;
 
 sx = maxx-minx;
 sy = maxy-miny;

 SetTextString(msg,"Press Space Bar To Stop.");

 if(nbpt<300)
   {
     tab[nbpt].Z = 0.0;
     while(new)
          {
           XNextEvent(dpy,&ev);
           posx = ev.xbutton.x;
           posy = ev.xbutton.y;
           sprintf(bx,"X: %4.4f",minx+posx*sx/(win_maxx));
           sprintf(by,"Y: %4.4f",maxy-posy*sy/(win_maxy));
           XSetForeground(dpy,gc,Black);
           XFillRectangle(dpy,win,gc,0,0,170,30);
           XSetForeground(dpy,gc,White);
           XDrawString(dpy,win,gc,10,10,bx,strlen(bx));
           XDrawString(dpy,win,gc,90,10,by,strlen(by));
       
           if(ev.xbutton.window == win) 
             {
               switch(ev.type)
                   {
                    case(ButtonPress):
                        xdeb1=tab[nbpt].X = posx;
                        ydeb1=tab[nbpt].Y = posy;

                        XSetForeground(dpy,gc,White);
                        draw = 1;
                        first = 1; 
                        nbpt++;
                        XDrawLine(dpy,win,gc,posx-5,posy,posx+5,posy);
                        XDrawLine(dpy,win,gc,posx,posy-5,posx,posy+5);
                        break;

                   case(KeyRelease):
                        XLookupString((XKeyEvent *)&ev,test,1,&key,&compose);
                        if( test[0] == ' ' )
                          {
                            draw = 0;
                            new = 0;
                          } 
                       
                        XSetForeground(dpy,gc,White); 
                        XSetFunction(dpy,gc,GXxor);
                        XDrawLine(dpy,win,gc,xdeb1,ydeb1,xold,yold); 
                        XSetFunction(dpy,gc,GXcopy);
                        break;
   
                   case(MotionNotify):
                        if( draw )
                          {
                           xdeb = posx;
                           ydeb = posy;
                           XSetFunction(dpy,gc,GXxor);
                           VRRedisplay();
                           XSetFunction(dpy,gc,GXcopy);
                          }
                        break;
                  } 
              } 
            XFlush(dpy);
          }
   }

 SetTextString(msg,"Stop Add point ");
} 

void VertexRevolDispALL()
{
 int i,posx,posy;

 XSetForeground(dpy,gc,White);
 for(i=0;i<nbpt;i++)
    {
     posx = tab[i].X;
     posy = tab[i].Y;

     XDrawLine(dpy,win,gc,posx-5,posy,posx+5,posy);
     XDrawLine(dpy,win,gc,posx,posy-5,posx,posy+5);
     if(i)
        XDrawLine(dpy,win,gc,posx,posy,(int)tab[i-1].X,(int)tab[i-1].Y);
    }
}

void VertexRevolDel()
{
 int posx,posy;

 if(nbpt>0)
   {
    posx = tab[--nbpt].X;
    posy = tab[nbpt].Y;
    XSetForeground(dpy,gc,Black);
    XDrawLine(dpy,win,gc,posx-5,posy,posx+5,posy);
    XDrawLine(dpy,win,gc,posx,posy-5,posx,posy+5);
    if(nbpt)
       XDrawLine(dpy,win,gc,(int)tab[nbpt].X,(int)tab[nbpt].Y,(int)tab[nbpt-1].X,(int)tab[nbpt-1].Y);
    XSetForeground(dpy,gc,White);
   }
}
 
void VertexRevolNew()
{
 while(nbpt)
       VertexRevolDel();
 XClearWindow(dpy,win);
 XSetForeground(dpy,gc,White);
 XDrawLine(dpy,win,gc,(win_maxx)/2,0,(win_maxx)/2,win_maxy);
}

void VertexRevolGen()
{
 OBJ3D *ob;
 float angle,pas;
 float x,y,z;
 int i,j;
 int *ftmp;
 int res;
 float *ptmp;
 char *buff;
 float sx,sy;

 if(nbpt<2)
    return;


 nbrevol++;

 buff=revolres->text.String;
 res = atoi(GetTextString(revolres));

 buff = GetTextString(revolname);
  
 sx = maxx-minx;
 sy = maxy-miny;

 if( (ob = FindNextPlace()) == 0)
    return;

 AllocOBJ3D(ob,res*nbpt,2*res*(nbpt-1));
 if(!ob)
   {
    fprintf(stderr,"Not Enough Memory\n");
    return;
   }

 ptmp = (float *)ob->PTtable;
 pas = 2*3.14159265/(float)res;

 for(i=0;i<nbpt;i++)
    for(j=0,angle=0.0;j<res;angle+=pas,j++)
       {
        x = minx+tab[i].X*sx/(win_maxx);      
        y = maxy-tab[i].Y*sy/(win_maxy);
        z = 0.0;
        *ptmp++ = x*cos(angle); 
        *ptmp++ = y; 
        *ptmp++ = -x*sin(angle);
       }

 ftmp = (int *)ob->FCtable; 
 for(i=0;i<nbpt-1;i++)
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

 strcpy(ob->Name,buff);
 AddListItem(objlist,buff);
 DestroyShell(&gui,rshell);
}

void VertexGenCANCEL()
{
 DestroyShell(&gui,rshell);
}

void VertexGenOK()
{
 XWidget *entry;
 char buff[15];

 rshell= CreateShell(&gui,"Vertex Create Revolution",0,0,250,120);
 entry = CreateButton(rshell,"OK",10,90,70,20);
 AddCallBack(entry,VertexRevolGen);

 entry = CreateButton(rshell,"CANCEL",170,90,70,20);
 AddCallBack(entry,VertexGenCANCEL);

 sprintf(buff,"REVOL%d",nbrevol);
 entry = CreateLabel(rshell,"Object Name",20,10,Black,4);
 revolname = CreateText(rshell,buff,110,10,100,20);
 entry = CreateLabel(rshell,"Resolution",20,40,Black,4);
 revolres= CreateText(rshell,"10",110,40,50,20);

 MapShell(rshell);
 MapAllWidget(rshell);
 LoopShellEvent(&gui,rshell);
}

void VertexCreateRevol()
{
 XWidget *revolfile,*revolgen,*revoladd,*revoldel,*revolnew,*revolquit;
 int Turn3View;

 Turn3View = FALSE;
 if( TriView == TRUE )
   {
    VertexTriView();
    Turn3View = TRUE;
   }

 /* Reset the Point Number */
 nbpt = 0;

 /* Build the Global Dialog Box    */
 /* But Before we Clear the window */
 /* And draw The reference Axe     */
 XClearWindow(dpy,win);
 XSetForeground(dpy,gc,White);
 XDrawLine(dpy,win,gc,(win_maxx)/2,0,(win_maxx)/2,win_maxy);
 
 revolshell = CreateShell(&gui,"Vertex Create Revolution",0,0,195,80);
 revolfile = CreateButton(revolshell,"File",10,10,85,20);
/* XtAddCallback(revolfile,XtNcallback,VertexFileGEN,NULL);
*/
 revolgen  = CreateButton(revolshell,"Generate",10,30,85,20);
 AddCallBack(revolgen,VertexGenOK);

 revoladd  = CreateButton(revolshell,"Add",96,10,85,20);
 AddCallBack(revoladd,VertexRevolAdd);
/*
 revoldel  = XtVaCreateWidget("DELETE",commandWidgetClass,revolform,RCF2,
                              XtNy,30,NULL);
 XtAddCallback(revoldel,XtNcallback,VertexRevolDel,NULL);
 revolnew  = XtVaCreateWidget("NEW",commandWidgetClass,revolform,RCF1,
                              XtNy,50,NULL);
 XtAddCallBack(revolnew,XtNcallback,VertexRevolNew,NULL);
*/
 revolquit = CreateButton(revolshell,"Quit",96,30,85,20);
 AddCallBack(revolquit,VertexRevolQuit);

 MapShell(revolshell);
 MapAllWidget(revolshell);

 LoopShellEvent(&gui,revolshell);

 if( Turn3View == TRUE)
    VertexTriView();

 VertexReDisplay();
}

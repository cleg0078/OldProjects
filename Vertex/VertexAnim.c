/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                 Vertex "Animation" File              */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexGUI.h"
#include"VertexRender.h"
#include"VertexColor.h"
#define MDepth(x) PlanesOfScreen(DefaultScreen((Display *)x))

/* Linux Server extension => Faster PutImage */
#ifdef X_SHM
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
XShmSegmentInfo shminfo;
int shm_flag;
int completion_type;
int xerror_flag;
#endif

extern XGUI gui;
extern XShell *toplevel;
extern XWidget *objlist,*msg;
extern Display *dpy;
extern GC gc;
extern int win_maxx,win_maxy,bpp;
extern float minx,maxx,miny,maxy,minz,maxz;
extern Colormap cmap;
extern int nbobj;
extern SCENE3D global_scene;
extern unsigned long White,Black,Pink;
extern unsigned long (*GetRGB24)(int,int,unsigned long,unsigned long,
                    unsigned long);

XShell *AnimShell;
XWidget *AnimMsg;
Window AnimWin;
GC AnimGC;
Pixmap AnimPix;
XImage *im;
int RenderMode,FirstColor,m;
PT3D Light;

/* Private CallBack Function */
void QuitAnim()
{
#ifdef X_SHM
 XShmDetach(dpy,&shminfo);
 shmdt(shminfo.shmaddr);
 im->data = 0;   /* XDestroyImage must not free() the detached shm address */
#endif
 XDestroyImage(im);

 XFreePixmap(dpy,AnimPix);
 XDestroyWindow(dpy,AnimWin);
 XFreeGC(dpy,AnimGC);
 DestroyShell(&gui,AnimShell);
}

void RealTimeAnim()
{
 TREE_OBJ3D *ltmp;
 char test[2];
 KeySym key;
 XComposeStatus compose;
 XEvent ev;
 float dx,dy,dz,ax,ay,az;
 float angX,angY,appX=0.0,appY=0.0,mdx=0.0,mdy=0.0;
 int flip,draw,move,rot,cont=1,dragging=0,pressx=0,pressy=0;

 XClearWindow(dpy,AnimWin);
 XSetForeground(dpy,AnimGC,White);
 
 switch(RenderMode)
       { 
        case(WIRE_RENDER):
             XSetForeground(dpy,gc,White);
             for(ltmp = global_scene.Head; ltmp ; ltmp = ltmp->Brother )
                 Display3D(AnimWin,win_maxx,win_maxy,ltmp->Actual,FALSE);
             break;
        case(OUTLINE_RENDER):
             RenderOutLine(AnimWin,AnimGC,win_maxx,win_maxy,0);
             break;
        case(FLAT_RENDER):
             RenderSolid(im,win_maxx,win_maxy,Light);  
             #ifdef X_SHM
             XShmPutImage(dpy,AnimWin,AnimGC,im,0,0,0,0,win_maxx,win_maxy,False);
             #else
             XPutImage(dpy,AnimWin,AnimGC,im,0,0,0,0,win_maxx,win_maxy);
             #endif
             memset(im->data,0,im->bytes_per_line*im->height);
       }


 SetTextString(AnimMsg,"Press Space Bar To Stop");
 XFlush(dpy);

 ax = ay = az = 0.0;

 while( cont )
      {
       flip=draw=move=rot=0;
       dx=dy=dz=0.0;
       mdx=mdy=0.0;
        
       while( XCheckWindowEvent(dpy,AnimWin,KeyPressMask|KeyReleaseMask|
                 ButtonPressMask|ButtonReleaseMask|PointerMotionMask,&ev) )
       {
       if( ev.type == KeyRelease)
          {
           XLookupString ((XKeyEvent *)&ev, test, 1, &key, &compose);

           switch(key)
            {     
             case(XK_space):   
                  cont = 0 ; break;
             case(XK_a): case(XK_A):
                  move=1;   dx = (maxx-minx)/40.0; break;
             case(XK_q): case(XK_Q):
                  move=1;   dx = -(maxx-minx)/40.0; break;
             case(XK_o): case(XK_O):
                  move=1;   dy = (maxy-miny)/40.0; break;
             case(XK_p): case(XK_P):
                  move=1;   dy = -(maxy-miny)/40.0; break;
             case(XK_l): case(XK_L):
                  move=1;   dz = (maxz-minz)/40.0; break;
             case(XK_m): case(XK_M):
                  move=1;   dz = -(maxz-minz)/40.0; break;
             case(XK_1): ax+= 1.0; break;
             case(XK_2): ax = 0.0; break;
             case(XK_3): ax-= 1.0; break;
             case(XK_4): ay+= 1.0; break;
             case(XK_5): ay = 0.0; break;
             case(XK_6): ay-= 1.0; break;
             case(XK_7): az+= 1.0; break;
             case(XK_8): az = 0.0; break;
             case(XK_9): az-= 1.0; break;
             case(XK_F1): XClearWindow(dpy,AnimWin); RenderMode=WIRE_RENDER; draw=1; break;
             case(XK_F2): XClearWindow(dpy,AnimWin); RenderMode=OUTLINE_RENDER; draw=1; break; 
             case(XK_F3): XClearWindow(dpy,AnimWin); RenderMode=FLAT_RENDER; draw=1;  break;  
             case(XK_F5): for(ltmp = global_scene.Head; ltmp ; ltmp = ltmp->Brother )
                              FlipNormal(ltmp->Actual);
                          flip=1;
                          break;
             case(XK_F6): 
            }
          }
       else if( ev.type == ButtonPress && ev.xbutton.button == Button1 )
          {
           dragging = 1;
           pressx = ev.xbutton.x;
           pressy = ev.xbutton.y;
           appX = appY = 0.0;
          }
       else if( ev.type == ButtonRelease && ev.xbutton.button == Button1 )
          {
           dragging = 0;
          }
       else if( ev.type == MotionNotify && dragging )
          {
           angX = 90.0*(pressy-ev.xmotion.y)/(win_maxy/2.0);
           angY = 90.0*(ev.xmotion.x-pressx)/(win_maxx/2.0);
           mdx += angX-appX;
           mdy += angY-appY;
           appX = angX;
           appY = angY;
          }
       }
 
           if(mdx!=0.0 || mdy!=0.0)
              for(ltmp = global_scene.Head; ltmp ; ltmp = ltmp->Brother )
                  RotatePT3D((float *)ltmp->Actual->PTtable,ltmp->Actual->NBPT3D,mdx,mdy,0.0);
           if(mdx!=0.0 || mdy!=0.0) rot=1;

           if(move)
              for(ltmp = global_scene.Head; ltmp ; ltmp = ltmp->Brother )
                  MovePT3D((float *)ltmp->Actual->PTtable,ltmp->Actual->NBPT3D,dx,dy,dz);

           if( ax!=0.0 || ay!=0.0 || az!=0.0 )
              for(ltmp = global_scene.Head; ltmp ; ltmp = ltmp->Brother )
                  RotatePT3D((float *)ltmp->Actual->PTtable,ltmp->Actual->NBPT3D,ax,ay,az);

           if(rot || flip || draw || move || ax!=0.0 || ay!=0.0 || az!=0.0 ) 
             {
              XSetForeground(dpy,AnimGC,Black);
              XFillRectangle(dpy,AnimPix,AnimGC,0,0,win_maxx,win_maxy);
              XSetForeground(dpy,AnimGC,White);
              
              switch(RenderMode)
                    {
                     case(WIRE_RENDER):   
                          XSetForeground(dpy,gc,White);
                          for(ltmp = global_scene.Head; ltmp ; ltmp = ltmp->Brother )
                              Display3D(AnimPix,win_maxx,win_maxy,ltmp->Actual,FALSE);
                          XCopyArea(dpy,AnimPix,AnimWin,AnimGC,0,0,win_maxx,win_maxy,0,0);
                          break;
                     case(OUTLINE_RENDER):
                          RenderOutLine(AnimPix,AnimGC,win_maxx,win_maxy,0);
                          XCopyArea(dpy,AnimPix,AnimWin,AnimGC,0,0,win_maxx,win_maxy,0,0);
                          break;
                     case(FLAT_RENDER):
                          RenderSolid(im,win_maxx,win_maxy,Light);
                          XPutImage(dpy,AnimWin,AnimGC,im,0,0,0,0,win_maxx,win_maxy);
                          memset(im->data,Black,im->bytes_per_line*im->height);
                          break;      
                    }
             }
      XFlush(dpy);
     }   

 SetTextString(AnimMsg,"Quit Real-Time Animation");
}

void SetWire()
{
 RenderMode = WIRE_RENDER;
}

void SetOutLine()
{
 RenderMode = OUTLINE_RENDER;
}

void SetFlat()
{
 RenderMode = FLAT_RENDER;
}

/************************************/
/*            Anim_Init             */
/*----------------------------------*/
/* Build The graphic User Interface */
/*----------------------------------*/
/* Parameters:                      */
/* IN:  Title of The Window         */
/* OUT: NONE                        */
/*----------------------------------*/
/* Comments:                        */
/* Warning: We use shared Memory So */
/* Vreify If Your System Accept This*/
/************************************/

void Anim_Init(char *title)
{
 XWidget *entry;
 XWidget *render;
 XColor color;
 XSetWindowAttributes attrs; 
 XGCValues gcv;
 XRectangle rect[2];
 char *data;
 int i,pad;

/* Set The default Render Mode And Build The Shell */

 RenderMode = WIRE_RENDER;
 Light.X = 0.0;  
 Light.Y = maxy; 
 Light.Z =minz;

 AnimShell = CreateShell(&gui,title,0,0,win_maxx+120,win_maxy+50);
 
/* Install The "Global" Menu Button */
 entry = CreateButton(AnimShell,"Quit",10,10,75,20);
 AddCallBack(entry,QuitAnim);

 entry = CreateButton(AnimShell,"Real Time",10,30,75,20);
 AddCallBack(entry,RealTimeAnim);

 render = CreateMenu(AnimShell,"Mode",10,50,75,20);
 entry = AddMenuItem(render,"Wire");
 AddCallBack(entry,SetWire);
 entry = AddMenuItem(render,"OutLine");
 AddCallBack(entry,SetOutLine);
 entry = AddMenuItem(render,"Flat");
 AddCallBack(entry,SetFlat);
 entry = AddMenuItem(render,"Gouraud");
 entry = AddMenuItem(render,"Phong");

 AnimMsg = CreateConstText(AnimShell,"",100,win_maxy+20,win_maxx,20);
 
/* Now map the Interface */
 MapShell(AnimShell);
 MapAllWidget(AnimShell); 

 AnimWin = XCreateSimpleWindow(dpy,AnimShell->Win,100,10,win_maxx,win_maxy,2,White,Black);

 XSelectInput(dpy,AnimWin,KeyPress|ButtonReleaseMask|ButtonPress|PointerMotionMask);
 XMapWindow(dpy,AnimWin);

 AnimPix=XCreatePixmap(dpy,AnimWin,win_maxx,win_maxy,DefaultDepth(dpy,DefaultScreen(dpy)));
 XSetWindowBackground(dpy,AnimWin,Black);
 gcv.function = GXcopy;
 AnimGC = XCreateGC(dpy,AnimWin,GCFunction,&gcv);
 XSetForeground(dpy,AnimGC,White);

 rect[0].x = 1; rect[0].y= 1;
 rect[0].width = win_maxx-1; rect[0].height = win_maxy-1;
 XSetClipRectangles(dpy,AnimGC,0,0,rect,1,Unsorted);
 im =NULL;
 
#ifdef X_SHM
  shm_flag = XShmQueryExtension(dpy);
  if(shm_flag)
      completion_type = XShmGetEventBase(dpy) + ShmCompletion;

  if(shm_flag)
    {
     if(!( im = XShmCreateImage(dpy,None,DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)),
                                ZPixmap,NULL, &shminfo, win_maxx,win_maxy)))
        {
         SetTextString(msg,"Can Not Create Shared Image");
         DestroyShell(&gui,AnimShell);
         return; 
        }

      shminfo.shmid = shmget(IPC_PRIVATE, (im->bytes_per_line *
                                           im->height),IPC_CREAT | 0777);

      if(shminfo.shmid < 0)
        {
         shm_flag = 0;
         XDestroyImage(im);
         SetTextString(msg,"Can Not Create Shared Image");
         DestroyShell(&gui,AnimShell);
         return; 
        }

      shminfo.shmaddr = (char *)shmat(shminfo.shmid, 0, 0);

      if (shminfo.shmaddr == ((char *) -1))
        {
         SetTextString(msg,"Can Not Create Shared Image");
         DestroyShell(&gui,AnimShell);
         shm_flag = 0;
         return;  
        }

      im->data = shminfo.shmaddr;
      shminfo.readOnly = False;

      XShmAttach(dpy, &shminfo);
      XSync(dpy, False);

      if (xerror_flag)
        {
         XDestroyImage(im);
         shm_flag    = 0;
         xerror_flag = 0;
         SetTextString(msg,"Can Not Create Shared Image");
         DestroyShell(&gui,AnimShell);
         return;
        }

      shmctl(shminfo.shmid, IPC_RMID, 0);
    }
#else
 switch( DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)) )
        {
         case(8):  data = (unsigned char *)malloc(win_maxx*win_maxy);
                   memset(data,Black,win_maxx*win_maxy);
                   pad=8;
                   break;
         case(16): data = (unsigned char *)malloc(2*win_maxx*win_maxy);
                   pad=16;
                   break;
         case(24): data = (unsigned char *)calloc(4,win_maxx*win_maxy);
                   pad = 32;   /* ZPixmap stride is 4 bytes (pad=32), not 3 */
        }
 
 im = XCreateImage(gui.dpy, DefaultVisual(gui.dpy,DefaultScreen(gui.dpy)),
                   DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)),
                   ZPixmap, 0, data,win_maxx,win_maxy,pad,win_maxx*(pad/8));
#endif

 LoopShellEvent(&gui,AnimShell);
}


void VertexAnim()
{
 Anim_Init("VERTEX 3D Animation");
}


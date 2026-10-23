/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*            Vertex "Graphic User Interface" File      */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/

#include"VertexGUI.h"
#define d(x,y) ((dm[((x) & 7) + (((y) & 7) << 3)] & 0xff - 128) / 3)

extern SCENE3D global_scene;
extern GC gc;
extern XGUI gui;
extern XShell *toplevel;
extern XWidget *objlist,*msg;
extern unsigned long color[216];
extern Display *dpy;
extern int win_maxx,win_maxy;
extern unsigned long White,Black,Pink;

XShell *ColorShell;
XWidget *r,*g,*b;
XWidget *rt,*gt,*bt;
Window ColorWin;
GC ColorGC;
int cview;
OBJ3D *CObj;

unsigned long GetRGB24_8(int x,int y,unsigned char Red,unsigned char Green,unsigned char Blue)
{
 register int r, g, b, rr,gg,bb;
 register int luminence;
 short dm[] = 
 {
             0, 192,  48, 240,  12, 204,  60, 252,
             128,  64, 176, 112, 140,  76, 188, 124,
             32, 224,  16, 208,  44, 236,  28, 220,
             160,  96, 144,  80, 172, 108, 156,  92,
             8, 200,  56, 248,   4, 196,  52, 244,
             136,  72, 184, 120, 132,  68, 180, 116,
             40, 232,  24, 216,  36, 228,  20, 212,
             168, 104, 152,  88, 164, 100, 148,  84
             };

 rr = ((int)Red  )+ d(x,y); if (rr>255) rr=255; if (rr<0) rr=0;
 gg = ((int)Green)+ d(x,y); if (gg>255) gg=255; if (gg<0) gg=0;
 bb = ((int)Blue )+ d(x,y); if (bb>255) bb=255; if (bb<0) bb=0;

 r = (rr * 216) / (256 * 36);
 g = (gg * 216) / (256 * 36);
 b = (bb * 216) / (256 * 36);

 /* find the pixel we got */
 return(color[(r * 36) + (g * 6) + b]);
}


unsigned long GetRGB24_16(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
 return( (unsigned long )((unsigned long)(r>>3)<<11|
         (unsigned long)(g>>2)<<5|(unsigned long)(b>>3)) );   
}

unsigned long GetRGB24_24(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
 return( (unsigned long)((unsigned long)(r<<16) +
         (unsigned long)(g<<8) + (unsigned long)(b)) );
}

void InitRGBDisplay()
{
 Colormap cmap;
 XColor tmp;
 int i,r,g,b;

 if( DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)) == 8 )
   { 
    cmap = DefaultColormap(gui.dpy, DefaultScreen(gui.dpy));
    for(i = r = 0; r < 6; r++)
        for(g = 0; g < 6; g++)
            for(b = 0; b < 6; b++, i++)
               {
                tmp.red = (r * 36) << 8;
                tmp.green = (g * 36) << 8;
                tmp.blue = (b * 36) << 8;
                tmp.flags = DoRed | DoGreen | DoBlue;
                if(!XAllocColor(gui.dpy, cmap, &tmp))
                   goto NotEnough;
           
                color[i] = tmp.pixel;
               }

   return;

   NotEnough:
     fprintf(stderr,"Can't Only Get %d colors\n",i);
  }
 else
  return;
}

void QuitColor()
{
 XFreeGC(dpy,ColorGC);
 XDestroyWindow(dpy,ColorWin);
 DestroyShell(&gui,ColorShell);
}

void MoveRedScroll()
{
 float pos;
 char buff[4];

 GetScrollPos(r,&pos);
 pos *= 2.55;
 sprintf(buff,"%d",(int)pos);
 SetTextString(rt,buff);
}

void MoveGreenScroll()
{
 float pos;
 char buff[4];

 GetScrollPos(g,&pos);
 pos *= 2.55;
 sprintf(buff,"%d",(int)pos);
 SetTextString(gt,buff);
}

void MoveBlueScroll()
{
 float pos;
 char buff[4];

 GetScrollPos(b,&pos);
 pos *= 2.55;
 sprintf(buff,"%d",(int)pos);
 SetTextString(bt,buff);
}


void ViewXY()
{
 TREE_OBJ3D *ltmp;

 cview = PLAN_XY;
 XClearWindow(dpy,ColorWin);
 ChangeShellName(ColorShell,"VERTEX 3D Color Edition     Actual View: XY");
 XSetForeground(dpy,gc,White);
 DisplayXY(ColorWin,win_maxx,win_maxy,CObj,0);
 DisplayPoint(ColorWin,win_maxx,win_maxy,CObj,cview);
}


void ViewXZ()
{
 TREE_OBJ3D *ltmp;

 cview = PLAN_XZ;
 XClearWindow(dpy,ColorWin);
 ChangeShellName(ColorShell,"VERTEX 3D Color Edition     Actual View: XZ");
 XSetForeground(dpy,gc,White);
 DisplayXZ(ColorWin,win_maxx,win_maxy,CObj,0);
 DisplayPoint(ColorWin,win_maxx,win_maxy,CObj,cview);
}

void ViewZY()
{
 TREE_OBJ3D *ltmp;

 cview = PLAN_YZ;
 XClearWindow(dpy,ColorWin);
 ChangeShellName(ColorShell,"VERTEX 3D Color Edition     Actual View: ZY");
 XSetForeground(dpy,gc,White);
 DisplayZY(ColorWin,win_maxx,win_maxy,CObj,0);
 DisplayPoint(ColorWin,win_maxx,win_maxy,CObj,cview);
}

void AssignObject()
{
 float red,green,blue;
 char buff[4];
 int i;

 GetScrollPos(r,&red);
 red *= 2.55;
 GetScrollPos(g,&green);
 green *= 2.55;
 GetScrollPos(b,&blue);
 blue *= 2.55;

 for(i = 0;i<CObj->NBFACE;i++)
    {
     CObj->Color[i].Red = red;
     CObj->Color[i].Green = green;
     CObj->Color[i].Blue = blue;
    }
}

void AssignRandom()
{
 int i;

 srand(time(0));

 for(i = 0;i<CObj->NBFACE;i++)
    {
     CObj->Color[i].Red = rand()%256;
     CObj->Color[i].Green = rand()%256;
     CObj->Color[i].Blue = rand()%256;
    }
}


void AssignFace()
{
}

void VertexColor()
{
 XSetWindowAttributes attrs;
 XWidget *entry,*cmenu;
 XGCValues gcv;
 char *buff;
 int pl;

 if( (pl=GetActiveListItem(objlist,&buff)) == -1)
   {
    SetTextString(msg,"Choose An Object Color Edition");
    return;
   }

 CObj = FindNamedOBJ(buff,pl);
 
 ColorShell = CreateShell(&gui,"",0,0,win_maxx+170,win_maxy+50);
 ChangeShellName(ColorShell,"VERTEX 3D Color Edition     Actual View: XY");


 ColorWin = XCreateSimpleWindow(dpy,ColorShell->Win,150,10,
                              win_maxx,win_maxy,2,White,Black);

 attrs.backing_store = Always;
 XChangeWindowAttributes(dpy,ColorWin,CWBackingStore,&attrs);

 gcv.function = GXcopy;
 ColorGC = XCreateGC(dpy,ColorWin,GCFunction,&gcv);
 XSetForeground(dpy,ColorGC,White);
 
/* Install The "Global" Menu Button */
 entry = CreateButton(ColorShell,"Quit",40,10,70,20);
 AddCallBack(entry,QuitColor);

 cmenu = CreateMenu(ColorShell,"View",40,30,70,20);
 entry = AddMenuItem(cmenu,"XY");
 AddCallBack(entry,ViewXY);
 entry = AddMenuItem(cmenu,"XZ");
 AddCallBack(entry,ViewXZ);
 entry = AddMenuItem(cmenu,"ZY");
 AddCallBack(entry,ViewZY);

 cmenu = CreateMenu(ColorShell,"Assign",40,50,70,20);
 entry = AddMenuItem(cmenu,"Object");
 AddCallBack(entry,AssignObject);
 entry = AddMenuItem(cmenu,"Face");
 AddCallBack(entry,AssignFace);
 entry = AddMenuItem(cmenu,"Random");
 AddCallBack(entry,AssignRandom);
 
 entry = CreateLabel(ColorShell,"R",28,180,Black,FALSE);
 entry = CreateLabel(ColorShell,"G",68,180,Black,FALSE);
 entry = CreateLabel(ColorShell,"B",108,180,Black,FALSE);

 r = CreateScroll(ColorShell,30,200,10,120,VSCROLL);
 AddCallBack(r,MoveRedScroll);
 g = CreateScroll(ColorShell,70,200,10,120,VSCROLL);
 AddCallBack(g,MoveGreenScroll);
 b = CreateScroll(ColorShell,110,200,10,120,VSCROLL);
 AddCallBack(b,MoveBlueScroll);

 rt = CreateConstText(ColorShell,"0",20,330,30,20);
 gt = CreateConstText(ColorShell,"0",60,330,30,20);
 bt = CreateConstText(ColorShell,"0",100,330,30,20);

 MapShell(ColorShell);
 XMapWindow(dpy,ColorWin);
 MapAllWidget(ColorShell);

 ViewXY();
 
 LoopShellEvent(&gui,ColorShell);
}



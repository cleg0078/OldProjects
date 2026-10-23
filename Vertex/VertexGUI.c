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
#define MDepth(x) PlanesOfScreen(DefaultScreen((Display *)x))

extern XGUI gui;
extern XShell *toplevel;
extern XWidget *objlist,*msg;
extern Window win,rootwindow,iconwindow;
extern Window win3D,winXY,winXZ,winYZ;
extern GC gc,gcpix;
extern Display *dpy;
extern XGCValues gcv;
extern XEvent ev;
extern int ViewType,bpp;
extern int win_maxx,win_maxy,trimaxx,trimaxy;
extern float minz;
extern Colormap cmap;
extern int nbobj;
extern SCENE3D global_scene;
extern int ed_color;
extern unsigned long (*GetRGB24)(int,int,unsigned char,unsigned char,unsigned char);
extern void (*VertexFillFace)(XImage *,int,int,int,int,int,int,int,int,int);
extern void FillFace8(XImage *,int,int,int,int,int,int,int,int,int);
extern void FillFace16_24(XImage *,int,int,int,int,int,int,int,int,int);
extern unsigned long Black,White,Gray1,Gray2,Pink;


/* Private CallBack Function */
void VertexGUI_Close()
{
 XFreeGC(dpy,gc);
 XDestroyWindow(dpy,win);
 XDestroyWindow(dpy,win3D);
 XDestroyWindow(dpy,winXY);
 XDestroyWindow(dpy,winXZ);
 XDestroyWindow(dpy,winYZ); 
 DestroyShell(&gui,toplevel);
 XDeconnect(&gui);
 exit(0);
}

/************************************/
/*            Vertex_Init           */
/*----------------------------------*/
/* Build The graphic User Interface */
/*----------------------------------*/
/* Parameters:                      */
/* IN: argc --> "main" argc value   */
/*     argv --> "main" argv value   */
/*     width--> GUI width >600      */
/*     height-> GUI Height >400     */
/*----------------------------------*/
/* Comments:                        */
/* argc and argv are required for   */
/* X ToolKit XtInitialize function  */
/************************************/

void VertexInit(int width,int height,char *dpyname)
{
 XWidget *globalbutton,*delete,*edit;
 XWidget *createbutton,*viewbutton,*filebutton;
 XWidget *operbutton,*renderbutton,*camera,*light;
 XWidget *surfbutton;
 XWidget *entry;

 /*Pixmap pix; */
 XSetWindowAttributes attrs;
 XColor color[256];
 Colormap stdcmap;
 float uis;
 int nb,i;

 /* We Use only GUI Lib Functions so no comments! */

 if( XConnect(&gui,dpyname) == 0 )
    exit(0);

 /* per-axis auto-size: an explicit -w or -h is honored on its own;
    an axis left at 0 defaults to 2/3 of the physical screen */
 if( width == 0 )
     width  = DisplayWidth(gui.dpy,DefaultScreen(gui.dpy)) * 2 / 3;
 if( height == 0 )
     height = DisplayHeight(gui.dpy,DefaultScreen(gui.dpy)) * 2 / 3;
 width = (width<640)?640:width;
 height= (height<410)?410:height;

 toplevel = CreateShell(&gui,"Vertex 3D",0,0,width,height);
 nb = 0;

 /* Resolution-adaptive layout: every fixed 1995 coordinate below scales */
 uis = XGUIScreenScale(gui.dpy);
#define UIS(n) ((int)((n)*uis))

 win_maxx = width-UIS(100)-UIS(20);
 win_maxy = height-UIS(10)-UIS(40);

 trimaxx = win_maxx/2;
 trimaxy = win_maxy/2;
 
/* Install The "Global" Menu Button */
 globalbutton = CreateMenu(toplevel,"Global",UIS(10),UIS(10),UIS(75),UIS(20));
 entry = AddMenuItem(globalbutton,"Scene");
 AddCallBack(entry,VertexScene);
 entry = AddMenuItem(globalbutton,"Info");
 AddCallBack(entry,VertexInfo);

 if(ed_color)
   {
    entry = AddMenuItem(globalbutton,"Palette");
    AddCallBack(entry,VertexPalette);
   }

 entry = AddMenuItem(globalbutton,"About");
 entry = AddMenuItem(globalbutton,"Refresh");
 AddCallBack(entry,VertexReDisplay);
 entry = AddMenuItem(globalbutton,"Quit");
 AddCallBack(entry,VertexGUI_Close);

 /* Build the "File" menu Button */
 filebutton = CreateMenu(toplevel,"File",UIS(10),UIS(30),UIS(75),UIS(20));
 entry= AddMenuItem(filebutton,"Load File");
 AddCallBack(entry,VertexLoad);
 entry= AddMenuItem(filebutton,"Save Vertex");
 AddCallBack(entry,VertexSave);
 entry= AddMenuItem(filebutton,"Save ASCII");
 AddCallBack(entry,VertexSaveASCII);
 entry= AddMenuItem(filebutton,"Save 3DS");
 AddCallBack(entry,VertexSave3DS);
 entry= AddMenuItem(filebutton,"Save POV");
 AddCallBack(entry,VertexSavePOV);
 entry= AddMenuItem(filebutton,"New");
 AddCallBack(entry,VertexNEWAll);

 /* Build the "view" menu button */
 viewbutton = CreateMenu(toplevel,"View",UIS(10),UIS(50),UIS(75),UIS(20));
 entry = AddMenuItem(viewbutton,"TriView");
 AddCallBack(entry,VertexTriView);
 entry = AddMenuItem(viewbutton,"Global 3D");
 AddCallBack(entry,VertexDisplay3D);
 entry = AddMenuItem(viewbutton,"XY");
 AddCallBack(entry,VertexDisplayXY);
 entry = AddMenuItem(viewbutton,"XZ");
 AddCallBack(entry,VertexDisplayXZ);
 entry = AddMenuItem(viewbutton,"ZY");
 AddCallBack(entry,VertexDisplayZY); 


 /* Build The "Create" Menu */
 createbutton = CreateMenu(toplevel,"Objects",UIS(10),UIS(70),UIS(75),UIS(20));
 entry = AddMenuItem(createbutton,"Cube");
 AddCallBack(entry,VertexCreateCube);
 entry = AddMenuItem(createbutton,"Cone");
 AddCallBack(entry,VertexCreateCone);
 entry = AddMenuItem(createbutton,"Cylinder");
 AddCallBack(entry,VertexCreateCylinder);
 entry = AddMenuItem(createbutton,"Disk");
 AddCallBack(entry,VertexCreateDisk);
 entry = AddMenuItem(createbutton,"HemiSphere");
 AddCallBack(entry,VertexCreateHSphere);
 entry = AddMenuItem(createbutton,"Pyramid");
 AddCallBack(entry,VertexCreatePyramid);
 entry = AddMenuItem(createbutton,"Sphere");
 AddCallBack(entry,VertexCreateSphere);
 entry = AddMenuItem(createbutton,"Torus");
 AddCallBack(entry,VertexCreateTorus);
 entry = AddMenuItem(createbutton,"Mountain");
 AddCallBack(entry,VertexCreateMountain);
 entry = AddMenuItem(createbutton,"Revolution");
 AddCallBack(entry,VertexCreateRevol);
 entry = AddMenuItem(createbutton,"Surface");
 AddCallBack(entry,VertexCreateSurface);

 /* Build The Camera Menu */
 camera = CreateMenu(toplevel,"Camera",UIS(10),UIS(90),UIS(75),UIS(20));
 entry = AddMenuItem(camera,"Create");
 entry = AddMenuItem(camera,"Move");
 entry = AddMenuItem(camera,"FOV");

 /* Build The Light Menu */
 light = CreateMenu(toplevel,"Light",UIS(10),UIS(110),UIS(75),UIS(20));
 entry = AddMenuItem(light,"Create");
 entry = AddMenuItem(light,"Move");
 entry = AddMenuItem(light,"Parameters");

 entry=CreateButton(toplevel,"?????",UIS(10),UIS(130),UIS(75),UIS(20));
 AddCallBack(entry,NULL);


/* Build The "Edit " Button */
 edit = CreateButton(toplevel,"Edit",UIS(10),UIS(150),UIS(75),UIS(20));
 AddCallBack(edit,VertexEdit);


/* Build The "Operation: Buttons */
 operbutton = CreateMenu(toplevel,"Operation",UIS(10),UIS(170),UIS(75),UIS(20));
 entry = AddMenuItem(operbutton,"Move");
 AddCallBack(entry,VertexMove);
 entry = AddMenuItem(operbutton,"Zoom");
 AddCallBack(entry,VertexZoom);
 entry = AddMenuItem(operbutton,"Rotation");
 AddCallBack(entry,VertexRotate);
 entry = AddMenuItem(operbutton,"Free");
 AddCallBack(entry,VertexFreeOper);
 entry=AddMenuItem(operbutton,"Delete");
 AddCallBack(entry,VertexDelOBJ);
 entry = AddMenuItem(operbutton,"Duplicate");
 AddCallBack(entry,VertexDuplicate);

/* Build The "Surface" Menu (Mapping Functions) */
 surfbutton =  CreateMenu(toplevel,"Surface",UIS(10),UIS(190),UIS(75),UIS(20));
 entry = AddMenuItem(surfbutton,"Color");
 AddCallBack(entry,VertexColor);
 entry = AddMenuItem(surfbutton,"Mapping");
 entry = AddMenuItem(surfbutton,"Material");
 AddCallBack(entry,VertexRenderMat);
 entry = AddMenuItem(surfbutton,"Normal Unify");
 AddCallBack(entry,VertexNormalUnify);
 entry = AddMenuItem(surfbutton,"Flip Normal");
 AddCallBack(entry,VertexFlipNormal);
 entry = AddMenuItem(surfbutton,"View Normal");
 AddCallBack(entry,VertexDisplayNormal);

/* Build The "Render Menu" */
 renderbutton = CreateMenu(toplevel,"Render",UIS(10),UIS(210),UIS(75),UIS(20));
 entry = AddMenuItem(renderbutton,"OutLine");
 AddCallBack(entry,VertexRenderOutLine);
 entry = AddMenuItem(renderbutton,"Flat");
 AddCallBack(entry,VertexRenderSolid);
 entry = AddMenuItem(renderbutton,"Gouraud");
 AddCallBack(entry,VertexRenderGouraud);   /* never wired in the 1995 tree */
 entry = AddMenuItem(renderbutton,"Phong");
 AddCallBack(entry,VertexRenderPhong);
 entry = AddMenuItem(renderbutton,"Ray Tracing");


 entry = CreateButton(toplevel,"Anim",UIS(10),UIS(230),UIS(75),UIS(20));
 AddCallBack(entry,VertexAnim);

/* Build The object List */ 
 objlist=CreateList(toplevel,UIS(10),UIS(280),UIS(75),UIS(120));

 /* build The Error Message Box */ 
 msg = CreateConstText(toplevel,"",UIS(100),height-UIS(30),width-UIS(120),UIS(20));
 
/* Now map the Interface */
 MapShell(toplevel);
 MapAllWidget(toplevel); 

 /* Build Up the output objects window */ 
 dpy = toplevel->dpy;
 rootwindow = toplevel->Win;

 White = WhitePixel(dpy,DefaultScreen(dpy));
 Black = BlackPixel(dpy,DefaultScreen(dpy));
 Gray1 = toplevel->SHCOLOR;
 Gray2 = toplevel->SHCOLOR;

 /* Init The 24 Bits Display */

 switch(DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)))
       {
        case(24):
             GetRGB24 = &GetRGB24_24;
             VertexFillFace = &FillFace16_24;
             bpp=3;
             break;
        case(15):
        case(16):
             GetRGB24 = &GetRGB24_16;
             VertexFillFace = &FillFace16_24;
             bpp=2;
             break;
        case(8):
             GetRGB24 = &GetRGB24_8;
             VertexFillFace = &FillFace8;
             bpp=1;
       }

 win = XCreateSimpleWindow(dpy,rootwindow,UIS(100),UIS(10),win_maxx,win_maxy,2,White,Black);
 attrs.backing_store = Always;
 XChangeWindowAttributes(dpy,win,CWBackingStore,&attrs); 

 winXY = XCreateSimpleWindow(dpy,rootwindow,UIS(100),UIS(10),trimaxx,trimaxy,2,White,Black);
 attrs.backing_store = Always;
 XChangeWindowAttributes(dpy,winXY,CWBackingStore,&attrs);

 winXZ = XCreateSimpleWindow(dpy,rootwindow,UIS(100)+trimaxx,UIS(10),trimaxx,trimaxy,2,White,Black);
 attrs.backing_store = Always;
 XChangeWindowAttributes(dpy,winXZ,CWBackingStore,&attrs);

 winYZ = XCreateSimpleWindow(dpy,rootwindow,UIS(100),UIS(10)+trimaxy,trimaxx,trimaxy,2,White,Black);
 attrs.backing_store = Always;
 XChangeWindowAttributes(dpy,winYZ,CWBackingStore,&attrs);

 win3D = XCreateSimpleWindow(dpy,rootwindow,UIS(100)+trimaxx,UIS(10)+trimaxy,trimaxx,trimaxy,2,White,Black);
 attrs.backing_store = Always;
 XChangeWindowAttributes(dpy,win3D,CWBackingStore,&attrs);

 XSelectInput(dpy,win,KeyPress|ButtonReleaseMask|ButtonPress|PointerMotionMask);
 XSelectInput(dpy,win3D,KeyPress|ButtonReleaseMask|ButtonPress|PointerMotionMask);
 XSelectInput(dpy,winXY,KeyPress|ButtonReleaseMask|ButtonPress|PointerMotionMask);
 XSelectInput(dpy,winXZ,KeyPress|ButtonReleaseMask|ButtonPress|PointerMotionMask);
 XSelectInput(dpy,winYZ,KeyPress|ButtonReleaseMask|ButtonPress|PointerMotionMask);

 XMapWindow(dpy,win);

 if(ed_color)
   {
    cmap=XCreateColormap(dpy,rootwindow,DefaultVisual(dpy,DefaultScreen(dpy)),AllocAll);
    stdcmap = DefaultColormap(dpy,DefaultScreen(dpy));
    for(i=0;i<16;i++)
       {
	color[i].pixel = i;
	color[i].flags = DoRed|DoGreen|DoBlue;
       }

    XQueryColors(dpy,stdcmap,color,16);
    XStoreColors(dpy,cmap,color,16);
   }

 XSetWindowBackground(dpy,win,Black);
 gcv.function = GXcopy;
 gc = XCreateGC(dpy,rootwindow,GCFunction,&gcv);
 XSetForeground(dpy,gc,White);

/*
 iconwindow = XCreateSimpleWindow(dpy,RootWindowOfScreen(XtScreen(toplevel)),0,0,140,100,0,0,1);
 XResizeWindow(dpy, iconwindow,120 ,200);
 pix = XCreatePixmapFromBitmapData(dpy,iconwindow,cam_bits,cam_width,cam_height,0,1,4);
 if(pix == 0 )
    printf("No PixMap Created....\n");
 XSetWindowBackgroundPixmap(dpy,iconwindow,pix);

*/

 XStoreName(dpy,toplevel->Win,"VERTEX 3D     Actual View: GLOBAL 3D");

 InitRGBDisplay();
 Pink = GetRGB24(0,0,255,128,128);


 LoopShellEvent(&gui,toplevel);
}


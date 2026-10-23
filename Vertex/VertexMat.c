/*********************************************/
/*           Vertex  Material Module         */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*                 C.LEGENDRE                */
/*********************************************/

#include<math.h>
#include<X11/Xlib.h>
#include<X11/IntrinsicP.h>
#include<X11/Shell.h>
#include<StringDefs.h>
#include"Connect.h"
#include"VRStruct.h"

extern XGUI gui;
extern Window RenderWin;
extern GC gc,RenderGC;
extern unsigned long Black,White;
extern void (*AddPixel)(int,int,unsigned long,unsigned long,unsigned long);
extern void RayTrace(char *,int);
extern void MakeMapping(OBJ *,char *);
extern void AddPixel8(int,int,unsigned long,unsigned long,unsigned long);
extern void AddPixel16(int,int,unsigned long,unsigned long,unsigned long);
extern void AddPixel24(int,int,unsigned long,unsigned long,unsigned long);
extern OBJ obj[MAX_OBJ];
extern LIGHT light[MAX_LIGHT];
extern int nobj,nosph,nosq;
extern int nlight,bgflag;
extern int xres,yres,maxrec_level;
extern VDBL fov;
extern VECTOR eye,eye_dir,up,right;
extern unsigned long (*GetRGB24)(int,int,unsigned char,unsigned char,unsigned char );

XShell *MatShell,*sh;
XWidget *R,*G,*B,*H,*L,*S;
XWidget *ambient,*diffuse,*specular;
XWidget *refrac,*ind,*wid,*map,*mapname;
XWidget *indexval,*widthval;
XWidget *rend,*texture;
int mat_depth,active;
unsigned long rcolor[3];
XColor rval[3];

void CloseMat()
{
 XDestroyWindow(gui.dpy,RenderWin);
 DestroyShell(&gui,MatShell);
}

XColor GetRGB(int v)
{
 return(rval[v]);
}

void SetRGBPos(int v)
{
 XColor tmp=GetRGB(v);

 SetScrollPos(R,100.0*tmp.red/65535.0);
 SetScrollPos(G,100.0*tmp.green/65535.0);
 SetScrollPos(B,100.0*tmp.blue/65535.0);
}

void ResetDS()
{
 SetRGBPos(0); 
 ActiveToggle(ambient);
 UnActiveToggle(diffuse);
 UnActiveToggle(specular);
 active=1;
}

void ResetAS()
{
 SetRGBPos(1);
 ActiveToggle(diffuse);
 UnActiveToggle(ambient);
 UnActiveToggle(specular);
 active=2;
}

void ResetAD()
{
 SetRGBPos(2);
 ActiveToggle(specular);
 UnActiveToggle(ambient);
 UnActiveToggle(diffuse);
 active=3;
}


void DrawADS()
{
 Foreground(MatShell,rcolor[0]);
 Fillrect(MatShell,85,140,62,22);
 Foreground(MatShell,rcolor[1]);
 Fillrect(MatShell,225,140,62,22);
 Foreground(MatShell,rcolor[2]);
 Fillrect(MatShell,365,140,62,22);
}

void MoveRGB()
{
 float posr,posg,posb;

 GetScrollPos(R,&posr);
 GetScrollPos(G,&posg);
 GetScrollPos(B,&posb);

 rval[active-1].red = (posr * 65535 /100.0);
 rval[active-1].green = (posg * 65535 /100.0);
 rval[active-1].blue = (posb * 65535 /100.0);

 switch( mat_depth )
   {
    case(8):
        rcolor[active-1] = GetRGB24(0,0,(posr * 255/100.0),(posg * 255/100.0),(posb * 255/100.0));
        break;
    case(16):
        rcolor[active-1] =(unsigned long )((unsigned long)(((int)posr*256/100)>>3)<<11|
                          (unsigned long)((int)posg*256/100.0)<<5|
                          (unsigned long)((int)posb*256/100.0)>>3);
        break;
    case(24):
        rcolor[active-1] = (((unsigned long)(posr * 65535 /100)/256)<<16)
                          |(((((unsigned long)(posg * 65535 /100)/256))<<8))
                          |((unsigned long)(posb * 65535 /100)/256);
        /* '+' outranked '<<' here, shifting by (16+green); fixed 2026 */
   }   

 DrawADS();
}

void GetTexture()
{
 char *n;

 /* dialog scales like the File/Open-Save requests (bands inside scale by s) */
 n = FileRequest(&gui,"Select Texture",0,0,
                 (int)(320*XGUIScreenScale(gui.dpy)),(int)(200*XGUIScreenScale(gui.dpy)));

 if( n == NULL)
    SetTextString(texture,"None");
 else
    SetTextString(texture,n);
}

void RenderMat()
{
 XColor tmp[3];
 float pos;
 char *t;
 int x;

 xres=yres=120;
 maxrec_level=3;
 nobj=1;

 switch( mat_depth )
    {
     case( 8 ):
         for(x=0; x<3 ; x++ )
            {
             tmp[x].pixel = rcolor[x];
             tmp[x].flags = DoRed|DoGreen|DoBlue;
            }

         XQueryColors(gui.dpy,DefaultColormap(gui.dpy,DefaultScreen(gui.dpy)),tmp,3);
         break;
        case(16):
        case(24): 
          tmp[0] = GetRGB(0); tmp[1]=GetRGB(1); tmp[2]=GetRGB(2);         
    }
 
 fov=50.0;
 fov = tan( fov * M_PI / 180 ) / sqrt(2.0);

 light[0].org.X = 2.5; light[0].org.Y = -0.5; light[0].org.Z = -2.0;
 light[0].angle = 1.0; light[0].angle *= M_PI/180.0;
 /* the per-light shading loop (2026-10) needs a count and neutral
    defaults: one white light keeps the preview pixel-identical */
 light[0].Color.R = light[0].Color.G = light[0].Color.B = 1.0;
 light[0].Intensity = 1.0;
 nlight = 1;
 
 eye.X = 2.5; eye.Y = -0.5; eye.Z = -2.0;
 eye_dir.X = -1.0; eye_dir.Y = 0.0; eye_dir.Z=1.0;
 up.X = 0.0; up.Y = 1.0; up.Z = 0.4;

 obj[0].Type = SPHERE_OBJ;
 obj[0].Mapping = 0;
 obj[0].data.sphere.Center.X = -0.4;
 obj[0].data.sphere.Center.Y = -0.5;
 obj[0].data.sphere.Center.Z = 1.3;
 obj[0].data.sphere.Radius = 1.67332;  /* retuned: the buggy intersect drew at sqrt(Radius) (was 2.8) */
 obj[0].ambient.R = tmp[0].red/65535.0;
 obj[0].ambient.G = tmp[0].green/65535.0;
 obj[0].ambient.B = tmp[0].blue/65535.0;
 obj[0].diffuse.R = tmp[1].red/65535.0;
 obj[0].diffuse.G = tmp[1].green/65535.0;
 obj[0].diffuse.B = tmp[1].blue/65535.0;
 obj[0].specular.R = tmp[2].red/65535.0;
 obj[0].specular.G = tmp[2].green/65535.0;
 obj[0].specular.B = tmp[2].blue/65535.0;
 obj[0].reflection = 1.0*M_PI/180;
 obj[0].refraction = 0.0;
 GetScrollPos(ind,&pos);
 obj[0].obindex = pos + 0.1;
 GetScrollPos(wid,&pos);
 obj[0].width = pos + 0.1;
 GetScrollPos(refrac,&pos);
 if( pos != 0.0) 
    bgflag = aX;
 else
    bgflag = NONE;
 obj[0].refraction = pos/100.0;
 obj[0].refr_color.R = obj[0].refr_color.G = obj[0].refr_color.B = pos/100.0;
 
 t = GetTextString(texture);
 if( strcmp(t,"None") != 0 )
   {
    obj[0].Mapping = 1;
    MakeMapping(&obj[0],t);
   }

 sh = (XShell *)malloc(sizeof(XShell));
 sh->Win = RenderWin;
 sh->gc = MatShell->gc;
 sh->dpy = gui.dpy;

 RenderGC = MatShell->gc;
 RayTrace(NULL,0);
 free(sh);
}
 
void VertexRenderMat()
{
 XGCValues gcv;
 XWidget *entry;
 XSetWindowAttributes attrs;
 XColor tmp[256];
 int x;

 MatShell = CreateShell(&gui,"VERTEX 3D Material",0,0,450,350);

 RenderWin = XCreateSimpleWindow(gui.dpy,MatShell->Win,10,10,120,120,2,White,Black);
 
 switch( DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)) )
        {
         case(8):  
              rcolor[0] = rcolor[1] = rcolor[2] = GetRGB24(0,0,0,0,0); 
              AddPixel = &AddPixel8;
              mat_depth=8;
              break;
         case(15):
         case(16):
              mat_depth=16;
              AddPixel = &AddPixel16;
              rcolor[0] = rcolor[1] = rcolor[2] = 0;  
            
              break;
         case(24):
              rcolor[0] = rcolor[1] = rcolor[2] = 0;
              AddPixel = &AddPixel24;
              mat_depth=24;
              break;
        }

 rval[0].red = rval[0].green = rval[0].blue = 0;
 rval[1].red = rval[1].green = rval[1].blue = 0;
 rval[2].red = rval[2].green = rval[2].blue = 0;
     
 attrs.backing_store = Always;
 XChangeWindowAttributes(gui.dpy,RenderWin,CWBackingStore,&attrs);

 entry = CreateLabel(MatShell,"Shininess:",150,10,Black,FALSE);
 ind = CreateScroll(MatShell,270,10,120,15,HSCROLL);
 entry = CreateLabel(MatShell,"Shin. Strength:",150,30,Black,FALSE);
 wid = CreateScroll(MatShell,270,30,120,15,HSCROLL);
 entry = CreateLabel(MatShell,"Transparency:",150,50,Black,FALSE);
 refrac = CreateScroll(MatShell,270,50,120,15,HSCROLL);

 ambient = CreateToggle(MatShell,"Ambient",10,140,70,20);
 AddCallBack(ambient,ResetDS);
 diffuse = CreateToggle(MatShell,"Diffuse",150,140,70,20);
 AddCallBack(diffuse,ResetAS);
 specular= CreateToggle(MatShell,"Specular",290,140,70,20);
 AddCallBack(specular,ResetAD);

 entry = CreateLabel(MatShell,"R",10,180,Black,FALSE);
 R = CreateScroll(MatShell,40,180,120,15,HSCROLL);
 AddCallBack(R,MoveRGB);
 entry = CreateLabel(MatShell,"G",10,205,Black,FALSE);
 G = CreateScroll(MatShell,40,205,120,15,HSCROLL);
 AddCallBack(G,MoveRGB);
 entry = CreateLabel(MatShell,"B",10,230,Black,FALSE);
 B = CreateScroll(MatShell,40,230,120,15,HSCROLL);
 AddCallBack(B,MoveRGB);

 entry = CreateLabel(MatShell,"H",220,180,Black,FALSE);
 H = CreateScroll(MatShell,250,180,120,15,HSCROLL);
 entry = CreateLabel(MatShell,"L",220,205,Black,FALSE);
 L = CreateScroll(MatShell,250,205,120,15,HSCROLL);
 entry = CreateLabel(MatShell,"S",220,230,Black,FALSE);
 S = CreateScroll(MatShell,250,230,120,15,HSCROLL);

 entry = CreateButton(MatShell,"Texture",40,260,70,20);
 AddCallBack(entry,GetTexture);
 texture = CreateText(MatShell,"None",112,260,100,20);
 
 rend = CreateButton(MatShell,"Render",370,320,70,20);
 AddCallBack(rend,RenderMat);
 entry = CreateButton(MatShell,"Quit",10,320,70,20);
 AddCallBack(entry,CloseMat);

 MapShell(MatShell);  
 MapAllWidget(MatShell);
 XMapWindow(gui.dpy,RenderWin);

 ResetDS();
 DrawADS();

 LoopShellEvent(&gui,MatShell);
}

 

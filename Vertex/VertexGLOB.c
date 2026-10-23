/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                    Vertex Global File                */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexOBJ.h"
#include"VertexDisplay.h"
#include"Connect.h"
#include"stdio.h"

extern SCENE3D global_scene;
extern XGUI gui;
extern XShell *toplevel;
extern XWidget *objlist,*msg;;
extern Window win,rootwindow;
extern GC gc;
extern Display *dpy;
extern int ViewType;
extern Colormap cmap;
extern Display * dpy;
extern int ed_color;
extern float minx,miny,minz,maxx,maxy,maxz;
extern float screen_dist;
extern unsigned long Black;

/* Palette Editor data */
XShell *cmshell;
XWidget *cmr,*cmg,*cmb,*tr,*tg,*tb;
XWidget *colorwid[16];   /* we Limit a 16 colors palette: we don't use more */
int actual;

/* Global Scene Editor Data */
XShell *gsshell;
XWidget *gsmaxx,*gsmaxy,*gsmaxz,*gsminx,*gsminy,*gsminz,*gsdist;

/* "About" Dialog Box Data */
XShell *ashell;

/* Info Shell */
XShell *ishell;

void VertexCMEnd()
{
 DestroyShell(&gui,cmshell);
}

/*
void ResetScrollBar_Text()
{
 char buff[12];
 XColor col;

 col.pixel = actual;
 col.flags = DoRed|DoGreen|DoBlue;
 XQueryColor(dpy,cmap,&col);

 sprintf(buff,"%3d",col.red>>8);
 XtVaSetValues(tr,XtNstring,buff,NULL);
 sprintf(buff,"%3d",col.green>>8);
 XtVaSetValues(tg,XtNstring,buff,NULL);
 sprintf(buff,"%3d",col.blue>>8);
 XtVaSetValues(tb,XtNstring,buff,NULL);

 XawScrollbarSetThumb(cmr,(FLOAT)(col.red/65535.0),255.0);
 XawScrollbarSetThumb(cmg,(FLOAT)(col.green/65535.0),255.0);
 XawScrollbarSetThumb(cmb,(FLOAT)(col.blue/65535.0),255.0);
}

void CMMove(Widget w,int *info,float *data)
{
 XColor col;

 col.pixel = actual;
 col.flags = DoRed|DoGreen|DoBlue;
 XQueryColor(dpy,cmap,&col);

 if(w==cmr)
   col.red =  *data * 65535;
 else
   if(w==cmg)
      col.green = *data * 65535;
   else
      if(w==cmb)
         col.blue = *data * 65535;

 XStoreColor(dpy,cmap,&col);
 ResetScrollBar_Text();
}

void CMOtherCol(Widget w)
{
 int i;
 for(i=0;i<16;i++)
     if(w==colorwid[i]) 
       {
         actual = i; break;
        }
 ResetScrollBar_Text();
}

void VertexColorMap()
{
 Widget ok,cancel;
 char buff[256];
 int info,i,x,y;

 if(ed_color==0)
   {
    strcpy(buff,"NO COLORMAP EDITION");
    XStoreName(dpy,rootwindow,buff);
    return;
   }
    
 cmshell = XtVaCreatePopupShell("Vertex Color Edition",transientShellWidgetClass,toplevel,NULL);
 cmform = XtVaCreateManagedWidget("cmform",formWidgetClass,cmshell,
                                   XtNwidth,300,XtNheight,200,
                                   XtNbackground,5,NULL);
 ok = XtVaCreateWidget("OK",commandWidgetClass,cmform,XtNx,10,XtNy,160,
                       XtNheight,20,XtNwidth,70,XtNbackground,2,NULL);
 XtAddCallback(ok,XtNcallback,VertexCMEnd,NULL);
 cancel = XtVaCreateWidget("CANCEL",commandWidgetClass,cmform,XtNx,220,
                           XtNy,160,XtNheight,20,XtNwidth,70,XtNbackground,2
                           ,NULL);
 XtAddCallback(cancel,XtNcallback,VertexCMEnd,NULL);
 
 cmr = XtVaCreateWidget("SRED",scrollbarWidgetClass,cmform,XtNbackground,2,
                        XtNx,20,XtNy,10,XtNheight,100,XtNwidth,10,NULL);
 XtAddCallback(cmr,XtNjumpProc,(XtCallbackProc)CMMove,(XtPointer)&info);
 cmg = XtVaCreateWidget("SGREEN",scrollbarWidgetClass,cmform,XtNbackground,2,
                        XtNx,60,XtNy,10,XtNheight,100,XtNwidth,10,NULL);
 XtAddCallback(cmg,XtNjumpProc,(XtCallbackProc)CMMove,(XtPointer)&info);
 cmb = XtVaCreateWidget("SBLUE",scrollbarWidgetClass,cmform,XtNbackground,2,
                        XtNx,100,XtNy,10,XtNheight,100,XtNwidth,10,NULL);
 XtAddCallback(cmb,XtNjumpProc,(XtCallbackProc)CMMove,(XtPointer)&info);
 tr = XtVaCreateWidget("TR",asciiTextWidgetClass,cmform,XtNbackground,2,
                       XtNx,10,XtNy,120,XtNwidth,30,
                       XtNeditType, XawtextEdit,XtNwrap,XawtextWrapNever,
                       XtNlength, 5,NULL);
 tg = XtVaCreateWidget("TR",asciiTextWidgetClass,cmform,XtNbackground,2,
                       XtNx,50,XtNy,120,XtNwidth,30,
                       XtNeditType, XawtextEdit,XtNwrap,XawtextWrapNever,
                       XtNlength, 5,NULL);
 tb = XtVaCreateWidget("TG",asciiTextWidgetClass,cmform,XtNbackground,2,
                       XtNx,90,XtNy,120,XtNwidth,30,
                       XtNeditType, XawtextEdit,XtNwrap,XawtextWrapNever,
                       XtNlength,5,NULL);
 
 for(i=0,y=10;y<110;y+=25)
    for(x=130;x<290;x+=40)
       {
        colorwid[i] = XtVaCreateWidget("",commandWidgetClass,cmform,XtNx,x,XtNy,y,
                                  XtNwidth,40,XtNheight,25,XtNbackground,i,NULL);
        XtAddCallback(colorwid[i++],XtNcallback,(XtCallbackProc)CMOtherCol,NULL);
       }
 XtPopup(cmshell,XtGrabExclusive);
 XtMapWidget(ok); XtMapWidget(cancel);
 XtMapWidget(cmr); XtMapWidget(cmg); XtMapWidget(cmb);
 XtMapWidget(tr); XtMapWidget(tg); XtMapWidget(tb);

 for(i=0;i<16;i++)
    XtMapWidget(colorwid[i]);

 actual =0;

 ResetScrollBar_Text();
}
*/

void SceneCancel()
{
 DestroyShell(&gui,gsshell);
}

void SceneOK()
{
 float mx,my,mz,mix,miy,miz,d;

 mx = VertexATOF(GetTextString(gsmaxx));
 my = VertexATOF(GetTextString(gsmaxy));
 mz = VertexATOF(GetTextString(gsmaxz));

 mix = VertexATOF(GetTextString(gsminx));
 miy = VertexATOF(GetTextString(gsminy));
 miz = VertexATOF(GetTextString(gsminz));

 d = VertexATOF(GetTextString(gsdist)); 
 
 if( mx>mix && my>miy && mz>miz && d>0.0 )
   {
    maxx = mx; minx = mix;
    maxy = my; miny = miy;
    maxz = mz; minz = miz;
    screen_dist = d;
    SceneCancel();
    VertexReDisplay();
   }
 else
   return;   /* Invalid Parameters */

} 

void VertexScene()
{
 XWidget *entry;
 char buff[50];
 char name[]="Vertex Scene Parameters";

 gsshell = CreateShell(&gui,name,0,0,380,150);
 entry = CreateButton(gsshell,"Ok",10,110,70,20);
 AddCallBack(entry,SceneOK);
 
 entry = CreateButton(gsshell,"Cancel",300,110,70,20);
 AddCallBack(entry,SceneCancel);
 entry = CreateLabel(gsshell,"Max X Y Z",10,10,Black,FALSE);

 sprintf(buff,"%.2f",maxx); 
 gsmaxx = CreateText(gsshell,buff,110,10,70,20);
 sprintf(buff,"%.2f",maxy);
 gsmaxy = CreateText(gsshell,buff,200,10,70,20);
 sprintf(buff,"%.2f",maxz);
 gsmaxz = CreateText(gsshell,buff,290,10,70,20);

 entry = CreateLabel(gsshell,"Min X Y Z",10,40,Black,FALSE);
 sprintf(buff,"%.2f",minx);
 gsminx = CreateText(gsshell,buff,110,40,70,20);
 sprintf(buff,"%.2f",miny);
 gsminy = CreateText(gsshell,buff,200,40,70,20);
 sprintf(buff,"%.2f",minz);
 gsminz = CreateText(gsshell,buff,290,40,70,20);


 entry = CreateLabel(gsshell,"Screen Dist.",10,70,Black,FALSE);
 sprintf(buff,"%d",(int)screen_dist);
 gsdist = CreateText(gsshell,buff,110,70,70,20);

 MapShell(gsshell);
 MapAllWidget(gsshell);
 LoopShellEvent(&gui,gsshell);
}

void VertexInfoEnd()
{
 DestroyShell(&gui,ishell);
}
 
void VertexInfo()
{
 XWidget *entry;
 int nbobj,nbpt,nbfc;
 char buff[80];
 TREE_OBJ3D *ltmp;

 nbpt=nbfc=nbobj=0;
 
 for(ltmp = global_scene.Head; ltmp ; ltmp = ltmp->Brother )
    {
     nbobj++;
     nbfc += ltmp->Actual->NBFACE;
     nbpt += ltmp->Actual->NBPT3D;
    }
    
 ishell = CreateShell(&gui,"Vertex Scene Info",0,0,210,120);
 sprintf(buff,"Number Of Object%s : %d",nbobj>1?"s":"",nbobj);
 entry = CreateLabel(ishell,buff,10,10,Black,FALSE);
 sprintf(buff,"Number Of Point%s : %d",nbpt>1?"s":"",nbpt);
 entry = CreateLabel(ishell,buff,10,30,Black,FALSE);
 sprintf(buff,"Number Of Face%s : %d",nbfc>1?"s":"",nbfc);
 entry = CreateLabel(ishell,buff,10,50,Black,FALSE);

 entry = CreateButton(ishell,"Ok",70,90,70,20);
 AddCallBack(entry,VertexInfoEnd);

 MapShell(ishell);
 MapAllWidget(ishell);
 LoopShellEvent(&gui,ishell);
} 


void VertexPalette()
{
 CreatePalette(&gui,"Vertex Palette",0,0);
}

 

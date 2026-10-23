/********************************************************/
/*             Graphic User Interface Librairie         */
/*                         V1.2                         */
/*------------------------------------------------------*/
/* Concept And Design: C.LEGENDRE  April 1996           */
/*------------------------------------------------------*/
/* Freely use. Spread The Disk and Not The Word!!!      */
/* But Remember: This is My Job...                      */
/********************************************************/

#ifndef __XCONNECT_H
#define __XCONNECT_H
#include"XGUI.h"

/* The "Widget XGUI" Functions */
extern void DestroyXWidget(XWidget *);
extern int XConnect(XGUI *,const char *);
extern void XDeconnect(XGUI *);
extern XShell *CreateShell(XGUI *,const char *,int,int,int,int);
extern XShell *CreateWindow(XShell *,int,int,int,int);
extern void DestroyShell(XGUI *,XShell *);
extern void MapShell(XShell *);
extern void ChangeShellName(XShell *,const char *);
extern void EnableDoubleBuffer(XShell *);
extern void FlipBuffer(XShell *);
extern XWidget *CreateButton(XShell *,const char *,int,int,int,int);
extern void SetIcon(XWidget *,char **);
extern XWidget *CreateRepeater(XShell *,const char *,int,int,int,int);
extern void MapWidget(XWidget *);
extern void MapAllWidget(XShell *);
extern void LoopEvent(XShell *);
extern void AddCallBack(XWidget *,void fct());
extern XWidget *CreateMenu(XShell *,const char *,int,int,int,int);
extern XWidget *AddMenuItem(XWidget *,const char *); 
extern XWidget *CreateScroll(XShell*,int,int,int,int,int);
extern XWidget *CreateVHScroll(XShell *,Display *,Window,int,int,int,int,int);
extern XWidget *CreateGToggle(Display *,Window,char *,int,int,int,int,int,int,unsigned long,unsigned long,unsigned long,unsigned long);
extern void GetScrollPos(XWidget *,float *);
extern void SetScrollPos(XWidget *,float);
extern void ScrollStep(XScrollWidget *,int);
extern XWidget *CreateToggle(XShell *,const char *,int,int,int,int);
extern void ActiveToggle(XWidget *);
extern void UnActiveToggle(XWidget *);
extern int GetToggleState(XWidget *);
extern XWidget *CreateList(XShell *,int,int,int,int);
extern XWidget *AddListItem(XWidget *,const char *);
extern int ListOwnsWindow(XListWidget *,Window);
extern void LoopShellEvent(XGUI *,XShell *);
extern XWidget *CreateLabel(XShell *,const char *,int,int,int,int);
extern XWidget *CreateText(XShell *,const char *,int,int,int,int);
extern XWidget *CreateConstText(XShell *,const char *,int,int,int,int);
extern void SetTextString(XWidget *,const char *);
extern char *GetTextString(XWidget *);
extern int GetActiveListItem(XWidget *,char **);
extern int ActiveListItem(XWidget *,char *);
extern int ActivePosListItem(XWidget *,int);
extern int UnActiveListItem(XWidget *,char *);
extern void DestroyListItem(XWidget *,int );
extern void DestroyAllListItem(XWidget *);
extern void ListScrollVis(XWidget *);
extern void ListScrollReset(XWidget *);
extern char *FileRequest(XGUI *,const char *,int,int,int,int);
extern void CreatePalette(XGUI *,const char *,int,int);
extern XWidget *CreateCheckBox(XShell *,const char *,int,int,int,int,int);
extern void ActiveCheckBox(XWidget *);
extern void UnActiveCheckBox(XWidget *);
extern int GetCheckBoxState(XWidget *);


/* The "XGUI Drawing" functions */
extern char WaitKey(XShell *); 
extern char WaitEscape(XShell *);
extern char GetKey(XShell *);
extern int PressMouseButton(XShell *,int *,int *);
extern int ReleaseMouseButton(XShell *,int *,int *);
extern void ClearShell(XShell *); 
extern void Background(XShell *,int);
extern void Foreground(XShell *,int);
extern void Putpixel(XShell *,int,int);
extern void GetPixel(XShell *,int,int,unsigned char *,unsigned char *,unsigned char *); 
extern void Line(XShell *,int,int,int,int);
extern void Circle(XShell *,int,int,int);
extern void Arc(XShell *,int,int,int,int,int,int);
extern void Rectangle(XShell *,int,int,int,int);
extern void Fillrect(XShell *,int,int,int,int);
extern int  GetMouseXY(XShell *,int *,int *);
extern void Fillcircle(XShell *,int,int,int);
extern void Fillarc(XShell *sh,int,int,int,int,int);
extern void SetLinewidth(XShell *,int);
extern void SetDrawmode(XShell *,int);
extern void Text(XShell *,int,int,const char *);

/*
extern colormap CreateColormap(XShell *);
extern void SetShellColormap(XShell *,colormap);
extern int ChangeColor(XShell *,colormap,unsigned char,unsigned char ,unsigned char);
extern int GetColor(XShell *,colormap,unsigned char *,unsigned char *,unsigned char *);
*/
#endif

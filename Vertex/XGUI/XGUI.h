/********************************************************/
/*             Graphic User Interface Librairie         */
/*                         V1.2                         */
/*------------------------------------------------------*/
/* Concept And Design: C.LEGENDRE  July 1996            */
/*------------------------------------------------------*/
/* Freely use. Spread The Disk and Not The Word!!!      */
/* But Remember: This is My Job...                      */
/********************************************************/    

#ifndef __XGUI_H
#define __XGUI_H
#include<X11/X.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/Xos.h>
#include<X11/Xatom.h>
#include<X11/keysym.h>
#include<X11/xpm.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#define DSCR(a) DefaultScreen(a)
#define DDEPTH(a) DefaultDepth(a,DSCR(a))
#define GetBlack(a) BlackPixel(a->dpy,DSCR(a))
#define GetWhite(a) WhitePixel(a->dpy,DSCR(a))
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/* Adaptive Sytem Variables */
#define DFONT "r14"     /* Change This For The DEC-ALPHA or Other Stations */
#define MAXWIDGET 300
#define MAXSHELL 20
#define MAXTEXT 80

/* Resolution-adaptive UI (see Connect.c): scale factor from the physical
   screen height, and the shared UI font picked at that scale. */
extern float XGUIScreenScale(Display *dpy);
extern XFontStruct *XGUIGetFont(Display *dpy);

/* GC Mode */
#define COPY GXcopy
#define AND GXand
#define OR GXor
#define XOR GXxor
#define INVERT GXinvert
#define CLEAR GXclear

/*Widget Tags: Don't Touch */
#define ENDTAGS        0X0000L
#define BORDERCOLOR    0X0001L
#define FORECOLOR      0X0002L
#define BACKCOLOR      0X0003L
#define SHELLCOLOR     0X0004L
#define WIDTH          0X0005L
#define HEIGHT         0X0006L
#define BPIXMAP        0X0007L
#define FPIXMAP        0X0008L
#define CALLBACK       0X0009L
#define HOTKEY         0X000AL
#define ACTIVE         0x000BL
#define NOTACTIVE      0x000CL
#define TITLE          0x000DL
#define TEXTPOS        0x000EL
#define HSCROLL        0x000FL
#define VSCROLL        0x0010L

/* Widget Type :Don't Touch*/
#define COMMAND        0x0000L
#define REPEATER       0X0001L
#define LIST           0X0002L
#define VIEWPORT       0X0003L
#define FILELIST       0x0004L
#define SCROLLBAR      0x0005L
#define CHECKBOX       0x0006L
#define MENU           0x0007L
#define TEXT           0x0008L
#define TOGGLE         0x0009L
#define LABEL          0x000AL

/* EXTRA TAG: Don't Touch */
#define LEFT           0x0000L
#define RIGHT          0x0001L
#define CENTER         0x0002L
#define BELOW          0x0003L
#define ABOVE          0x0004L

/* Global Structures: Don't Touch */
typedef struct xanywidget
{
 short Type;
 Display *dpy;
 Window Win;
 GC gc;
 Pixmap Fore,Back,Border;
 XFontStruct *Font;
 int Width,Height;
 short HotKey;
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
}XAnyWidget;

typedef struct xbuttonwidget
{
 int Type;
 Display *dpy;
 Window Win;
 GC gc;
 Pixmap Fore,Back,Border;
 XFontStruct *Font;
 int Width,Height;
 short HotKey;
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
 char String[260];
 void (*CallBack)();
 void (*Resize)(struct xbuttonwidget *);
 void (*Press)(struct xbuttonwidget *);
}XButtonWidget;

typedef XButtonWidget XRepeatWidget;
 
typedef struct xtogglewidget
{
 short Type;
 Display *dpy;
 Window Win;
 GC gc;
 Pixmap Fore,Back,Border;
 XFontStruct *Font;
 int Width,Height;
 short HotKey;
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
 char String[260];
 int State;      /* Activate Or Not */
 int WBorder;      /* Usefull For The List Type */
 int TextPlace;
 void (*CallBack)();
 void (*Resize)(struct xtogglewidget *);
 void (*Press)(struct xtogglewidget *);
}XToggleWidget;

typedef struct xmenuwidget
{
 short Type;
 Display *dpy;
 Window Win;
 GC gc;
 Pixmap Fore,Back,Border;
 XFontStruct *Font;
 int Width,Height;
 short HotKey;
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
 char *String;
 Window MenuWin;
 int Maxx,Maxy,Posx,Posy;
 int NextFree;
 XButtonWidget *List[20];   /* Maximun: 20 Menu */ 
 void (*CallBack)();
 void (*Resize)(struct xmenuwidget *);
 void (*Press)(struct xmenuwidget *);
}XMenuWidget;

typedef struct xscrollwidget
{
 short Type;
 Display *dpy;
 Window Win;
 GC gc;
 Pixmap Fore,Back,Border;
 XFontStruct *Font;
 int Width,Height;
 short HotKey;
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
 short Orientation;
 Window ScrollWin;
 int Act,ActRoot;
 float Min,Max,Pos;
 void *Owner;                 /* Back-link for composite widgets (List) */
 void (*CallBack)();
 void (*Press)(struct xscrollwidget *);
}XScrollWidget;
 
typedef struct xtextwidget
{
 short Type;
 Display *dpy;
 Window Win;
 GC gc;
 Pixmap Fore,Back,Border;
 XFontStruct *Font;
 int Width,Height;
 short HotKey;
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
 char *String;
 int Actual;
 void (*CallBack)();
 void (*Redraw)(struct xtextwidget *);
 void (*Press)(struct xtextwidget *);
}XTextWidget;

typedef struct xlabelwidget
{
 short Type;
 Display *dpy;
 Window Win;
 GC gc;
 Pixmap Fore,Back,Border;
 XFontStruct *Font;
 int Width,Height;
 short HotKey;
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
 char String[260];
}XLabelWidget;

typedef struct xlistwidget
{
 short Type;
 Display *dpy;
 Window Win;
 GC gc;
 Pixmap Fore,Back,Border;
 XFontStruct *Font;
 int Width,Height;
 short HotKey;
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
 XScrollWidget *scroll;
 XToggleWidget *List[200];
 int NextFree;
 int Maxx,Maxy;
 void (*CallBack)();
 void (*Press)(struct xlistwidget *,Window);
}XListWidget;

typedef struct xcheckwidget
{
 short Type;
 Display *dpy;
 Window Win;
 GC gc;
 Pixmap Fore,Back,Border;
 XFontStruct *Font;
 int Width,Height;
 short HotKey;
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
 char String[260];
 int State;      /* Activate Or Not */
 int TextPlace;
 int Px,Py;
 Window parent;
 void (*CallBack)();
 void (*Resize)(struct xcheckwidget *);
 void (*Press)(struct xcheckwidget *);
}XCheckBoxWidget;

typedef union 
{
 XAnyWidget any;
 XButtonWidget button;
 XRepeatWidget repeater;
 XMenuWidget menu;
 XScrollWidget scroll;
 XTextWidget text;
 XToggleWidget toggle;
 XListWidget list;
 XLabelWidget label;
 XCheckBoxWidget check;
}XWidget; 

typedef struct xshell
{
 Display *dpy;
 GC gc;
 Window Win;
 Colormap cmap;
 XWidget *List[MAXWIDGET];
 int NextFree;
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
 int destroyed;
 int activeLoops;
 Pixmap BackBuffer;   /* Off-screen pixmap for double buffering */
 int Width;           /* Window width */
 int Height;          /* Window height */
}XShell;

typedef struct xgui
{
 Display *dpy;
 XShell *List[MAXSHELL];
 unsigned long FGCOLOR,BGCOLOR,BOCOLOR,SHCOLOR;
}XGUI;

#endif


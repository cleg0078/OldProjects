#include "Connect.h"
#include "stdio.h"

XGUI xgui;
XShell *xshell,*win;

void Quit(void)
{
DestroyShell(&xgui,xshell);
XDeconnect(&xgui);
}

void ClearWindow()
{
 ClearShell(win);
}

void InsertCell()
{
 int x,y;

 while( PressMouseButton(win,&x,&y) != 1 );
     
 Foreground(win,GetWhite(win));
 Fillcircle(win,x-10,y-10,20);
}


void JoinCell()
{
 int x1,y1,x2,y2;

 /* Repeat Until The Left Mouse is Not Pressed */
 while( PressMouseButton(win,&x1,&y1) != 1 );

 while( PressMouseButton(win,&x2,&y2) != 1 );

 Foreground(win,GetWhite(win));
 Line(win,x1,y1,x2,y2);
}   

void main()
{
XWidget *boutton,*menu;

if(XConnect(&xgui,"")==0)
  exit(0);

xshell=CreateShell(&xgui,"Simple Graph Draw By C.LEGENDRE",0,0,600,400);
menu=CreateMenu(xshell,"File",10,10,70,20);
boutton=AddMenuItem(menu,"Clear");
AddCallBack(boutton,ClearWindow);
boutton=AddMenuItem(menu,"Insert");
AddCallBack(boutton,InsertCell);
boutton=AddMenuItem(menu,"Join");
AddCallBack(boutton,JoinCell);

boutton=AddMenuItem(menu,"Quit");
AddCallBack(boutton,Quit);

MapShell(xshell);
win = CreateWindow(xshell,100,10,490,380);
MapAllWidget(xshell);
LoopShellEvent(&xgui,xshell);
}


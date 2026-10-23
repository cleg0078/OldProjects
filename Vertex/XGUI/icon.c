#include "Connect.h"
#include "stdio.h"
#include"ArcTriomphe.xpm"

XGUI xgui;
XShell *xshell;
XWidget *bouton;

void Quit(void)
{
DestroyShell(&xgui,xshell);
XDeconnect(&xgui);
}


void place()
{
 SetIcon(bouton,ArcTriomphe);
 printf("PLACE\n");
}

void main()
{

if(XConnect(&xgui,"")==0)
  exit(0);

xshell=CreateShell(&xgui,"Simple Graph Draw By C.LEGENDRE",0,0,600,400);
bouton = CreateButton(xshell,"Quit",10,10,480,220);
AddCallBack(bouton,place);

MapShell(xshell);
MapAllWidget(xshell);
LoopShellEvent(&xgui,xshell);
}


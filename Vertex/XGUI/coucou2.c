#include "Connect.h"
#include "stdio.h"

XGUI xgui;
XShell *xshell;
XWidget *scroll1,*scroll2,*texte,*liste;

void delete(void)
{
DestroyListItem(liste,0);
}

void saisie(void)
{
char *buffer;
buffer=GetTextString(texte);
AddListItem(liste,buffer);
}

void affichepos(void)
{
float pos;
GetScrollPos(scroll1,&pos);
fprintf(stdout,"%f\n",pos);
SetScrollPos(scroll2,pos);
}

void affichepos2(void)
{
float pos;
GetScrollPos(scroll2,&pos);
fprintf(stdout,"%f\n",pos);
}

void quitter(void)
{
DestroyShell(&xgui,xshell);
XDeconnect(&xgui);
}

void hello(void)
{
fprintf(stdout,"Bonjour\n");
}

void main()
{
XWidget *boutton,*menu;
if(XConnect(&xgui,"")==0)
  exit(0);

xshell=CreateShell(&xgui,"Coucou",0,0,440,400);
menu=CreateMenu(xshell,"Hello",10,10,70,20);
boutton=AddMenuItem(menu,"Bonjour");
AddCallBack(boutton,hello);
boutton=AddMenuItem(menu,"Delete");
AddCallBack(boutton,delete);

boutton=CreateButton(xshell,"Quit",10,30,70,20);
AddCallBack(boutton,quitter);

scroll1=CreateScroll(xshell,10,80,50,50,HSCROLL);
AddCallBack(scroll1,affichepos);
scroll2=CreateScroll(xshell,280,10,60,120,VSCROLL);
AddCallBack(scroll2,affichepos2);

texte=CreateText(xshell,"Texte",100,10,110,20);

boutton=CreateButton(xshell,"Add",10,50,70,20);
AddCallBack(boutton,saisie);

/*
liste=CreateList(xshell,250,300,150,100);
*/

MapShell(xshell);
MapAllWidget(xshell);
LoopShellEvent(&xgui,xshell);
}


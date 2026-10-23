#include<stdio.h>
#include<stdlib.h>
#include"Connect.h"

XShell *sh;
static XGUI gui;
XWidget *xs1,*xs2;
XWidget *cb;

void Quit()
{
 DestroyShell(&gui,sh);
 XDeconnect(&gui);
 exit(0);
}

void DrawPixel()
{
 XShell *tsh;
 int i,x,y,b;

 tsh = CreateShell(&gui,"XGUI PutPixel By C.LEGENDRE",0,0,320,200);
 EnableDoubleBuffer(tsh);  /* Enable back buffer */
 MapShell(tsh);

 srand(time(0));

 for(i=0 ; i<4000 ; i++ )
    {
     x=rand()%320;
     y=rand()%200;
     Foreground(tsh,rand()%8); 
     Putpixel(tsh,x,y);
    }

 Foreground(tsh,0); 
 Text(tsh,20,20,"Press First Mouse Button To Exit");
 
 FlipBuffer(tsh);  /* Display the drawing */
 
 while( PressMouseButton(tsh,&x,&y) != 1 );
 DestroyShell(&gui,tsh);
}

void GetFile()
{
 char *buff = FileRequest(&gui,"XGUI File Request",0,0,320,200); 
 
 if( buff )
    puts(buff);
}

void DrawLine()
{
 XShell *tsh;
 int i,x1,y1,x2,y2;

 tsh = CreateShell(&gui,"XGUI Line By C.LEGENDRE",0,0,320,200);
 EnableDoubleBuffer(tsh);  /* Enable back buffer */
 MapShell(tsh);

 srand(time(0));

 for(i=0 ; i<10000 ; i++ )
    {
     x1=rand()%320; y1=rand()%200;
     x2=rand()%320; y2=rand()%200;
     Foreground(tsh,rand()%8);
     Line(tsh,x1,y1,x2,y2);
    }

 Foreground(tsh,0);
 Text(tsh,20,20,"Press A Key To Exit");
 
 FlipBuffer(tsh);  /* Display the drawing */
 
 WaitKey(tsh);
 DestroyShell(&gui,tsh);
 UnActiveCheckBox(cb);
}

void DrawRectangle()
{
 XShell *tsh;
 int i,x1,y1,x2,y2;

 tsh = CreateShell(&gui,"XGUI Rectangle By C.LEGENDRE",0,0,320,200);
 EnableDoubleBuffer(tsh);  /* Enable back buffer */
 MapShell(tsh);
 
 srand(time(0));

 for(i=0 ; i<1000 ; i++ )
    {
     x1=rand()%320; y1=rand()%200;
     x2=rand()%320; y2=rand()%200;
     Foreground(tsh,rand()%8);
     Rectangle(tsh,x1,y1,x2,y2);
    }

 Foreground(tsh,0);
 Text(tsh,20,20,"Press A Key To Exit");
 
 FlipBuffer(tsh);  /* Display the drawing */
 
 WaitKey(tsh);
 DestroyShell(&gui,tsh);
}

void DrawCircle()
{
 XShell *tsh;
 int i,x1,y1,x2,y2;

 tsh = CreateShell(&gui,"XGUI Circle By C.LEGENDRE",0,0,320,200);
 EnableDoubleBuffer(tsh);  /* Enable back buffer */
 MapShell(tsh);
 
 srand(time(0));

 for(i=0 ; i<1000 ; i++ )
    {
     x1=rand()%320; y1=rand()%200;
     x2=rand()%60;
     Foreground(tsh,rand()%8);
     Circle(tsh,x1,y1,x2);
    }

 Foreground(tsh,0);
 Text(tsh,10,10,"Press A Key To Exit");
 
 FlipBuffer(tsh);  /* Display the drawing */
 
 WaitKey(tsh);
 DestroyShell(&gui,tsh);
}

void GetPalette()
{
 /* Don't work yet.... */
 CreatePalette(&gui,"Palette Editor",320,200);
}



void main()
{
 XWidget *entry,*DrawMenu,*list;
 int i,x1,y1,x2,y2;
 char buff[256];

 if( XConnect(&gui,"") == 0 )
    exit(0);

 sh = CreateShell(&gui,"XGUI Demo By C.LEGENDRE ",0,0,320,220);

 entry = CreateButton(sh,"Quit",10,10,70,20);
 AddCallBack(entry,Quit);
 
 DrawMenu = CreateMenu(sh,"Draw",10,32,70,20);
 entry = AddMenuItem(DrawMenu,"Pixel");
 AddCallBack(entry,DrawPixel);
 entry = AddMenuItem(DrawMenu,"Rectangle");
 AddCallBack(entry,DrawRectangle);
 entry = AddMenuItem(DrawMenu,"Circle");
 AddCallBack(entry,DrawCircle);
 entry = AddMenuItem(DrawMenu,"Line");
 AddCallBack(entry,DrawLine);
 
 entry = CreateButton(sh,"File",10,54,70,20);
 AddCallBack(entry,GetFile);

 entry = CreateScroll(sh,90,10,15,140,VSCROLL);
 entry = CreateScroll(sh,120,10,15,140,VSCROLL);
 entry = CreateScroll(sh,90,160,140,15,HSCROLL);

 entry = CreateToggle(sh,"Toggle",10,76,70,20);

/*
 entry = CreateButton(sh,"Palette",10,98,70,20);
 AddCallBack(entry,GetPalette);
*/

 entry = CreateText(sh,"Text",150,10,70,20);
/*
 list = CreateList(sh,150,40,70,80);
 entry = AddListItem(list,"Liste1");
 entry = AddListItem(list,"Liste2");
 entry = AddListItem(list,"Liste3");
 entry = AddListItem(list,"Liste4");
 entry = AddListItem(list,"Liste5");
 entry = AddListItem(list,"Liste6");
*/

 cb = CreateCheckBox(sh,"CheckBox",10,170,30,30,RIGHT);

 MapShell(sh);
 MapAllWidget(sh);
 LoopShellEvent(&gui,sh);
 exit(0);
}

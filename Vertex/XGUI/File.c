/********************************************************/
/*             Graphic User Interface Librairie         */
/*                         V1.2                         */
/*------------------------------------------------------*/
/* Concept And Design: C.LEGENDRE  April 1996           */
/*------------------------------------------------------*/
/* Freely use. Spread The Disk and Not The Word!!!      */
/* But Remember: This is My Job...                      */
/********************************************************/

#include"XGUI.h"
#include"Connect.h"
#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/utsname.h>

XShell *fsh;
XGUI *file_intgui;
static XWidget *text,*list;
static char r[255];

void FileOK()
{
 strncpy(r,text->text.String,254);
 r[254]=0;
 DestroyShell(file_intgui,fsh);
}

void FileCancel()
{
 memset(r,0,255);
 DestroyShell(file_intgui,fsh);
}


int EnterDirItem(const char *item)
{
 size_t len;
 char name[256];

 if( item == NULL )
    return(0);

 len = strlen(item);
 if( len < 3 || item[0] != '<' || item[len-1] != '>' )
    return(0);

 len -= 2;
 if( len >= sizeof(name) )
    len = sizeof(name)-1;

 strncpy(name,item+1,len);
 name[len] = 0;

 return( chdir(name) == 0 ) ? 1 : 0;
}

void BuildList()
{
 DIR *dirfile;
 struct dirent *direntry;
 struct stat buff;
 char tmp[260];

 dirfile = opendir(".");
 if( dirfile == NULL )
    return;

 AddListItem(list,"<..>");

 while( (direntry = readdir(dirfile) ) != NULL)
      {
       if( strcmp(direntry->d_name,".") == 0 ||
           strcmp(direntry->d_name,"..") == 0 )
          continue;

       if( stat(direntry->d_name,&buff) == -1 )
          buff.st_mode = 0;

       if( (buff.st_mode & 0x7000 ) == 0x4000 )
         {
          snprintf(tmp,sizeof(tmp),"<%s>",direntry->d_name);
          AddListItem(list,tmp);
         }
       else
         AddListItem(list,direntry->d_name);
      }

 closedir(dirfile);
}

void ChangeDir()
{
 int pl;
 char *tmp;
 char dirname[256];

 pl=GetActiveListItem(list,&tmp);

 dirname[0]=0;
 if( pl >-1 && tmp != NULL)
    {
     strncpy(dirname, tmp, 255);
     dirname[255]=0;
    }

 if( dirname[0] == 0 )
    return;

 if( EnterDirItem(dirname) )
    {
     DestroyAllListItem(list);
     BuildList();
    }
 else
   if( dirname[0] != '<' )
      SetTextString(text,dirname);
}
 
char *FileRequest(XGUI *gui,const char *txt,int x,int y,int tx,int ty)
{
 XWidget *ok,*cancel;
 int i;
 float s = XGUIScreenScale(gui->dpy);   /* fixed 1995 band heights scale too */
 int bh = (int)(20*s), b30 = (int)(30*s), b70 = (int)(70*s);

 file_intgui = gui;

 fsh = CreateShell(gui,txt,x,y,tx,ty);
 if( fsh == NULL )
    return(NULL);   /* a failed dialog reads as a Cancel (fixed 2026) */
 ok = CreateButton(fsh,"Ok",10,ty-b30,tx/4,bh);
 AddCallBack(ok,FileOK);
 cancel = CreateButton(fsh,"Cancel",tx-tx/4-10,ty-b30,tx/4,bh);
 AddCallBack(cancel,FileCancel);
 list = CreateList(fsh,10,10,tx-20,ty-b70);
 AddCallBack(list,ChangeDir);
 text = CreateText(fsh,"",10,10+ty-b70,tx-20,bh);

 XMapWindow(fsh->dpy,fsh->Win); 
 for(i=0;i<fsh->NextFree ; i++)
     MapWidget(fsh->List[i]);

 BuildList();
 LoopShellEvent(gui,fsh);
 if( strlen(r) == 0 )
    return(NULL);
 else
    return(r);
} 

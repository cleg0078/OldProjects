/********************************************************/
/*  Headless unit test for FileRequest folder logic     */
/*  Exercises EnterDirItem() (File.c) without X11.      */
/********************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

extern int EnterDirItem(const char *item);

static int fails = 0;
static int checks = 0;
static char rootcwd[512];

static void ck(int cond,const char *msg)
{
 checks++;
 if(cond)
    printf("PASS: %s\n",msg);
 else
   {
    fails++;
    printf("FAIL: %s\n",msg);
   }
}

static int cwd_ends_with(const char *tail)
{
 char buf[512];
 size_t n,t;
 if( getcwd(buf,sizeof(buf)) == NULL )
    return 0;
 n = strlen(buf);
 t = strlen(tail);
 return( n >= t && strcmp(buf + n - t, tail) == 0 );
}

static int cwd_is_root(void)
{
 char buf[512];
 return( getcwd(buf,sizeof(buf)) != NULL && strcmp(buf,rootcwd) == 0 );
}

int main(void)
{
 char root[512], sub[512];
 char longname[64];
 char item[128];
 int i;

 snprintf(root,sizeof(root),"/tmp/xgui_filetest_%d",(int)getpid());
 snprintf(sub,sizeof(sub),"%s/sub",root);

 if( mkdir(root,0755) != 0 )
    { perror("mkdir root"); return(1); }
 if( mkdir(sub,0755) != 0 )
    { perror("mkdir sub"); return(1); }
 if( chdir(root) != 0 )
    { perror("chdir root"); return(1); }
 snprintf(rootcwd,sizeof(rootcwd),"%s",root);

 /* --- parse-level rejections: not a folder marker --- */
 ck( EnterDirItem(NULL) == 0, "NULL rejected" );
 ck( EnterDirItem("") == 0, "empty string rejected" );
 ck( EnterDirItem("plainfile.txt") == 0, "plain file rejected" );
 ck( EnterDirItem("<>") == 0, "empty marker <> rejected" );
 ck( EnterDirItem("<") == 0, "lone < rejected" );
 ck( EnterDirItem("<a") == 0, "unterminated <a rejected" );
 ck( EnterDirItem("no>") == 0, "trailing > without < rejected" );
 ck( EnterDirItem("a<b>c") == 0, "inner angle brackets not a marker" );
 ck( cwd_is_root(), "cwd unchanged after rejections" );

 /* --- positive navigation --- */
 ck( EnterDirItem("<sub>") == 1, "enter <sub> succeeds" );
 ck( cwd_ends_with("/sub"), "cwd is now sub" );

 /* --- failure leaves cwd intact (C2) --- */
 ck( EnterDirItem("<nope>") == 0, "missing folder fails" );
 ck( cwd_ends_with("/sub"), "cwd still sub after failed enter" );

 /* --- parent traversal --- */
 ck( EnterDirItem("<..>") == 1, "enter <..> succeeds" );
 ck( cwd_is_root(), "cwd is root again" );

 /* --- truncated item (Toggle String[40] cut, no closing >) fails safely --- */
 memset(longname,'L',sizeof(longname));
 longname[sizeof(longname)-1]=0;
 longname[60]=0;
 if( mkdir(longname,0755) != 0 )
    { perror("mkdir longname"); return(1); }
 /* simulate the 39-char widget-string truncation: '<' + 38 chars, no '>' */
 item[0]='<';
 for(i=0;i<38;i++) item[1+i]='L';
 item[39]=0;
 ck( EnterDirItem(item) == 0, "truncated item (no closing >) rejected" );
 ck( cwd_is_root(), "cwd unchanged after truncated item" );

 /* --- well-formed 60-char marker resolves --- */
 item[0]='<';
 for(i=0;i<60;i++) item[1+i]='L';
 item[61]='>';
 item[62]=0;
 ck( EnterDirItem(item) == 1, "60-char marker enters folder" );
 ck( EnterDirItem("<..>") == 1, "back to root" );
 ck( cwd_is_root(), "cwd is root after 60-char round trip" );

 /* --- marker longer than internal name buffer: fails, cwd kept --- */
 {
  char big[600];
  big[0]='<';
  for(i=0;i<590;i++) big[1+i]='M';
  big[591]='>';
  big[592]=0;
  EnterDirItem(big);
  ck( cwd_is_root(), "cwd kept on overlong (590-char) marker" );
 }

 rmdir(longname);
 rmdir(sub);
 rmdir(root);

 printf("\n%d checks, %d failures\n",checks,fails);
 return(fails?1:0);
}

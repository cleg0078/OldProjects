/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*               Vertex "Main Function" File            */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"Vertex.h"
#include"Connect.h"
#define VUSAGE "[ -color -h number  -w number -display Name]"


#ifdef __ALPHA
float VertexATOF(char *buff)
{
 return ( (float)atof(buff) );
}
#else
float VertexATOF(char *buf)
{
 char *tmp = buf;
 char res[20]={0};
 int i,j,frac,seen;
 float den;

 for(frac=seen=j=0; *tmp ; tmp++)
    if( *tmp != '.' )
       {
        if( j < 19 )        /* keep room for the NUL in res[20] (I-12) */
          {
           res[j++] = *tmp;
           if( seen )
              frac++;       /* digits counted after the decimal point */
          }
       }
    else
       seen = 1;

 den = 1.0;
 for(i=0;i<frac;i++)
     den*=10.0;

 return ( atoi(res)/den );
}
#endif

int main(int argc,char **argv)
{
 int i,parent;
 char *dpyname=NULL; 
 /* Verify The Parameters And Set Default*/

 ed_color= 0;    /* default: No ColoMap Edition */

/* Size Of The Display Window (0 = auto-size from the display, VertexInit) */
 win_maxx = 0;
 win_maxy = 0;

 for(i=1;i<argc;i++)
     if(strcmp("-h",argv[i]) == 0)
        {
         if( ++i >= argc )   /* option without its value (I-13) */
            { fprintf(stderr,"USAGE %s %s\n",argv[0],VUSAGE); exit(0); }
         win_maxy = atoi(argv[i]);
        }
     else
        if(strcmp("-w",argv[i]) == 0)
           {
            if( ++i >= argc )
               { fprintf(stderr,"USAGE %s %s\n",argv[0],VUSAGE); exit(0); }
            win_maxx= atoi(argv[i]);
           }
        else
          if(strcmp("-color",argv[i]) == 0 )
             ed_color = 1;
           else 
             if(strcmp("-display",argv[i]) == 0 )
               {
                if( ++i >= argc )
                   { fprintf(stderr,"USAGE %s %s\n",argv[0],VUSAGE); exit(0); }
                dpyname = argv[i];
               }
             else
               { 
                fprintf(stderr,"USAGE %s %s\n",argv[0],VUSAGE);
	        exit(0); 
               }

/* I Want A Mono View */
 TriView = FALSE;

/* Initialize global data <-- We Have No Object */
 nbobj = nbdisk = nbcube = nbpyra = nbsphere = nbrevol = 0;
 nbsurf = nbmount = nbcyl = nbcone = nbtorus = 0;

/* Set The World Size */
 maxx=maxy=maxz = 1000.0;
 minx=miny= -1000.0;  
 minz=-2000.0;

/* And The Screen Distance <-- See Vertex.h */
 screen_dist = 350.0;

global_scene.Head = global_scene.Tail = NULL;

/* Accelerator Buffer For Rendering */
sgl = (XSegment *)malloc(600*sizeof(XSegment));
tbuff = 600;

if( sgl == 0 )
  { 
   fprintf(stderr,"No Memory For SpeedUp Buffer\n");
   exit(0);
  }
 
/* Build The New Process: Launch the Multi-Task System */

/* parent = fork();
 if( parent > 0 )
    exit(0);
*/

/* And Now Build The "Graphic User Interface" */

 VertexInit(win_maxx,win_maxy,dpyname);

 return 0;
}


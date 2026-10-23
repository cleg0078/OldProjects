#include"VRay.h"
#include<math.h>

int main(int nb,char **argv)
{
 fprintf(stderr,"Sizeof OBJ=%lu\n",(unsigned long)sizeof(OBJ));

 if( nb < 5 )
   {
    fprintf(stderr,"Usage: %s <RECURSION LEVEL> <RESX> <RESY> <FILE>\n",argv[0]);
    exit(0);
   }

 maxrec_level=atoi(argv[1]);
 xres=atoi(argv[2]);
 yres=atoi(argv[3]);
 
 ParseFile(argv[4]);

 /* Standalone owns the display lifecycle, like the embedded caller
    (VertexMat.c) does: InitPlot first, pure render with internal=0,
    then wait and tear down. Calling RayTrace with internal!=0 used to
    ClosePlot() (XDeconnect) before returning, so WaitKeyBoard() below
    ran XNextEvent on a closed Display -> SIGSEGV in _XReadEvents. */
 if( InitPlot(xres,yres) == 0 )
   {
    fprintf(stderr,"No Display\n");
    exit(1);
   }

 RayTrace("pop.tga",0);

 WaitKeyBoard();
 ClosePlot();

 exit(0);
}


/* 
 * This is the actual ray-tracing part
 */

#include"VRStruct.h"
#include"Vector.h"
#include"Display.h"
#include<stdio.h>
#include<math.h>
/*
#define __DEBUG 1
*/

extern COLOR Trace(RAY r,int n);
extern INTERSECT Intersect(RAY r);
extern COLOR Shade(INTERSECT i,RAY r,int n);
extern COLOR Bgcolor(RAY r);
extern int display,verbose,xres,yres;
extern VDBL fov;
extern VECTOR eye,eye_dir,up,right;
extern Display *dpy;
extern Window RenderWin;
extern GC gc;
extern unsigned long (*GetRGB24)(int,int,unsigned char,unsigned char,unsigned char);

RAY SampleRay(RAY r,VDBL theta)
{
 VECTOR c;	/* directional cosines */
 RAY r2;

 c = r.dir;
 r2.pos = r.pos;
 r2.dir = norm( c );
 r2.theta = 0;
 r2.obj = r.obj;
 return r2;
}

/*
 * trace a single strait ray
 */

COLOR TraceRay(RAY r,int n)
{
 INTERSECT inter;
 COLOR col;
 VDBL m;

/* check for intersection with the object */
inter = Intersect(r);

/* if no intersection, return some background color */
if( inter.obj == NULL )
   return Bgcolor(r);

/* Calculate the shading function there */

 col = Shade(inter, r, n);

/* if the color > 1, that means that the components are too big. Normalize. */

 m = col.R;
 if( col.G > m )
    m = col.G;
 
 if( col.B > m )
    m = col.B;

 if( m > (VDBL)1.0 )
   {
    /* overflow condition */
    /* normalize it! */
    col.R /= m;
    col.G /= m;
    col.B /= m;
   }

 #ifdef __DEBUG
 printf("Red=%f   Green=%f   Blue=%f\n",col.R,col.G,col.B);
 #endif

 return col;
}

COLOR Trace(RAY r,int n)
{
 RAY r2;

 r2 = SampleRay(r, r.theta);

 return ( TraceRay(r2, n) );
}



void RayTrace(char *fname,int internal)
{
 int x, y, x1, y1,t0,t1;
 VDBL xr, yr, xstep, ystep;
 VECTOR hor,ver;
 COLOR col,color;
 RAY ray, r2;
 unsigned long r, g, b;
 VDBL m;
 FILE *f;
 int i,save;
 VDBL p_w;


 /* Render To Screen Only ?? */
 save = ( fname != NULL);

 p_w = fov / MIN(xres, yres);	/* pixel width in radians */

 hor = norm( vcross(up, eye_dir) );	/* the x screen vector */
 ver = norm( vcross( eye_dir, hor) );	/* the y screen vector */

 if( save )
   {
    f = fopen(fname, "w");
    if(f == NULL)
       save=0;
   }

 ray.pos = eye;		/* eye is the beginning of the ray */

 ray.obj = NULL;		/* not coming from an object */

 /* Save The TGA Structure */
 if( save )
   {
    fprintf(f,"%c",0); fprintf(f,"%c",0);
    fprintf(f,"%c",2);
    fprintf(f,"%c",0); fprintf(f,"%c",0);
    fprintf(f,"%c",0); fprintf(f,"%c",0);
    fprintf(f,"%c",0);
    fprintf(f,"%c",0); fprintf(f,"%c",0);
    fprintf(f,"%c",0); fprintf(f,"%c",0);
    fprintf(f,"%c",(unsigned char)(xres%256));
    fprintf(f,"%c",(unsigned char)(xres/256));
    fprintf(f,"%c",(unsigned char)(yres%256));
    fprintf(f,"%c",(unsigned char)(yres/256));
    fprintf(f,"%c",24);
    fprintf(f,"%c",0x20);
   }
 
 yr = 1.0;

 xstep = 2.0/xres;
 ystep = 2.0/yres;
 
 xstep = 2. / xres; ystep = 2. / yres;

 if( internal )
    InitPlot(xres,yres);
 
 fprintf(stdout,"RENDERING\n");
 for(y = 0; y < yres;y++)
    {
     xr = -1.0;
     for(x = 0; x < xres; x++)
        {
         /* ray direction calculations */
         ray.dir = vadd( svproduct(xr*fov, hor),
         svproduct(yr*fov, ver) );
         ray.dir = norm( vadd( ray.dir, eye_dir) );
         ray.theta = 0;

	 color.R = color.G = color.B = 0.0;

         r2 = SampleRay(ray,p_w);
         col = TraceRay(r2, 0);
         color.R += col.R;
         color.G += col.G;
         color.B += col.B;

         /* calc the integer value to be put to the file */
         r = (unsigned long)(color.R*255);
         g = (unsigned long)(color.G*255);
         b = (unsigned long)(color.B*255);

         AddPixel(x,y,r,g,b);
 
         if( save)
           {
            putc(b,f);
            putc(g,f);
            putc(r,f);
           }
         xr += xstep;
        }
	yr -= ystep;
    }

 if( save)
    fclose(f);

 if( internal)
    ClosePlot();
}


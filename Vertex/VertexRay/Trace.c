/* 
 * This is the actual ray-tracing part
 */

#include"VRStruct.h"
#include"Vector.h"
#include"Display.h"
#include"BVH.h"
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
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



/* --------------------------------------------------------------------
 * Row-parallel rendering (2026-10).
 *
 * The pixel loop in RayTrace() has no cross-row state that a worker
 * cannot recreate bit-for-bit: xr restarts at -1.0 on every row, and
 * yr is a pure accumulation of ystep subtractions from 1.0 — a worker
 * rendering row y replays the same y subtractions (scalar cost ~yres
 * ops, nothing next to a row of ray tracing). Rows are independent, so
 * work is pulled dynamically from one counter (load-balanced: sky rows
 * are cheap, object rows are not).
 *
 * Each worker only computes RGB8 bytes into the frame buffer. AddPixel
 * and the TGA putc stream stay on the main thread, in the original
 * row-major order, in a second pass — the on-screen image appears at
 * the end of the render instead of creeping top-down; the file bytes
 * are identical.
 * -------------------------------------------------------------------- */
static unsigned char *bv_fb;                 /* xres*yres*3 bytes, R,G,B */
static VDBL bv_xstep, bv_ystep, bv_pw;
static VECTOR bv_hor, bv_ver;
static int  bv_next_row;
static pthread_mutex_t bv_row_lock = PTHREAD_MUTEX_INITIALIZER;

static void render_row(int y)
{
 int x, k;
 VDBL xr, yr;
 RAY ray, r2;
 COLOR col, color;
 unsigned long R, G, B;
 unsigned char *pix;

 ray.pos = eye;             /* eye is the beginning of the ray */
 ray.obj = NULL;            /* not coming from an object */

 yr = 1.0;
 for( k=0; k<y; k++ )
    yr -= bv_ystep;         /* replay the serial accumulation exactly */

 xr = -1.0;
 for(x = 0; x < xres; x++)
    {
     /* ray direction calculations — identical expressions to the
        serial loop, so every ray is the same double */
     ray.dir = vadd( svproduct(xr*fov, bv_hor),
     svproduct(yr*fov, bv_ver) );
     ray.dir = norm( vadd( ray.dir, eye_dir) );
     ray.theta = 0;

     color.R = color.G = color.B = 0.0;

     r2 = SampleRay(ray,bv_pw);
     col = TraceRay(r2, 0);
     color.R += col.R;
     color.G += col.G;
     color.B += col.B;

     /* calc the integer value to be put to the file */
     R = (unsigned long)(color.R*255);
     G = (unsigned long)(color.G*255);
     B = (unsigned long)(color.B*255);

     pix = &bv_fb[ ((size_t)y*xres + x)*3 ];
     pix[0] = (unsigned char)R;   /* AddPixel's color params are unsigned
                                     char and putc takes the low 8 bits,
                                     so one byte is what both would see */
     pix[1] = (unsigned char)G;
     pix[2] = (unsigned char)B;

     xr += bv_xstep;
    }
}

static void pull_and_render(void)
{
 int y;
 for(;;)
   {
    pthread_mutex_lock(&bv_row_lock);
    y = bv_next_row++;
    pthread_mutex_unlock(&bv_row_lock);
    if( y >= yres )
       return;
    render_row(y);
   }
}

static void *row_worker(void *arg)
{
 (void)arg;
 pull_and_render();
 return NULL;
}

static int wanted_threads(void)
{
 const char *e = getenv("VRAY_THREADS");
 long n;
 if( e )
   {
    n = atol(e);
    if( n < 1 ) n = 1;
    if( n > 64 ) n = 64;
    return (int)n;
   }
 n = sysconf(_SC_NPROCESSORS_ONLN);
 if( n < 1 ) n = 1;
 if( n > 64 ) n = 64;
 return (int)n;
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
 int nt, made = 0;
 pthread_t th[64];


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
 
 xstep = 2. / xres; ystep = 2. / yres;

 if( internal )
    InitPlot(xres,yres);
 
 fprintf(stdout,"RENDERING\n");

 /* Threaded pass: compute all rows into the frame buffer, then run
    AddPixel + the TGA byte stream on this thread in the original
    order. Falls back to the untouched serial loop below when the
    buffer cannot be allocated or threading is disabled. */
 /* Publish a complete BVH before workers query it, including for embedded
    callers that populate obj[] without ParseFile. Geometry must remain
    unchanged until all workers have joined. */
 (void)BVHValid();
 nt = wanted_threads();
 if( nt > yres )
    nt = yres;
 bv_fb = NULL;
 if( nt > 1 )
    bv_fb = (unsigned char *) malloc( (size_t)xres * (size_t)yres * 3 );

 if( bv_fb )
   {
    bv_xstep = xstep; bv_ystep = ystep; bv_pw = p_w;
    bv_hor = hor; bv_ver = ver;
    bv_next_row = 0;

    for( i=0; i<nt; i++ )
      {
       if( pthread_create(&th[i], NULL, row_worker, NULL) != 0 )
          break;
       made++;
      }
    pull_and_render();          /* the main thread works too */
    for( i=0; i<made; i++ )
       pthread_join(th[i], NULL);

    for( y=0; y<yres; y++ )
      for( x=0; x<xres; x++ )
        {
         r = bv_fb[ ((size_t)y*xres+x)*3 ];
         g = bv_fb[ ((size_t)y*xres+x)*3 + 1 ];
         b = bv_fb[ ((size_t)y*xres+x)*3 + 2 ];
         AddPixel(x,y,r,g,b);
         if( save)
           {
            putc(b,f);
            putc(g,f);
            putc(r,f);
           }
        }
    free(bv_fb);
    bv_fb = NULL;
   }
 else
   {
 yr = 1.0;
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

   }

 if( save)
    fclose(f);

 if( internal)
    ClosePlot();
}


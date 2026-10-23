#include <stdlib.h>
#include <math.h>
#include "Connect.h"


void (*VertexFillFace)(Drawable,int,int,int,int,int,int,int,int,int);


void MLine(XImage *im,int x1,int y1,int x2,int y2,int r,int g,int b)
{
 int c1,c2,critere,x,y,dx,dy,incx,incy,tmp;

 x=x1;y=y1;

 dx=x2-x1;dy=y2-y1;
 if (dx>=0)
   incx=1;
 else { incx=-1;dx=-dx; }
 if (dy>=0)
   incy=1;
 else { incy=-1;dy=-dy; }


 if (dy<dx)
 {
  c1=2*(dy-dx);c2=2*dy;
  critere=c2-dx;

  for (tmp=0;tmp<=dx;tmp++)
  {
   XPutPixel(im,x,y,GetRGB24(x,y,r,g,b));
 
   if (critere>0)
   { y+=incy;critere+=c1; }
   else critere+=c2;
   x+=incx;
  }
 }
 else
 {
  c1=2*(dx-dy);c2=2*dx;
  critere=c2-dy;
 
  for (tmp=0;tmp<=dy;tmp++)
  {
   XPutPixel(im,x,y,GetRGB24(x,y,r,g,b));

   if (critere>0)
   { x+=incx;critere+=c1; }
   else critere+=c2;
   y+=incy;
  }
 }
}

void swapcoords(int *x1,int *y1,int *x2,int *y2)
{
   int temp;

   temp = *x1;
   *x1  = *x2;
   *x2  = temp;

   temp = *y1;
   *y1  = *y2;
   *y2  = temp;

}


void FillFace8(XImage *im,int x0,int y0,int x1,int y1,
                  int x2,int y2,int r,int g,int bl)

{
 register int  z,a,b,dx,dy,d,deltap,deltaq,jstart;
 static int xl[2][3],yl[2][3];
 static int Xa[2][512],Ya[2][512];
 register int x,y,i,j,k;

/* sort the points in ascending vertical order:                       */
 if (y1 < y0) swapcoords(&x1,&y1,&x0,&y0);
 if (y2 < y0) swapcoords(&x2,&y2,&x0,&y0);
 if (y2 < y1) swapcoords(&x2,&y2,&x1,&y1);

/* stick them in arrays for the triangle edge computation:            */
 xl[0][0] = x0;              yl[0][0] = y0;
 xl[0][1] = x1;              yl[0][1] = y1;
 xl[0][2] = x2;              yl[0][2] = y2;

/* for the sake of using loops instead of if statements, we're        */
/* pretending that the long side of the triangle is made up of two    */
/* lines.  one of them has zero length: (x2,y2) to (x2,y2).           */

 xl[1][0] = x0;              yl[1][0] = y0;
 xl[1][1] = x2;              yl[1][1] = y2;
 xl[1][2] = x2;              yl[1][2] = y2;

/* i loops over the two triangles:                                    */
/* k loops over the two short sides and the one long side:            */

   for (i = 0; i < 2; i++)
     {
       jstart = 0;
       for (k = 1; k < 3; k++)
         {
           dx = abs(xl[i][k] - xl[i][k-1]);
           dy = abs(yl[i][k] - yl[i][k-1]);

           x = xl[i][k-1];
           y = yl[i][k-1];

           if (dy <= dx)
             {
               z = xl[i][k];
               a = (xl[i][k-1] <= xl[i][k]) ? 1 : -1;
               b = (yl[i][k-1] <= yl[i][k]) ? 1 : -1;

               deltap = dy << 1;
               d      = deltap - dx;
               deltaq = d - dx;
               j = jstart;
               Xa[i][j] = x;
               Ya[i][j] = y;

               while (x != z)
                 {
                   x += a;
                   (d < 0) ? (d += deltap) :
                             (y += b,d += deltaq,j += 1);
                   Xa[i][j] = x;
                   Ya[i][j] = y;
                 }
             }
           else
             {
               z = yl[i][k];
               a = (yl[i][k-1] <= yl[i][k]) ? 1 : -1;
               b = (xl[i][k-1] <= xl[i][k]) ? 1 : -1;

               deltap = dx << 1;
               d      = deltap - dy;
               deltaq = d - dy;
               j = jstart;
               Xa[i][j] = x;
               Ya[i][j] = y;

               while (y != z)
                 {
                   y += a;
                   j += 1;
                   (d < 0) ? (d += deltap) :
                             (x += b,d += deltaq);
                   Xa[i][j] = x;
                   Ya[i][j] = y;
                 }
             }
           jstart = dy;
         }
     }

/* draw the blank horizontal lines:                                   */
   for(j=0; j <= (yl[0][2] - yl[0][0]); j++)
       MLine(im,Xa[0][j],Ya[0][j],Xa[1][j],Ya[0][j],r,g,bl);
} 


/*
void FillFace16_24(Drawable im,int x0,int y0,int x1,int y1,
                  int x2,int y2,int r,int g,int bl)
{
 XPoint pt[3]; 

 XSetForeground(dpy,gc,GetRGB24(0,0,r,g,bl));

 pt[0].x = x0;   pt[0].y = y0;
 pt[1].x = x1;   pt[1].y = y1;
 pt[2].x = x2;   pt[2].y = y2;
 
 XFillPolygon(dpy,im,gc,pt,3,Convex,CoordModeOrigin);
}
*/


#define UBYTE unsigned char
#include <string.h>
const int PE_MAX_HEIGHT = 1024;

const int D_SHIFT  = 4;
const int D_MASK   = 15;
const int D_PMASK  = 3;
const int D_PSHIFT = 2;

typedef struct 
{
 int x,y;
}PT2D;

typedef struct 
{
  PT2D left;
  PT2D right;
}Mug;

Mug mug[1024];

int D4[] =
   {
    0, 12,  3, 15,
    8,  4, 11,  7,
    2, 14,  1, 13,
   10,  6,  9,  5
   };

void (*VertexFillFace)(XImage *,int,int,int,int,int,int,int,int,int);


void FillFace8_2(register UBYTE *graph_mem,register int bytes_per_line,PT2D *p1,register UBYTE Red,register UBYTE Green,register UBYTE Blue,register int width,register int height,int pxsize)
{
 int miny = 1000, maxy = -1;
 int i,j,u,k;
 unsigned char *graph_ptr, *graph_ptr0;
 int clock_wise = 0;
 int cw; 
 int treshold; 
/*
 int r = ((( Red ) < ( (3  << 6 )  )) ? ( Red ) : ( (3  << 6 )  ))  >> 2 ; 
 int g = ((( Green ) < ( (3  << 6 )  )) ? ( Green ) : ( (3  << 6 )  ))  >> 2 ; 
 int b = ((( Blue) < ( (3  << 6 )  )) ? ( Blue ) : ( (3  << 6 )  ))  >> 2 ;  
 int mr = r  & D_MASK; 
 int mg = g  & D_MASK; 
 int mb = b  & D_MASK; 
 int rr = r >> D_SHIFT; 
 int gg = g >> D_SHIFT; 
 int bb = b >> D_SHIFT; 
*/
 
 int y,y1,y2,dy,sy;
 int x,x1,x2,dx,sx,ex;
 int dx1,dy1,dx2,dy2;

  for(i=0; i<3; i++)
     {
       if(!clock_wise)
	 {
	   j = i+1;
	   if(j == 3)
	     j = 0;

	   k = j+1;
	   if(k == 3)
	     k = 0;

	   dx1 = p1[i].x - p1[j].x;
	   dy1 = p1[i].y - p1[j].y;
	   dx2 = p1[k].x - p1[j].x;
	   dy2 = p1[k].y - p1[j].y;
	
	   clock_wise = dx1 * dy2 - dx2 * dy1;
	 }

	miny = ((( miny ) < ( p1[i].y )) ? ( miny ) : ( p1[i].y )) ;
	maxy = ((( maxy ) > ( p1[i].y )) ? ( maxy ) : ( p1[i].y )) ;
     }

  clock_wise = (clock_wise > 0);

  if(miny == maxy)
    return;
  
  for(i=0; i<3; i++)
    {
      j = i+1;

      if(j==3)
	j = 0;


      if(p1[i].y == p1[j].y)
	continue;
      
      
      y1 = p1[i].y;
      y2 = p1[j].y;
      dy = y2 - y1;
      sy = ((( dy ) > (0)) ? 1 : (( dy ) ? -1 : 0)) ;
      dy *= sy;
      y  = y1;


      x1 = p1[i].x;
      x2 = p1[j].x;
      dx = x2 - x1;
      sx = ((( dx ) > (0)) ? 1 : (( dx ) ? -1 : 0)) ;
      dx *= sx;
      ex = (dx << 1) - dy;
      x  = x1;

      if(clock_wise ^ (y2 > y1))
	 {
	    for(u=0; u <= dy; u++)
	      {
		 if( y >= 0 && y < PE_MAX_HEIGHT ) mug[y].right.x = x;   /* vertices can leave the window */
		 y += sy;

		 while(ex >= 0)
		   {
		      x += sx;
		      ex -= dy << 1;
		   }

		 ex += dx << 1;
	      }
	    
	 }
       else
	 {
	    for(u=0; u <= dy; u++)
	      {
		 if( y >= 0 && y < PE_MAX_HEIGHT ) mug[y].left.x = x;

		 y += sy;

		 while(ex >= 0)
		   {
		      x += sx;
		      ex -= dy << 1;
		   }

		 ex += dx << 1;
	      }
	 }
    }

  graph_ptr0 = (unsigned char *)(graph_mem + ((miny<0)?0:miny) * bytes_per_line);

  if( miny>=0 )
  for(y=miny; y<=maxy && y<height && y<PE_MAX_HEIGHT ; y++)
    {
      x1 = mug[y].left.x;
      x2 = mug[y].right.x;
      dx = x2 - x1 + 1;
      x  = x1;
      graph_ptr = graph_ptr0 + x1*pxsize;

      if( x1>=0 )
        {
         /* The 2- and 4-byte-per-pixel converters (GetRGB24_16/_24) ignore
            x,y, so the packed pixel is constant across the whole span:
            compute it once and store it at native width. The 8-bit dithered
            path keeps the per-pixel lookup (d(x,y) varies). */
         if(pxsize == 4)
           {
            unsigned long px = GetRGB24(x1,y,Red,Green,Blue);
            for(x=x1; x <= x2 && x<width ; x++)
               {
                memcpy(graph_ptr,&px,4);
                graph_ptr += 4;
               }
           }
         else if(pxsize == 2)
           {
            unsigned long px = GetRGB24(x1,y,Red,Green,Blue);
            for(x=x1; x <= x2 && x<width ; x++)
               {
                memcpy(graph_ptr,&px,2);
                graph_ptr += 2;
               }
           }
         else
          for(x=x1; x <= x2 && x<width ; x++)
            {
             unsigned long px = GetRGB24(x,y,Red,Green,Blue);
             int pb;
             for(pb=0;pb<pxsize;pb++)
                *graph_ptr++ = (unsigned char)(px >> (8*pb));
            }
        }

      graph_ptr0 = (unsigned char *)(((unsigned char *)graph_ptr0) + bytes_per_line);
    }
}


void FillFace8(XImage *img,int x0,int y0,int x1,int y1,
                  int x2,int y2,int r,int g,int bl)
{
 PT2D rfpt[3];

 rfpt[0].x = x0;   rfpt[0].y = y0;
 rfpt[1].x = x1;   rfpt[1].y = y1;
 rfpt[2].x = x2;   rfpt[2].y = y2;

 FillFace8_2(img->data,img->bytes_per_line,rfpt,r,g,bl,img->width,img->height,
             /* ZPixmap stride per pixel follows the image pad, not the
                raw bits_per_pixel: 1, 2 or 4 bytes (24-bit images use 4) */
             (img->bits_per_pixel <= 8) ? 1 :
             ((img->bits_per_pixel <= 16) ? 2 : 4));
}


/* Truecolor (16/24-bit) variant: same framebuffer path as FillFace8, now
   bytes-per-pixel aware via bits_per_pixel (replaces the old broken
   Drawable/XFillPolygon version that stayed commented out since 1995). */
void FillFace16_24(XImage *img,int x0,int y0,int x1,int y1,
                  int x2,int y2,int r,int g,int bl)
{
 FillFace8(img,x0,y0,x1,y1,x2,y2,r,g,bl);
}

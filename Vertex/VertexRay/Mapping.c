/* Spherical Map From POV 2.2 */

#include"VRStruct.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define MABS(x) (x>0)?x:-x
#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b

int SphericalMap(OBJ *obj,VDBL x,VDBL y,VDBL z,INTERSECT *i)
{
 VDBL len, phi, theta;
 int pos,rx,ry;

 len = sqrt(x*x + y*y + z*z);
 if (len == 0.0)
   return 0;
 else
   {
    x /= len;
    y /= len;
    z /= len;
   }
  
 phi = 0.5 + asin(y) / M_PI; /* This will be from 0 to 1 */

 len = sqrt(x*x + z*z);
 if(len == 0.0)
   {
    /* This point is at one of the poles. Any value of xcoord will be ok...*/
    theta = 0;
   }
 else
   {
    if(z == 0.0)
      {
       if(x > 0)
         theta = 0.0;
      else
        theta = M_PI;
      }  
    else
      {
      theta = acos(x / len);
      if (z < 0.0) theta = 2.0 * M_PI - theta; 
      }
    theta /= 2.0 * M_PI; /* This will be from 0 to 1 */
    }

  rx =(int)(theta * obj->map.Width /obj->map.Scale_X + obj->map.Width-50)  % (obj->map.Width);
  ry =(int)(phi * obj->map.Height/obj->map.Scale_Y ) % (obj->map.Height);
 
  pos = ry*obj->map.Width+rx;

  i->CMapping.R = (VDBL)obj->map.Red[pos] /256.0;
  i->CMapping.G = (VDBL)obj->map.Green[pos] /256.0;
  i->CMapping.B = (VDBL)obj->map.Blue[pos] /256.0;

  return 1;
}


int PlanarMap(OBJ *obj,VDBL x,VDBL y,VDBL z,INTERSECT *i)
{
 int pos;
 int u,v;


 u = (int)(x*obj->map.Width);
 v = (int)(y*obj->map.Height);
 u = MABS(u);
 v = MABS(v);

 u = u%obj->map.Width;
 v = v%obj->map.Height;  

 pos = u+ v*obj->map.Width;
 i->CMapping.R = obj->map.Red[pos] / 256.0;
 i->CMapping.G = obj->map.Green[pos] / 256.0;
 i->CMapping.B = obj->map.Blue[pos] / 256.0;
 return 1;

}
 
 
void MakeMapping(OBJ *obj,char *name)
{
 FILE *f;
 unsigned char c,c1[12],yd,yr,xd,xr;
 unsigned char *buff;
 register int xi,yi,pad,i,j,max; 
 register int t1,t2;
 int hdr;

 if( (f = fopen(name,"rb")) == NULL)
   {
    obj->Mapping=0;
    fprintf(stderr,"Unable To Open Map File %s\n",name);
    exit(0);  
   }

 hdr = 1;
 hdr &= ( fscanf(f,"%12c",c1) == 1 );
 hdr &= ( fscanf(f,"%c",&c) == 1 );  xr = c;
 hdr &= ( fscanf(f,"%c",&c) == 1 );  xd = c;
 hdr &= ( fscanf(f,"%c",&c) == 1 );  yr = c;
 hdr &= ( fscanf(f,"%c",&c) == 1 );  yd = c;

 hdr &= ( fscanf(f,"%c",&c) == 1 );
 if( c!= 24 ) { fclose(f); obj->Mapping=0; return; }
 hdr &= ( fscanf(f,"%c",&c) == 1 );
 if( c!= 0x20 ) { fclose(f); obj->Mapping=0; return; }

 if( !hdr )
   {   /* a header truncated mid-field: load no mapping rather than
          map uninitialized dimensions (unused-result sweep) */
    fclose(f);
    fprintf(stderr,"Bad Map Header %s\n",name);
    obj->Mapping=0;
    return;
   }

 xi = xd*256 + xr;
 yi = yd*256 + yr;

 if( xi < 1 || yi < 1 || (long)xi*(long)yi > 4096L*4096L )
   {    /* zero width/height gave SIGFPE in the map lookup, oversized
           overflowed the int sizes (I-03) */
    fclose(f);
    fprintf(stderr,"Bad Map Dimensions %dx%d in %s\n",xi,yi,name);
    obj->Mapping=0;
    return;
   }


 switch( obj->Type )
       {
        case(SPHERE_OBJ): obj->map.Type=SPHERICAL; break;
        case(PLANE_OBJ):  obj->map.Type=PLANAR;    break;
       }
             
 free(obj->map.Red);    /* a remap must not leak the previous planes (M-11) */
 free(obj->map.Green);
 free(obj->map.Blue);
 obj->map.Red = obj->map.Green = obj->map.Blue = 0;

 obj->map.Red   = (unsigned char *)malloc(xi*yi);
 obj->map.Green = (unsigned char *)malloc(xi*yi);
 obj->map.Blue  = (unsigned char *)malloc(xi*yi);


 obj->map.Scale_X = 0.5;
 obj->map.Scale_Y = 1.0;

 obj->map.Width  = xi;
 obj->map.Height = yi;


 if( !obj->map.Red || !obj->map.Green || !obj->map.Blue ) 
   {
    fclose(f);
    fprintf(stderr,"Not Enough Memory For Mapping Buffer\n");
    free(obj->map.Red);        /* free whatever did allocate (M-11) */
    free(obj->map.Green);
    free(obj->map.Blue);
    obj->map.Red = obj->map.Green = obj->map.Blue = 0;
    obj->Mapping=0; return;
   }

 max = xi*yi;

 buff = (unsigned char *)malloc(max*3); 
 if( buff == NULL )
   { 
    for(i=1 ; i<=max &&
             fread(&(obj->map.Blue[max-i]),1,1,f) == 1 &&
             fread(&(obj->map.Green[max-i]),1,1,f) == 1 &&
             fread(&(obj->map.Red[max-i]),1,1,f)  == 1 ; i++ )
        ;
    if( i <= max )
      {   /* truncated texture on the no-buffer path: same bail as below */
       fclose(f);
       fprintf(stderr,"Truncated Map File %s\n",name);
       free(obj->map.Red);
       free(obj->map.Green);
       free(obj->map.Blue);
       obj->map.Red = obj->map.Green = obj->map.Blue = 0;
       obj->Mapping=0;
       return;
      }
   }
 else
   {
    if( fread(buff,max*3,1,f) != 1 )
      {   /* truncated texture: drop the mapping rather than render garbage (I-03) */
       fclose(f);
       fprintf(stderr,"Truncated Map File %s\n",name);
       free(buff);
       free(obj->map.Red);
       free(obj->map.Green);
       free(obj->map.Blue);
       obj->map.Red = obj->map.Green = obj->map.Blue = 0;
       obj->Mapping=0;
       return;
      }
    for(j=max-1,i=0 ; i<(max*3) ; i++,j--)
       {
        obj->map.Blue[j]  = buff[i];
        i++;
        obj->map.Green[j] = buff[i];
        i++;
        obj->map.Red[j]   = buff[i];   
       }

   } 

/*
  for(j=0 ; j< yi ; j++)
      for(i=0 ; i<(xi/2) ; i++ )
         {
          t1 = j*xi+i;
          t2 = j*xi+xi-i-1;
          pad = obj->map.Blue[t1];
          obj->map.Blue[t1]=obj->map.Blue[t2];
          obj->map.Blue[t2]=pad;

          pad = obj->map.Green[t1];
          obj->map.Green[t1]=obj->map.Green[t2];
          obj->map.Green[t2]=pad;

          pad = obj->map.Red[t1];
          obj->map.Red[t1]=obj->map.Red[t2];
          obj->map.Red[t2]=pad;
         }
*/

 free(buff);
 fclose(f);
}

/******************************************/
/*                VerteX 3D               */
/*         Ray Tracing Extension          */
/*               Version 1.0              */
/*----------------------------------------*/
/*        Vector Operations Module        */
/*----------------------------------------*/
/* Concept And Design:C.LEGENDRE Nov 1995 */
/******************************************/
#include "Vector.h"
#include<math.h>

VECTOR vadd(VECTOR a,VECTOR b)
{
 VECTOR c;

 c.X=a.X+b.X;
 c.Y=a.Y+b.Y;
 c.Z=a.Z+b.Z;

 return(c);
}

VECTOR vsub(VECTOR a,VECTOR b)
{
 	VECTOR	c;

 c.X=a.X-b.X;
 c.Y=a.Y-b.Y;
 c.Z=a.Z-b.Z;

 return c;
}

VECTOR vneg(VECTOR a)
{
 VECTOR	b;

 b.X= -a.X;
 b.Y= -a.Y;
 b.Z= -a.Z;

 return b;
}

VECTOR svproduct(VDBL k,VECTOR a)
{
 a.X*=k;
 a.Y*=k;
 a.Z*=k;

 return a;
}
 
VDBL vdot(VECTOR a,VECTOR b)
{
 return (a.X*b.X+a.Y*b.Y+a.Z*b.Z);
}

VECTOR	vcross(VECTOR a,VECTOR b)
{
 VECTOR	c;

 c.X=a.Y*b.Z-b.Y*a.Z;
 c.Y=b.X*a.Z-a.X*b.Z;
 c.Z=a.X*b.Y-b.X*a.Y;

 return c;
}

VECTOR norm(VECTOR a)
{
 VDBL len;
 VECTOR b;

 len=sqrt(a.X*a.X+a.Y*a.Y+a.Z*a.Z);
 if(len != 0.0 )
   {
    b.X=a.X/len;
    b.Y=a.Y/len;
    b.Z=a.Z/len;
   }

 return b;
}

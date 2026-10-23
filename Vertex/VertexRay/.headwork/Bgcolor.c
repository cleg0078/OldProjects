
#include "VRStruct.h"

extern int bgflag;

COLOR Bgcolor(RAY r)
{
 COLOR c;

/* if it is above the appropriate axis just give a grey value */
/* if below give a gradually whiter color                     */
	switch( bgflag ) {
		case NONE:
			c.R = c.G = c.B = 0.0;
			break;
		case aX:
			if( r.dir.X > 0.0 )
				c.R = c.G = c.B = 0.2;
			else
				c.R = c.G = c.B = 0.2 - 0.8 * r.dir.X;
			break;
		case aY:
			if( r.dir.Y > 0.0 )
				c.R = c.G = c.B = 0.2;
			else
				c.R = c.G = c.B = 0.2 - 0.8 * r.dir.Y;
			break;
		case aZ:
			if( r.dir.Z > 0.0 )
				c.R = c.G = c.B = 0.2;
			else
				c.R = c.G = c.B = 0.2 - 0.8 * r.dir.Z;
			break;
	}
	return c;
}

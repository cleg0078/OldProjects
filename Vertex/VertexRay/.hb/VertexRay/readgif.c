/* GIF-decompession-routine  */

#include <stdio.h>
#include <stdlib.h>
#include"Connect.h"

#define MAX(a,b)    ((a)>(b)?(a):(b))
#define MIN(a,b)    ((a)<(b)?(a):(b))
#define MABS(x)      ((x<0)?(-(x)):(x))
#define d(x,y) ((dm[((x) & 7) + (((y) & 7) << 3)] & 0xff - 128) / 3)

typedef unsigned char UBYTE;
typedef unsigned short UWORD;
typedef unsigned long ULONG;

#define ILL_PAR 102
#define NO_MEM 100
#define NOT_INP 104
#define NOT_OUT 105

/* Globale Variablen */

UBYTE *PixelBuffer[2048];
unsigned char Cregs[768];
UWORD Prefix[4096], Suffix[4096];
UWORD OutCode[1025];
char Interlace;
unsigned long TCounter = 0;
int FileTerminate = 0;

UWORD XC = 0,
    YC = 0,
    Pass = 0,
    OutCount = 0,
    RWidth, RHeight,
    Width, Height,
    LeftOfs, TopOfs,
    CodeSize;

UBYTE background;
float aspect;

XGUI gui;
XShell *sh;
unsigned long color[216];
void (*AddPixel)(int,int,unsigned long,unsigned long,unsigned long);

void AddPixel8(int x,int y,ULONG Red,ULONG Green,ULONG Blue)
{
 register  long thePixel;
 register int r, g, b, rr,gg,bb;
 register int luminence;
 static int dm[] = {
                    0, 192,  48, 240,  12, 204,  60, 252,
                    128,  64, 176, 112, 140,  76, 188, 124,
                    32, 224,  16, 208,  44, 236,  28, 220,
                    160,  96, 144,  80, 172, 108, 156,  92,
                    8, 200,  56, 248,   4, 196,  52, 244,
                    136,  72, 184, 120, 132,  68, 180, 116,
                    40, 232,  24, 216,  36, 228,  20, 212,
                    168, 104, 152,  88, 164, 100, 148,  84
                   };

 rr = ((int)Red  )+ d(x,y); if (rr>255) rr=255; if (rr<0) rr=0;
 gg = ((int)Green)+ d(x,y); if (gg>255) gg=255; if (gg<0) gg=0;
 bb = ((int)Blue )+ d(x,y); if (bb>255) bb=255; if (bb<0) bb=0;
 r = (rr * 216) / (256 * 36);
 g = (gg * 216) / (256 * 36);
 b = (bb * 216) / (256 * 36);

 thePixel = color[(r * 36) + (g * 6) + b];
 XSetForeground(gui.dpy,sh->gc,thePixel);
 XDrawPoint(gui.dpy,sh->Win,sh->gc,x,y);
}

void AddPixel16(int x,int y,unsigned long r,unsigned long g,unsigned long b)
{
 unsigned long rcol=(unsigned long )((unsigned long)(r>>3)<<11|
                    (unsigned long)(g>>2)<<5|(unsigned long)(b>>3));

 XSetForeground(gui.dpy,sh->gc,rcol);
 XDrawPoint(gui.dpy,sh->Win,sh->gc,x,y);
}


UWORD fget16(fp)
FILE *fp;
{
  register UBYTE b1,b2;
  b2=getc(fp);
  b1=getc(fp);
  return((UWORD)b1<<8 | ((UWORD)b2 ));
}

/*getubyte: get a byte from blocked file */
/* returns byte in <ch> and a return value of 0 in <FileTerminate> if ok */
/* uses global variables <count>, <FileTerminate> and <stdin> */

int count = 0;

UBYTE getubyte()
{
  register UBYTE ch;

  if (feof(stdin)) {
    FileTerminate=!0;
    return(0);
  };
  if (count != 0) {
    ch = getc(stdin);
    count--;
  }
  else {
    count = getc(stdin);
    if (count==0) {
      FileTerminate=!0;
      return(0);
    };
    ch = getc(stdin);
    count--;
  };
  return(ch);
}

short bitcount = 0;
UWORD restbits = 0;

UWORD getbits(bits)
short bits;
{
 UWORD w;
 UBYTE mask;
 short shift;
 UBYTE c;

  w = 0;
  shift = 0;
  while (bits != 0) {
    if (bitcount==0) {
      c=getubyte();
      if (FileTerminate) {
	return(w);
      };
      if (bits>=8) {
        w |= ((UWORD)c << shift);
	bits -= 8;
	shift += 8;
	restbits = 0;
      }
      else {
        mask = 0xff;
	mask >>= (8-bits);
	w |= ((c & mask) << shift);
	restbits = c >> bits;
	bitcount = 8-bits;
	shift += bits;
        bits = 0;
      };
    }
    else { /* bitcount > 0 */
      if (bits>=bitcount) {
	w |= (restbits << shift);
	bits -= bitcount;
        shift += bitcount;
	bitcount=0;
	restbits = 0;
      }
      else { /* bits < bitcount */
	mask = 0xff;
        mask >>= (8-bits);
        w |= ((restbits & mask) << shift);
	restbits >>= bits;
	bitcount -= bits;
	shift += bits;
	bits = 0;
      };
    };
  }; /* while */
  return(w);
}

void AddToPixel(Index)
UBYTE Index;
{
  UBYTE *ptr;

  if (YC < Height) {
    ptr = PixelBuffer[YC];
    ptr += (unsigned long)XC;
    *ptr = Index;
    if (++XC == Width) {
      XC=0;
      if (!Interlace) YC++;
      else {
	switch(Pass) {
          case 0:
            YC += 8;
	    if (YC >= Height) {
	      Pass++;
	      YC=4;
	    };
	    break;
          case 1:
	    YC += 8;
	    if (YC >= Height) {
	      Pass++;
	      YC=2;
	    };
            break;
          case 2:
	    YC += 4;
	    if (YC >= Height) {
	      Pass++;
	      YC=1;
	    };
            break;
	  case 3:
	    YC += 2;
	    break;
	  default:
	  break;
        };
      };
    };
  };
}


void main(int nb,char **argv)
{
  FILE *in,*ftmp;
  UWORD Colors;
  UWORD sWidth, sHeight;
  UBYTE gFlags;
  UWORD iWidth, iHeight, iLeft, iTop;
  UWORD iCol,
    Code,
    MaxCode,
    ClearCode,
    EOFCode,
    OldCode,
    InCode,
    FirstFree,

    FinChar,
    BitMask,
    InitCodeSize,
    BitsPerPixel;
  UBYTE iFlags;
  UBYTE c;
  UBYTE AspByte; /* Aspect Ratio information */
  int n;
  char IDbuffer[6];
  UWORD FreeCode, CurCode;
  short i,j;
  ULONG Red,Green,Blue,r,g,b;
  Colormap cmap;
  XColor tmp;

  if( nb<2 )
    {
     fprintf(stderr,"USAGE: %s FILE\n",argv[0]);
     exit(0);
    }

  
  if( !(in=fopen(argv[1],"rb")) )
    {
     fprintf(stderr,"Unable To Open %s\n",argv[1]);
     exit(0);
    }

 ftmp = (FILE *)calloc(sizeof(FILE),1);

 *ftmp = *stdin; 
 *stdin = *in;

  n=fread(IDbuffer,6,1,stdin);
  if(strncmp(IDbuffer,"GIF87a",6)!=0)
   {
    fprintf(stderr,"GIF - ID not found\n");
    exit(0);
   }

  sWidth = Width = fget16(stdin);
  sHeight = Height = fget16(stdin);
  gFlags = (UBYTE)getc(stdin);
  background = (UBYTE)getc(stdin);
  AspByte = (UBYTE)getc(stdin);
  if (AspByte!=0) aspect = ((float)((UWORD)AspByte + 15)) / 64.0;

  BitsPerPixel = (gFlags & 0x07) + 1;
  Colors= 1 << BitsPerPixel;
  fprintf(stderr,"Bits Per Pixel %d Nb Colors=%d\n",BitsPerPixel,Colors);


  n=fread(Cregs,(int)Colors,3,stdin);

  if ((c=getc(stdin))==0x2c) { /* process Image separator */
    iLeft = fget16(stdin);
    iTop = fget16(stdin);
    iWidth = Width = fget16(stdin);
    iHeight = Height = fget16(stdin);
    iFlags = getc(stdin);
    Interlace = (iFlags & 0x40);

    if (iFlags & 0x80) {
      iCol = (iFlags & 0x07) + 1;
      BitsPerPixel = iCol;
      Colors = iCol;
    };
  }
  else {
    fprintf(stderr,"No Image separator found\n");
    exit(30);
  };

  if (AspByte == 0) {
    if ((Width==320) && (Height==200)) aspect=1.2;
    else if ((Width==640) && (Height==480)) aspect=1.0;
    else if ((Width==640) && (Height==350)) aspect=1.37142857;
    else aspect = 1.0;
  };

  if (Height>2048) {
    fprintf(stderr,"Image too large (Height > 2048) %d\n",Height);
    exit(40);
  };

  for(i=0; i<Height; i++)
     {
      if((PixelBuffer[i]=calloc(1,Width))==NULL)
	{
	 fprintf(stderr,"not enough memory for the pixelbuffer\n");
	 exit(50);
	}
     }

  for(i=0; i<4096; i++)
      Prefix[i]=Suffix[i]=0;

  fprintf(stderr,"Reading %u*%u Image\n",(unsigned)Width,(unsigned)Height);


  BitMask = Colors - 1;
  CodeSize = getc(stdin);
  ClearCode = (1 << CodeSize);
  EOFCode = ClearCode + 1;
  FreeCode = FirstFree = ClearCode + 2;

  CodeSize++;
  InitCodeSize = CodeSize;
  MaxCode = (1 << CodeSize);
  InCode = 0;

  /* Decompress */

  Code = getbits(CodeSize);
  if (FileTerminate) {
    goto Corrupt_GIF;
  };
  while (Code != EOFCode) {
    if (Code == ClearCode) {
      CodeSize = InitCodeSize;
      MaxCode = (1 << CodeSize);
      FreeCode = FirstFree;
      CurCode = OldCode = Code = getbits(CodeSize);
      if (FileTerminate)
	{

	goto Corrupt_GIF;
      };
      FinChar = CurCode & BitMask;
      AddToPixel((UBYTE)FinChar);
      TCounter++;
    }
    else { /* Normal Data */
      CurCode = InCode = Code;
      if (CurCode >= FreeCode) {
	CurCode = OldCode;
	OutCode[OutCount++] = FinChar;
        TCounter++;
      };

      while (CurCode > BitMask) {
	if (OutCount > 1024) {

Corrupt_GIF:
	  goto save_image;
	};
	TCounter++;
	OutCode[OutCount++] = Suffix[CurCode];
	CurCode = Prefix[CurCode];
      };
      FinChar = CurCode & BitMask;
      TCounter++;
      OutCode[OutCount++] = FinChar;

      for (i = OutCount-1; i>=0; i--) AddToPixel((UBYTE)OutCode[i]);
      OutCount = 0;

      Prefix[FreeCode] = OldCode;
      Suffix[FreeCode] = FinChar;
      OldCode = InCode;

      FreeCode++;
      if (FreeCode >= MaxCode) {
	if (CodeSize < 12) {
	  CodeSize++;
	  MaxCode <<= 1;
	}
      }
    } /* else */
    Code = getbits(CodeSize);
    if (FileTerminate)
      {
       fprintf(stderr," Corrupt GIF File ");
      goto Corrupt_GIF;
    }
  }; /* while */

  *stdin = *ftmp;

  if( XConnect(&gui,"") == 0)
      goto save_image;

 switch(DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)))
       {
        case(15):
        case(16):
             AddPixel = &AddPixel16;
             break;
        case(8): 
            cmap = DefaultColormap(gui.dpy, DefaultScreen(gui.dpy));
            for(i = r = 0; r < 6; r++)
                for(g = 0; g < 6; g++)
                    for(b = 0; b < 6; b++, i++)
                       {
                        tmp.red = (r * 36) << 8;
                        tmp.green = (g * 36) << 8;
                        tmp.blue = (b * 36) << 8;
                        tmp.flags = DoRed | DoGreen | DoBlue;
                        if(!XAllocColor(gui.dpy, cmap, &tmp))
                           goto NotEnough;
                        color[i] = tmp.pixel;
                       }
            AddPixel = &AddPixel8;
            NotEnough:
            if(i != 216)
               fprintf(stderr, "warning: could only get %d colors.\n", i);
      }     

  sh = CreateShell(&gui,"GIF Viewer By C.LEGENDRE",0,0,Width,Height);
  MapShell(sh);

  for(j=0;j<Height;j++)
     for(i=0;i<Width;i++)
        {
         Red   = Cregs[ PixelBuffer[j][i] *3 ];
         Green = Cregs[ PixelBuffer[j][i] *3 + 1];
         Blue = Cregs[ PixelBuffer[j][i] *3 + 2];
	 AddPixel(i,j,Red,Green,Blue);
        }

  WaitKey(sh);
  fclose(in);

save_image:
  for (i=0; i<Height; i++) free(PixelBuffer[i]);
  exit(0);
}

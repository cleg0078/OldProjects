/********************************************************/
/*             Graphic User Interface Librairie         */
/*                         V1.2                         */
/*------------------------------------------------------*/
/* Concept And Design: C.LEGENDRE  April 1996           */ 
/*------------------------------------------------------*/
/* Freely use. Spread The Disk and Not The Word!!!      */
/* But Remember: This is My Job...                      */
/********************************************************/

#include"Connect.h"

int XConnect(XGUI *tmp,const char *dp)
{
 tmp->dpy = XOpenDisplay(dp);
 return( tmp->dpy == NULL ? 0:1);
}

/* UI scale from the physical screen height: 1.0 up to ~1080p screens,
   2.0 on 4K, capped at 2.5. Smaller screens keep the 1995 compact layout. */
float XGUIScreenScale(Display *dpy)
{
 float s;

 if( dpy == NULL )
    return(1.0);

 s = (float)DisplayHeight(dpy,DefaultScreen(dpy)) / 1080.0;
 if( s < 1.0 )
    s = 1.0;
 if( s > 2.5 )
    s = 2.5;

 return(s);
}

/* Shared UI font, sized to the screen. X bitmap fonts do not scale, so an
   exact-pixel-size request can silently miss; we walk a candidate ladder
   (scalable families first) and verify the loaded metrics are near the
   requested size before accepting a font. s==1.0 loads the legacy DFONT. */
XFontStruct *XGUIGetFont(Display *dpy)
{
 static XFontStruct *font = NULL;
 /* The Display the cached font was loaded on: a font belongs to its
    connection, so a call on another Display must drop the cache first. */
 static Display *font_dpy = NULL;
 static int tried = 0;
 char pat[80];
 int px,i,n;
 char **names;
 XFontStruct *fs;
 XFontStruct *infos;

 if( tried )
    {
     /* cached font lives on another Display: free it on the connection
        that owns it, clear the cache and reload below on dpy */
     if( font != NULL && dpy != NULL && dpy != font_dpy )
        {
         XFreeFont(font_dpy,font);
         font = NULL;
         tried = 0;
        }
     else
        return(font);
    }
 tried = 1;

 if( dpy == NULL )
    return(NULL);

 px = (int)(14 * XGUIScreenScale(dpy));

 /* 1080p-and-below: exact 1995 behavior */
 if( px <= 14 )
    {
     font = XLoadQueryFont(dpy,DFONT);
     if( font != NULL )
        {
         font_dpy = dpy;
         return(font);
        }
    }

 /* exact-pixel-size patterns, nicer family first; misc-fixed is scalable
    in the modern X server's fontconfig backend */
 {
  const char *pats[] = {
    "-misc-fixed-medium-r-normal--%d-*-*-*-*-*-iso8859-1",
    "-*-helvetica-medium-r-normal--%d-*-*-*-*-*-*-*",
    0 };
  for(i=0; pats[i]; i++)
     {
      snprintf(pat,sizeof(pat),pats[i],px);
      fs = XLoadQueryFont(dpy,pat);
      if( fs != NULL &&
          fs->ascent + fs->descent >= px*3/4 &&
          fs->ascent + fs->descent <= px*5/4 )
        {
         font_dpy = dpy;
         return(font = fs);
        }
      if( fs != NULL )
         XFreeFont(dpy,fs);
     }
 }

 /* any font the server lists at (nearly) this pixel size */
 names = XListFontsWithInfo(dpy,"-*-*-*-*-*--*-*-*-*-*-*-*",64,&n,&infos);
 if( names != NULL )
   {
    for(i=0;i<n;i++)
        if( infos[i].ascent + infos[i].descent >= px*3/4 &&
            infos[i].ascent + infos[i].descent <= px*5/4 )
          {
           fs = XLoadQueryFont(dpy,names[i]);
           if( fs != NULL )
             {
              XFreeFontNames(names);
              XFree(infos);   /* the XFontStruct array is caller-owned */
              font_dpy = dpy;
              return(font = fs);
             }
          }
    XFreeFontNames(names);
    XFree(infos);
   }

 /* last resorts */
 font = XLoadQueryFont(dpy,DFONT);
 if( font == NULL )
    font = XLoadQueryFont(dpy,"fixed");
 if( font != NULL )
    font_dpy = dpy;

 return(font);
}

void XDeconnect(XGUI *tmp)
{
 if(tmp->dpy)
    XCloseDisplay(tmp->dpy);
}


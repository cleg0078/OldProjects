/*
 *
 * Xgport.C
 *
 */


#include "Xgport.h"
#include <stdlib.h>

OpCode XGraphicPort::RESIZE   = GraphicComponent::UniqueOpCode();
OpCode XGraphicPort::XSTDPROP = GraphicComponent::UniqueOpCode();

int XGraphicPort::Cntl(OpCode op, int arg)
{
  if(op == RESIZE)
    {
      XSizeHints  hint;
      hint.flags = PMinSize | PMaxSize;
      if(arg)
	{
	  hint.min_width  = MIN_WIDTH;
	  hint.min_height = MIN_HEIGHT;
	  hint.max_width  = MAX_WIDTH;
	  hint.max_height = MAX_HEIGHT;
	}
      else
	{
	  hint.min_width  = _width;
	  hint.min_height = _height;
	  hint.max_width  = _width;
	  hint.max_height = _height;
	  hint.flags = PMinSize | PMaxSize;
	}
      XSetStandardProperties (_display, 
			      _window, 
			      _name, 
			      _name, None, NULL, 0, &hint);
      return 1;
    }
  else
  if(op == XSTDPROP)
    {
      XSizeHints *hint = (XSizeHints *)arg;
      XSetStandardProperties(_display, 
			      _window, 
			      _name, 
			      _name, None, NULL, 0, hint);
      return 1;
    }

  return 0;
}

XGraphicPort::XGraphicPort(char *name, ScrCoord width, ScrCoord height, int verbose_level): 
       GraphicPort(name, width, height, verbose_level)
{

  _error_code  = GPE_OK; 
  _xerror_flag = 0;
  _buffered    = 0;
  _resources.SetAll(XGPR_NONE);
  _proc_name = "XGraphicPort";

  if(!OpenWindow())
    {
      (*this)[MSG_ERROR] << "could not init X stuff\n";
      return;
    }

#ifdef X_SHM  
  _shm_flag = XShmQueryExtension(_display);
  if(_shm_flag)
    {
      fprintf(stderr,"X server has MIT XShm. Good.\n");
      _completion_type = XShmGetEventBase(_display) + ShmCompletion;
    }
#else
  _shm_flag = 0;
#endif

  _local_cmap_flag = 0;

  if(!AllocateXImage())
    return;

  Attributes().Set(GPA_DBUFF);

  if(!AllocateColormaps())
    return;

  ColormapMode();

  if(_resources.Get(XGPR_STD_XIMAGE) || _resources.Get(XGPR_SHM_XIMAGE))
    _bytes_per_line = _ximage->bytes_per_line;

  _clip.Set(0,0,_width-1,_height-1);

  if(_verbose_level >= MSG_INFO)
    {
      fprintf(stderr,"[TAGL] %s:: bytes per pixel = %d\n", 
	      _proc_name, _bytes_per_pixel);
      fprintf(stderr,"[TAGL] %s:: R mask = ",
	      _proc_name); printb(_R_mask); fprintf(stderr,"\n");
      fprintf(stderr,"[TAGL] %s:: G mask = ",
	      _proc_name); printb(_G_mask); fprintf(stderr,"\n");
      fprintf(stderr,"[TAGL] %s:: B mask = ",
	      _proc_name); printb(_B_mask); fprintf(stderr,"\n");
    }

}


////
////
//
//   an X error handler: detect problems during XImage initialisation
//
////
////


int XGraphicPort::_xerror_flag;

int XGraphicPort::HandleXError(Display *display, XErrorEvent *event)
{
  _xerror_flag = 1;
  return 0;
}

void XGraphicPort::InstallXErrorHandler()
{
  (*this)[MSG_INFO] << "Installing X error handler\n";
  XSetErrorHandler(HandleXError);
  XFlush(_display);
}

void XGraphicPort::UninstallXErrorHandler()
{
  (*this)[MSG_INFO] << "Uninstalling X error handler\n";
  XSetErrorHandler(NULL);
  XFlush(_display);
}


////
////
//
//   Attributes handling & destructor
//
////
////

XGraphicPort::~XGraphicPort(void)
{
  if(_resources.Get(XGPR_ZBUFFER))
    FreeZBuffer();

  if(_resources.Get(XGPR_STD_XIMAGE) || _resources.Get(XGPR_SHM_XIMAGE))
    FreeXImage();

  ResetColormaps();

  if(_resources.Get(XGPR_X_ERROR_HANDLER))
    UninstallXErrorHandler();

  if(_resources.Get(XGPR_WINDOW))
    XDestroyWindow(_display, _window);

  if(_resources.Get(XGPR_DISPLAY))
    XCloseDisplay(_display);
}

void XGraphicPort::CommitAttributes(void)
{
  Flags changed;

  changed.SetAll(_last_attributes.GetAll() ^ Attributes().GetAll());

  if(changed.Get(GPA_CMAP) && Attributes().Get(GPA_CMAP))
    ColormapMode();

  if(changed.Get(GPA_RGB)  && Attributes().Get(GPA_RGB))
    RGBMode();

  if(changed.Get(GPA_ZBUFF))
    ZBuffer(Attributes().Get(GPA_ZBUFF));
}

////
////
//
//   Create and open window 
//
////
////


int XGraphicPort::OpenWindow(char *display_name) 
{
  XVisualInfo vinfo;  
  XSizeHints  hint;
  unsigned    int fg, bg;

  _error_code = GPE_OK;

  (*this)[MSG_RESOURCE] << "Opening window\n";

  _display = XOpenDisplay(display_name);
  if (!_display) 
    {
    _error_code = XGPE_DISPLAY;  
    (*this)[MSG_ERROR] << "Cannot open display\n";
    return 0;
    }

  _resources.Set(XGPR_DISPLAY);

  _screen = DefaultScreen(_display);
 
  _resources.Set(XGPR_SCREEN);
 
  hint.width      = _width;
  hint.height     = _height;
  hint.min_width  = MIN_WIDTH;
  hint.min_height = MIN_HEIGHT;
  hint.max_width  = MAX_WIDTH;
  hint.max_height = MAX_HEIGHT;

  hint.flags = PSize | PMinSize | PMaxSize;
  
  // foreground and background
  
  fg = WhitePixel (_display, _screen);
  bg = BlackPixel (_display, _screen);
  
  _gc = XDefaultGC(_display, _screen);

  
  // test display visual class 
 

  // test display visual class 
  // I should get _bytes_per_pixel from X another way.
  // It is set to _ximage->bits_per_pixel >> 3 at the
  // end of the constructor.c

  if(XMatchVisualInfo(_display, _screen, 24, TrueColor, &vinfo))
    {
      (*this)[MSG_INFO]    << "24bpp TrueColor display, you've got the high-score !!\n";
      _bytes_per_pixel = 4;
    }
  else
    if(XMatchVisualInfo(_display, _screen, 16, TrueColor, &vinfo))
      {
	(*this)[MSG_INFO] << "16bpp TrueColor display, waooow !!\n";
	_bytes_per_pixel = 2;
      }
    else
      if(XMatchVisualInfo(_display, _screen, 15, TrueColor, &vinfo))
	{
	  (*this)[MSG_INFO] << "15bpp TrueColor display, waooow !!\n";
	  _bytes_per_pixel = 2;
	}
      else
	if(XMatchVisualInfo(_display, _screen, 8, PseudoColor, &vinfo))
	  {
	    (*this)[MSG_INFO] << "8bpp PseudoColor display\n";
	    _bytes_per_pixel = 1;
	  }
	else
	  if(XMatchVisualInfo(_display, _screen, 8, GrayScale, &vinfo))
	    {
	      (*this)[MSG_INFO] << "8bpp GrayScale display\n";
	      _bytes_per_pixel = 1;
	    }
	  else
	    {
	      _error_code = XGPE_VISUAL;
	      (*this)[MSG_ERROR] << "Wrong visual class, must be either 8,15,16,or 24 bpp\n";
	      return 0;
	    }

  if(_bytes_per_pixel > 1)
    {
      _R_mask = vinfo.red_mask;
      _G_mask = vinfo.green_mask;
      _B_mask = vinfo.blue_mask;
    }
  else
    {
      _R_mask = ((UColorCode)3) << 4;
      _G_mask = ((UColorCode)3) << 2;
      _B_mask = ((UColorCode)3);
    }

  _bits_per_pixel = vinfo.depth;

  // build window

  _window = XCreateSimpleWindow (_display,
				 DefaultRootWindow (_display),
				 hint.x, hint.y,
				 hint.width, hint.height,
				 4, fg, bg);
  
  _resources.Set(XGPR_WINDOW);

  XSelectInput(_display, _window, StructureNotifyMask);
  XSetStandardProperties (_display, 
			  _window, 
			  _name, 
			  _name, None, NULL, 0, &hint);
  

  XMapWindow(_display, _window);
  
  // Wait for window map.
  while(1) 
    {
      XEvent xev;

      XNextEvent(_display, &xev);
      if(xev.type == MapNotify && xev.xmap.event == _window)
	break;
    }

  XSelectInput(_display, _window, KeyPressMask | StructureNotifyMask | ExposureMask 
	       | PointerMotionMask);
  return 1;
}


////
////
//
//   Utilities for colormaps
//
////
////


int XGraphicPort::AllocateColormaps(void)
{

  if(_bits_per_pixel > 8)
    {
      (*this)[MSG_INFO] << "Colormap emulation\n";
      return 1;
    }

  (*this)[MSG_INFO] << "Allocating colormaps\n";

  if(!_resources.Get(XGPR_DISPLAY) || !_resources.Get(XGPR_SCREEN))
    {
      (*this)[MSG_ERROR] << "Cannot alloc cmaps, display and screen not set\n";
      return 0;
    }

  _global_cmap = XDefaultColormap(_display, _screen);
  _resources.Set(XGPR_GLOBAL_CMAP);

  return 1;
}

void XGraphicPort::LocalColormap(void)
{
  XColor xcolor;
  XWindowAttributes xwa;
  int i,r,g,b;


  (*this)[MSG_INFO] << "Allocating local colormap\n";

  XGetWindowAttributes(_display, _window, &xwa);
  _local_cmap  = XCreateColormap(_display, _window, xwa.visual, AllocNone);
  _resources.Set(XGPR_LOCAL_CMAP);
  XSetWindowColormap(_display, _window, _local_cmap);
  _local_cmap_flag = 1;

  xcolor.flags = DoRed | DoGreen | DoBlue;

  for(i=0; i<GP_COLORMAP_SZ; i++)
    if(_colortable[i].Stat().Get(CC_USED))
      {
	_colortable[i].Get(&r,&g,&b);
	xcolor.red   = r << 8;
	xcolor.green = g << 8;
	xcolor.blue  = b << 8;
	if(!XAllocColor(_display, _local_cmap, &xcolor))
	  {
	    (*this)[MSG_ERROR] << "could not allocate color in local cmap (strange)\n";
	    exit(1);
	  }
	_colormap[i] = (ColorIndex)(xcolor.pixel);
      }
}

void XGraphicPort::ResetColortable(void)
{
  int c;

  for(c=0; c<GP_COLORMAP_SZ; c++)
    _colortable[c].Stat().Reset(CC_USED);
}

void XGraphicPort::ResetColormaps(void)
{
  int c;

  if((_error_code != GPE_OK) || _bits_per_pixel > 8)
      return;

  (*this)[MSG_INFO] << "Reseting colormaps\n";

  if(_local_cmap_flag)
    {
      XSetWindowColormap(_display, _window, _global_cmap);
      XFreeColormap(_display, _local_cmap);
      _resources.Reset(XGPR_LOCAL_CMAP);
    }
  else
    for(c=0; c<GP_COLORMAP_SZ; c++)
      {
	unsigned long tmp_pixel;
	if(_colortable[c].Stat().Get(CC_USED))
	  {
	    tmp_pixel = _colormap[c];
	    XFreeColors(_display, _global_cmap, &tmp_pixel, 1, 0);
	  }
      }
  _local_cmap_flag = 0;
}

////
////
//
// XImage utilities
//
////
////

int XGraphicPort::AllocateXImage(void)
{

  if(!_resources.Get(XGPR_DISPLAY))
    {
      (*this)[MSG_ERROR] << "Display not found, cannot create ximage\n";
      return 0;
    }

  if(!_resources.Get(XGPR_X_ERROR_HANDLER))
    {
      InstallXErrorHandler();
      _resources.Set(XGPR_X_ERROR_HANDLER);
    }

#ifdef X_SHM
  if(_shm_flag)
    {
      if(!(_ximage = XShmCreateImage(_display, None, _bits_per_pixel, ZPixmap, 
				     NULL, &_shminfo, _width, _height)))
	{
	  (*this)[MSG_WARNING] << "could not create XShmImage\n";
	  _shm_flag = 0;
	  return AllocateXImage();
	}

      _shminfo.shmid = shmget(IPC_PRIVATE, (_ximage->bytes_per_line *
					    _ximage->height),
			      IPC_CREAT | 0777);

      if(_shminfo.shmid < 0)
	{
	  _shm_flag = 0;
	  XDestroyImage(_ximage);
	  return AllocateXImage();
	}

      _shminfo.shmaddr = (char *)shmat(_shminfo.shmid, 0, 0);

      if (_shminfo.shmaddr == ((char *) -1)) 
	{
	  _shm_flag = 0;
	  XDestroyImage(_ximage);
	  return AllocateXImage();
	}

      _ximage->data = _shminfo.shmaddr;
      _graph_mem    = (ColorIndex *) _ximage->data;
      _shminfo.readOnly = False;

      XShmAttach(_display, &_shminfo);
      XSync(_display, False);

      if (_xerror_flag) 
	{
	  XDestroyImage(_ximage);
	  _shm_flag    = 0;
	  _xerror_flag = 0;
	  return AllocateXImage();
	} 

      shmctl(_shminfo.shmid, IPC_RMID, 0);
      _resources.Set(XGPR_SHM_XIMAGE);

      (*this)[MSG_RESOURCE] << "Shm XImage created\n";
      return 1;
    }
  else
    {
      (*this)[MSG_INFO] << "Switching to native X calls\n";
#endif

      if(!(_ximage = XCreateImage(_display, None,  _bits_per_pixel, ZPixmap, 0, NULL,
				  _width, _height, _bytes_per_pixel*8, 0)))
	{
	  (*this)[MSG_ERROR] << "Could not create XImage\n";
	  _error_code = XGPE_XIMAGE;
	  return 0;
	}

      if(!(_ximage->data = (char *)malloc(_height*_width*_bytes_per_pixel)))
	{
	  (*this)[MSG_ERROR] << "Could not alloc img buffer\n";
	  _error_code = XGPE_MALLOC;
	  return 0;
	}
      
      _graph_mem = (unsigned char *)(_ximage->data);
      _resources.Set(XGPR_STD_XIMAGE);
      (*this)[MSG_RESOURCE] << "Std XImage created\n";      
      return 1;
      
#ifdef X_SHM
    }
#endif

if(_resources.Get(XGPR_X_ERROR_HANDLER))
  UninstallXErrorHandler();

}

void XGraphicPort::FreeXImage(void)
{

#ifdef X_SHM
  if(_resources.Get(XGPR_SHM_XIMAGE))
    {
      XShmDetach(_display,&_shminfo);
      XDestroyImage(_ximage);
      shmdt(_shminfo.shmaddr);
      _resources.Reset(XGPR_SHM_XIMAGE);
      (*this)[MSG_RESOURCE] << "Shm XImage destroyed\n";
    }
#endif

  if(_resources.Get(XGPR_STD_XIMAGE))
    {
      XDestroyImage(_ximage);
      _resources.Reset(XGPR_STD_XIMAGE);
      (*this)[MSG_RESOURCE] << "Std XImage destroyed\n";
    }
}


////
////
//
// ZBuffer 
//
////
////

int XGraphicPort::AllocateZBuffer(void)
{
  if(_resources.Get(XGPR_ZBUFFER))
    {
      (*this)[MSG_WARNING] << "ZBuffer already allocated\n";
      return 1;
    }

  if(!(_z_mem = new ZCoord[_width * _height]))
    {
      (*this)[MSG_ERROR] << "could not alloc ZBuffer\n";
      _error_code = XGPE_MALLOC;
      return 0;
    }

  _resources.Set(XGPR_ZBUFFER);

  (*this)[MSG_RESOURCE] << "ZBuffer allocated\n";
  return 1;
}

void XGraphicPort::FreeZBuffer(void)
{
  (*this)[MSG_RESOURCE] << "Freeing ZBuffer\n";

  if(_resources.Get(XGPR_ZBUFFER))
    delete[] _z_mem;

  _resources.Reset(XGPR_ZBUFFER);
}


////
////
//
// Resize frame buffer & Z buffer
//
////
////


int XGraphicPort::Resize(const ScrCoord width, const ScrCoord height)
{
  int retval = 1;

  (*this)[MSG_INFO] << "Resizing to " << width << "x" << height << "\n";

  _width  = width;
  _height = height;

  if(_resources.Get(XGPR_STD_XIMAGE) || _resources.Get(XGPR_SHM_XIMAGE))
    {
      FreeXImage();
      retval = AllocateXImage();
    }

  if(_resources.Get(XGPR_STD_XIMAGE) || _resources.Get(XGPR_SHM_XIMAGE))
    _bytes_per_line = _ximage->bytes_per_line;

  if(_resources.Get(XGPR_ZBUFFER))
    {
      FreeZBuffer();
      retval &= AllocateZBuffer();
    }

  _clip.Set(0, 0, _width - 1, _height - 1);

  return retval;
}


////
////
//
// High level functions
//
////
////

void XGraphicPort::MapColor(const ColorIndex idx, 
			    const ColorComponent r, 
			    const ColorComponent g, 
			    const ColorComponent b )
{
  XColor xcolor;

  StoreColor(idx,r,g,b);

  if(_bits_per_pixel > 8)
    {
      _colortable[idx].Stat().Set(CC_USED);
      _colortable[idx].Set(r,g,b);
      return;
    }

  xcolor.flags = DoRed | DoGreen | DoBlue;
  xcolor.red   = r << 8;
  xcolor.green = g << 8;
  xcolor.blue  = b << 8;
  xcolor.pixel = idx;

  if(_local_cmap_flag)
    {
      if(_colortable[idx].Stat().Get(CC_USED))
	{
	  unsigned long tmp_pixel = _colormap[idx];
	  XFreeColors(_display, _local_cmap, &tmp_pixel, 1, 0);
	}

      if(!XAllocColor(_display, _local_cmap, &xcolor))
	{
	  (*this)[MSG_ERROR] << "could not allocate color in local cmap (panic!)\n";
	  exit(1);
	}

      _colormap[idx] = (ColorIndex)(xcolor.pixel);
    }
  else
    {
      if(_colortable[idx].Stat().Get(CC_USED))
	{
	  unsigned long tmp_pixel = _colormap[idx];
	  XFreeColors(_display, _global_cmap, &tmp_pixel, 1, 0);
	}

      if(XAllocColor(_display, _global_cmap, &xcolor))
	  _colormap[idx] = (ColorIndex)(xcolor.pixel);
      else
	{
	  (*this)[MSG_INFO] << "switching to local colormap mode\n";
	  ResetColormaps();
	  LocalColormap();
	  MapColor(idx,r,g,b);
	}
    }

  _colortable[idx].Set(r,g,b);
  _colortable[idx].Stat().Set(CC_USED);
  _truecolormap[idx] = _colormap[idx];

}  

void XGraphicPort::RGBMode(void)
{
  int r,g,b;

  (*this)[MSG_INFO] << "Switching to RGB mode\n";

  ResetColormaps();
  ResetColortable();

  if(_bits_per_pixel == 8)
    for(r=0; r<4; r++)
      for(g=0; g<4; g++)
        for(b=0; b<4; b++)
	  MapColor(b + (g << 2) + (r << 4), r << 6, g << 6, b << 6);

  Attributes().Set(GPA_RGB);
  Attributes().Reset(GPA_CMAP);
}

void XGraphicPort::ColormapMode(void)
{

  (*this)[MSG_INFO] << "Switching to colormap mode\n";

  ResetColormaps();
  ResetColortable();

// default colors

  MapColor(BLACK,   0,   0,   0   );
  MapColor(RED,     255, 0,   0   );
  MapColor(GREEN,   0,   255, 0   );
  MapColor(YELLOW,  255, 255, 0   );
  MapColor(BLUE,    0,   0,   255 );
  MapColor(MAGENTA, 255, 0,   255 );
  MapColor(CYAN,    0,   255, 255 );
  MapColor(WHITE,   255, 255, 255 );

  Attributes().Set(GPA_CMAP);
  Attributes().Reset(GPA_RGB);
}


int XGraphicPort::SingleBuffer(void)
{
  (*this)[MSG_WARNING] << "SingleBuffer mode not available for X\n";
  return 0;
}

int XGraphicPort::DoubleBuffer(void)
{
  (*this)[MSG_WARNING] << "You are ALWAYS in DoubleBuffer mode under X\n";
  return 1;
}

int XGraphicPort::SwapBuffers(void)
{

  XEvent xev;

#ifdef X_SHM

  if (_shm_flag) 
    {

      XShmPutImage(_display, _window, _gc, 
		     _ximage, 
		     0, 0, 0, 0,
		     _width, _height, True);

      XFlush(_display);
      while(1) 
	{
	  XNextEvent(_display, &xev);
	  if(xev.type == _completion_type)
	    break;

	  else
	    if (xev.type == KeyPress)
	      _buffered = 
		(XLookupString((XKeyEvent *)&xev, 
			       &_charbuff, 1, NULL, NULL) > 0);
	    else
	      if (xev.type == ConfigureNotify)
		{
		  int w  = ((XConfigureEvent *)&xev)->width;
		  int h  = ((XConfigureEvent *)&xev)->height;
		  if((w != _width) || (h != _height))
		    Resize(w, h);
		}
	}
    }
  else 
    {
#endif
      XPutImage(_display, _window, _gc, _ximage, 0, 0, 0, 0, _width, _height);
      if (!XCheckWindowEvent(_display, _window, StructureNotifyMask, &xev))
	return 1;

      if(xev.type == ConfigureNotify)
	{
	  int w  = ((XConfigureEvent *)&xev)->width;
	  int h  = ((XConfigureEvent *)&xev)->height;
	  if((w != _width) || (h != _height))
	    Resize(w, h);
	}
#ifdef X_SHM
    }
#endif
  return 1;
}


Bool XGraphicPort::predproc(Display *display, XEvent *event, char *arg)
{
  if(event->xany.window == *(Window *)arg)
    return True;
  else
    return False;
}

int XGraphicPort::WaitEvent(void)
{
  XEvent event;
  XPeekIfEvent(_display, &event, predproc, (char *)&_window);
  return event.type;
}

char XGraphicPort::GetKey(void)
{
  char	q;
  XEvent xev;
  
  if(_buffered)
    {
      _buffered = 0;
      return _charbuff;
    }

  if (!XCheckWindowEvent(_display, _window, KeyPressMask, &xev))
    return 0;
  
  if ((xev.type == KeyPress) && (XLookupString((XKeyEvent *)&xev, &q, 1, NULL, NULL) > 0))
    return q;
	
  return 0;
}

int  XGraphicPort::GetMouse(ScrCoord *x, ScrCoord *y)
{
  Window	rootw, childw;
  int		sx, sy;
  unsigned int	mask;

  XQueryPointer(_display, _window, &rootw, &childw, &sx, &sy, x, y, 
		&mask);

  return(mask >> 8);
}

int XGraphicPort::ZBuffer(const int yes)
{
  int result;
  if(yes)
    {
      result = AllocateZBuffer();
      Attributes().Set(GPA_ZBUFF);
    }
  else
    {
      result = 1;
      FreeZBuffer();
      Attributes().Reset(GPA_ZBUFF);
    }
  return result;
}
  

int XGraphicPort::SetGeometry(const ScrCoord width, const ScrCoord height)
{
  int retval;

  if((width == _width) && (height == _height))
    return 1;

  retval = Resize(width, height);
  XResizeWindow(_display, _window, _width, _height);
  
  // Wait for window resize.
  while(1) 
    {
      XEvent xev;

      XNextEvent(_display, &xev);
      if(xev.type == Expose && xev.xmap.event == _window && !xev.xexpose.count)
	break;
    }

  return retval;
}


////
////
//
// Virtual constructor stuff
//
////
////


GraphicPort* XGraphicPort::Make(char *name, ScrCoord width, ScrCoord height, 
				int verb)
{
  XGraphicPort *GP = new XGraphicPort(name, width, height, verb);
  if(GP->ErrorCode())
    {
      delete GP;
      return NULL;
    }
  return GP;
}

// The only purpose of this dummy class is to have its constructor
// called before the beginning of main(), so that XGraphicPort::Make
// is registered to the virtual constructor.

static class Stub
{
public:
  Stub(void)
    { 
      GraphicPort::Register(XGraphicPort::Make,GP_VC_NORMAL); 
    } 
} Dummy;



CFLAGS= 
#Wstrict-prototypes 
CC =gcc -O3 $(CFLAGS) -g -I. -I../XGUI -I/usr/X11/include -Wno-error=implicit-function-declaration
CC2 =gcc -O3 -I. -I../XGUI -I/usr/X11/include
AR= ar
RM= rm
# Make's implicit rules for making a .o file from a .c file...
#
RAYFILE = Vector.c Vector.h VRStruct.h VRay.c VRay.h\
	  Display.h Display.c Parse.c Parse.h libVRay.a\
          Intersect.c Trace.c Shade.c Bgcolor.c Mapping.c\
	  BVH.c BVH.h \
	  Makefile XGUI 

RAYOBJ  = Mapping.o Bgcolor.o Intersect.o Shade.o Trace.o\
	  Parse.o Vector.o Display.o BVH.o

all:    $(RAYOBJ)

lib: $(RAYOBJ)
	rm -f libVRay.a 
	$(AR) -r libVRay.a $(RAYOBJ)

exe: 
	$(CC2) VRay.c -o VRay -I. -I/usr/X11/include -L../XGUI -L. -L/usr/X11/lib -lVRay -lXGUI -lX11 -lXext -lm 

clean:
	rm -f $(RAYOBJ) *.a VRay
tar:
	tar -cf VRay.tar $(RAYFILE)
list:
	ls $(RAYFILE)

CC = gcc -O2 -fcommon -DX_SHM -I. -I./VertexRay -I./XGUI -I/usr/X11/include -I/usr/X11/include/X11  -I/usr/include/X11 -Wno-error=implicit-function-declaration
AR= ar
RM= rm
TMPDIR=./tmp/
# Make's implicit rules for making a .o file from a .c file...
#
VERTOBJ = VertexGUI.o VertexMount.o VertexPyramid.o VertexCyl.o \
	  VertexCone.o VertexTorus.o VertexSphere.o VertexCube.o \
	  VertexSurf.o VertexDisk.o VertexAnim.o VertexRevol.o \
	  VertexOutLine.o VertexSolid.o VertexGouraud.o VertexPhong.o \
	  VertexEdit.o VertexGLOB.o VertexOper.o VertexFile.o \
	  VertexColor.o VertexMemory.o VertexDisplay.o VertexMat.o

all:    $(VERTOBJ)
     
lib: $(VERTOBJ)
	rm -f libVertex.a
	$(AR) -r libVertex.a $(VERTOBJ)

exe: VertexMain.c lib
	$(CC) VertexMain.c -g -o Vertex -L/usr/X11/lib -L./VertexRay -L./XGUI -L. \
	-lVertex -lVRay -lXGUI -lm -lXpm -lX11 -lXext

clean:
	rm -f $(VERTOBJ) libVertex.a Vertex

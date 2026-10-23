CFLAGS=	 
CC = gcc -O2 -I. -Wno-error=implicit-function-declaration
AR= ar
RM= rm -f
# Make's implicit rules for making a .o file from a .c file...
#
GUIFILE = Connect.c XShell.c Menu.c General.c Button.c Scroll.c \
	  Text.c List.c Toggle.c Label.c File.c Drawing.c \
	  CheckBox.c Palette.c graph.c ess.c coucou2.c fract.c \
          Makefile Makefile.std comp XGUI.h Connect.h \
          scrolltest.c scrollmath_test.c filetest.c

GUIOBJ  = Connect.o File.o XShell.o Menu.o General.o Button.o \
          Text.o List.o Scroll.o Toggle.o Label.o Palette.o \
	  CheckBox.o Drawing.o

GUITESTFILE = graph coucou2 ess

all:    $(GUIOBJ)
     
lib: $(GUIOBJ)
	rm -f libXGUI.a
	$(AR) -r libXGUI.a $(GUIOBJ)
clean:
	-rm $(GUIOBJ) libXGUI.a
tar:
	rm -f G*.tar g*.tar
	tar -cf guilib.tar $(GUIFILE)
list:
	ls $(GUIFILE)

test:	$(GUITESTFILE)

check:	lib scrollmath
	./scrollmath

filetest: filetest.c lib
	$(CC) -g -o filetest filetest.c -L. -lXGUI -lXpm -lX11 -lm

scrollmath: scrollmath_test.c Scroll.c
	$(CC) -o scrollmath scrollmath_test.c -lX11 -lm

graph:  graph.c lib
	$(CC) -g -o graph graph.c -L. -lXGUI -lXpm -lXt -lX11 -lm 

coucou2: coucou2.c lib
	$(CC) -g -o coucou2 coucou2.c -L. -lXGUI -lXpm -lXt -lX11 -lm

ess: ess.o lib
	$(CC) -g -o ess ess.c -L. -lXGUI -lXpm -lXt -lX11 -lm

fract: fract.o lib
	$(CC) -g -o fract fract.c -L. -lXGUI -lXpm -lXt -lX11 -lm

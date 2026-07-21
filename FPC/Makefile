CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -ll -lm

.PHONY: all clean

all: fpc fpcinter

fpc: fpc.tab.c lex.yy.c listid.c fpcstr.h listid.h
	flex -I fpc.l
	bison -d fpc.y
	$(CC) $(CFLAGS) -o fpc fpc.tab.c $(LDFLAGS)

fpcinter: fpc.tab.h fpcinter.c fpcstr.h
	$(CC) $(CFLAGS) -o fpcinter fpcinter.c -lm

lex.yy.c: fpc.l
	flex -I fpc.l

fpc.tab.c fpc.tab.h: fpc.y
	bison -d fpc.y

clean:
	rm -f fpc fpcinter fpc.tab.c fpc.tab.h lex.yy.c

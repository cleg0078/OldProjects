
CC = gcc
LEX = flex
YACC = bison

LEX_SRC = expr.l
YACC_SRC = expr.y

LEX_OUT = lex.yy.c
YACC_OUT_C = expr.tab.c
YACC_OUT_H = expr.tab.h

TARGET = deriv

all: $(TARGET)

$(YACC_OUT_C) $(YACC_OUT_H): $(YACC_SRC)
	$(YACC) -d $(YACC_SRC)

$(LEX_OUT): $(LEX_SRC)
	$(LEX) $(LEX_SRC)

$(TARGET): $(YACC_OUT_C) $(LEX_OUT)
	$(CC) -g -o $(TARGET) $(YACC_OUT_C) $(LEX_OUT) -lfl

release: $(YACC_OUT_C) $(LEX_OUT)
	$(CC) -O2 -o $(TARGET) $(YACC_OUT_C) $(LEX_OUT) -lfl -D__RELEASE

clean:
	rm -f $(TARGET) $(LEX_OUT) $(YACC_OUT_C) $(YACC_OUT_H)

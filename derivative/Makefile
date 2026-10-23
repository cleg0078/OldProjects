
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
	rm -f $(TARGET)
	$(CC) -g -Wall -Wno-unused-function -o $(TARGET) $(YACC_OUT_C) $(LEX_OUT) -lfl

release: $(YACC_OUT_C) $(LEX_OUT)
	rm -f $(TARGET)
	$(CC) -O2 -Wall -Wno-unused-function -o $(TARGET) $(YACC_OUT_C) $(LEX_OUT) -lfl -D__RELEASE

clean:
	rm -f $(TARGET) $(LEX_OUT) $(YACC_OUT_C) $(YACC_OUT_H) expr.output *.gcno *.gcda

demo: $(YACC_OUT_C) $(LEX_OUT)
	rm -f $(TARGET)
	$(CC) -g -Wall -Wno-unused-function -o $(TARGET) $(YACC_OUT_C) $(LEX_OUT) -lfl

test:
	./run_tests.sh
	./run_error_tests.sh

coverage:
	./tests/coverage.sh

test-all: test
	@if command -v python3 >/dev/null 2>&1; then \
		python3 tests/oracle_check.py 200; \
	else \
		echo "SKIP oracle: python3 not installed"; \
	fi
	./tests/coverage.sh

results:
	./tests/gen_results.sh

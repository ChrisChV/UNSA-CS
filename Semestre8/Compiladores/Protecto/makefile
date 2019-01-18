lexico:
	lex -t Main/lexico.l > Main/lex.yy.c

sintactico:
	yacc -d -v Main/sintactico.y -o Main/y.tab.c

main:
	gcc -o Main/run Main/lex.yy.c Main/y.tab.c -ly -ll -lm

clean:
	rm -f Main/lex.yy.c
	rm -f Main/y.tab.c
	rm -f Main/y.h
	rm -f Main/y.output

all:
	make clean
	make lexico
	make sintactico
	make main





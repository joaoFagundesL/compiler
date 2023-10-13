all: nag

nag.tab.c nag.tab.h: nag.y
	bison -t -v -d nag.y

lex.yy.c: nag.l nag.tab.h
	flex nag.l

nag: lex.yy.c nag.tab.c nag.tab.h
	gcc nag.h nag.c -o nag nag.tab.c lex.yy.c -g

clean:
	rm nag nag.tab.h nag.tab.c lex.yy.c nag.output

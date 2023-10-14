nag: nag.y nag.l
	bison -d nag.y
	flex nag.l
	gcc nag.c -o nag nag.tab.c lex.yy.c -g

clean:
	rm -f nag nag.tab.h nag.tab.c lex.yy.c nag.output aslgenerated/*.asl

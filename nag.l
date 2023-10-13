%{ 
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "nag.h"
    #include "nag.tab.h"
%}

%option yylineno
%option noyywrap

LETTER [a-zA-Z_]
DIGIT [0-9]
ID {LETTER}({LETTER}|{DIGIT})*

%%

"crencas:"			{ return CRENCAS; }
"objetivos:"	    { return OBJETIVOS; }
"planos:"	        { return PLANOS; }
"E"|"OU"|"NAO"      { yylval.op = strdup(yytext); return OPERADOR; }
"("|")"|"{"|"}"|";"	{ return yytext[0]; }
{ID}		        { yylval.id = strdup(yytext); return ID; }
"\n"	            { return EOL; }
<<EOF>>             { return 0; }
[ \t]		        ;
.			        { printf("Unknown character: %s\n", yytext); return 0; }

%%

extern FILE *yyin;  

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "[!] Use mode: %s <FILE>", argv[0]);
		return EXIT_FAILURE;
	}

	yyin = fopen(argv[1], "r");

	if (!yyin) {
		fprintf(stderr, "[!] File not found.\n");
		return EXIT_FAILURE;
	}

	yyparse();
}


%{ 
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "nag.h"
    #include "nag.tab.h"
%}

%option yylineno
%option noyywrap

%%

"crencas:"			{ return CRENCAS; }
"objetivos:"	    { return OBJETIVOS; }
"planos:"	        { return PLANOS; }
"E"|"OU"|"NAO"      { yylval.operator = strdup(yytext); return OPERATOR; }
"("|")"|"{"|"}"|";"	{ return yytext[0]; }
[a-zA-Z_][a-zA-Z0-9_]*		        { yylval.identifier = strdup(yytext); return IDENTIFIER; }
"\n"	            { return END_OF_LINE; }
<<EOF>>             { return 0; }
[ \t]		        ;
.			        { fprintf(stderr, "Unknown character '%s' at line %d\n", yytext, yylineno); return 0; }

%%

extern FILE *yyin;  

int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      fprintf (stderr, "[!] Use mode: %s <FILE>", argv[0]);
      return EXIT_FAILURE;
    }

  yyin = fopen (argv[1], "r");

  if (!yyin)
    {
      fprintf (stderr, "[!] File not found.\n");
      return EXIT_FAILURE;
    }

  yyparse ();
  fclose (yyin);

  return EXIT_SUCCESS;
}

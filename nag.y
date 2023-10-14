%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "nag.h"
    #include "nag.tab.h"

    struct head *custom_agent;
    char *evt;
    struct expr *exp_curr;
    struct list *l = NULL;
    
    int yylex();
%}

%union {
    char *operator;
    char *identifier;
}

%token <identifier> IDENTIFIER
%token <operator> OPERATOR
%token PLANOS CRENCAS OBJETIVOS
%token END_OF_LINE
%token LCURLY RCURLY LPAREN RPAREN SEMI

%start list_stmt

%%

nome_crenca: IDENTIFIER { set_new_belief(custom_agent, $1); }
    ;

crencas: 
	| nome_crenca SEMI crencas 
    | LCURLY crencas RCURLY  
	;

nome_objetivo: IDENTIFIER { set_new_goal(custom_agent, $1); }
    ;

objetivos: 
	| nome_objetivo SEMI objetivos 
    | LCURLY objetivos RCURLY
	;

event_trigger: IDENTIFIER              { evt = $1; }
    ;

logic_exp: IDENTIFIER OPERATOR IDENTIFIER	{ exp_curr = create_exp($1, $2, $3); }
	| OPERATOR IDENTIFIER               { exp_curr = create_exp("", $1, $2); }  
    ;

contexto: 
    | IDENTIFIER                        { exp_curr = create_exp("", "", $1); }
    | logic_exp	          
    ;

set_structure: 
    | IDENTIFIER SEMI set_structure { l = set_new_list(l, $1); }     
    ;

str: LCURLY set_structure RCURLY
    ;

nome_plano: IDENTIFIER LPAREN event_trigger SEMI contexto SEMI str RPAREN 
            { 
                set_new_plan(custom_agent, $1, evt, exp_curr, l); 
                l = NULL;
            }
    ;

planos: 
    | nome_plano SEMI planos
    | LCURLY planos RCURLY	       
    ;

nome_agente: IDENTIFIER { custom_agent = set_new_agent($1); }
    ;

stmt:                         
	| PLANOS LCURLY planos RCURLY 
	| OBJETIVOS	LCURLY objetivos RCURLY
    | nome_agente CRENCAS LCURLY crencas RCURLY
	;

end_of_program:
    {
        free_agent(custom_agent);
    }
    ;


list_stmt: { generate_jason_file(custom_agent); }
	| stmt END_OF_LINE list_stmt
	| '0' end_of_program
	;

%%
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

%start list_stmt

%%

nome_crenca: IDENTIFIER { set_new_belief(custom_agent, $1); }
    ;

crencas: 
	| nome_crenca ';' crencas 
    | '{' crencas '}'  
	;

nome_objetivo: IDENTIFIER { set_new_goal(custom_agent, $1); }
    ;

objetivos: 
	| nome_objetivo ';' objetivos 
    | '{' objetivos '}'
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
    | IDENTIFIER ';' set_structure { l = set_new_list(l, $1); }     
    ;

str: '{' set_structure '}'
    ;

nome_plano: IDENTIFIER '(' event_trigger ';' contexto ';' str ')' 
            { 
                set_new_plan(custom_agent, $1, evt, exp_curr, l); 
                l = NULL;
            }
    ;

planos: 
    | nome_plano ';' planos
    | '{' planos '}'	       
    ;

nome_agente: IDENTIFIER { custom_agent = set_new_agent($1); }
    ;

stmt:                         
	| PLANOS '{' planos '}' 
	| OBJETIVOS	'{' objetivos '}'
    | nome_agente CRENCAS '{' crencas '}'
	;

list_stmt: { generate_jason_file(custom_agent); }
	| stmt END_OF_LINE list_stmt
	| '0'
	;

%%
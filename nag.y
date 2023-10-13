%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "nag.h"
    #include "nag.tab.h"

    struct head *agent;
    char *evt;
    struct expr *exp_curr;
    struct list *l = NULL;
    
    int yylex();
%}

%union {
    char *op;
    char *id;
}

%token <id> ID
%token <op> OPERADOR
%token PLANOS CRENCAS OBJETIVOS
%token EOL

%start list_stmt

%%

nome_crenca: ID { set_new_belief(agent, $1); }
    ;

crencas: 
	| nome_crenca ';' crencas 
    | '{' crencas '}'  
	;

nome_objetivo: ID { set_new_goal(agent, $1); }
    ;

objetivos: 
	| nome_objetivo ';' objetivos 
    | '{' objetivos '}'
	;

event_trigger: ID              { evt = $1; }
    ;

logic_exp: ID OPERADOR ID	{ exp_curr = create_exp($1, $2, $3); }
	| OPERADOR ID               { exp_curr = create_exp("", $1, $2); }  
    ;

contexto: 
    | ID                        { exp_curr = create_exp("", "", $1); }
    | logic_exp	          
    ;

set_structure: 
    | ID ';' set_structure { l = set_new_list(l, $1); }     
    ;

str: '{' set_structure '}'
    ;

nome_plano: ID '(' event_trigger ';' contexto ';' str ')' 
            { 
                set_new_plan(agent, $1, evt, exp_curr, l); 
                l = NULL;
            }
    ;

planos: 
    | nome_plano ';' planos
    | '{' planos '}'	       
    ;

nome_agente: ID { agent = set_new_agent($1); }
    ;

stmt:                         
	| PLANOS '{' planos '}' 
	| OBJETIVOS	'{' objetivos '}'
    | nome_agente CRENCAS '{' crencas '}'
	;

list_stmt: { generate_jason_file(agent); }
	| stmt EOL list_stmt
	| '0'
	;

%%
%{
	/* Analisador Sintático TradutorNAG_Jason */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "nag.h"
    #include "nag.tab.h"
    int yylex();

    struct head *agent;
    char *currentEventoGat;
    struct expr *currentExp;
    struct list *currentCorpo = NULL;
%}

%union {
    char *id;
    char *op;
}

%token <id> ID
%token <op> OPERADOR
%token CRENCAS OBJETIVOS PLANOS
%token EOL

%start list_stmt
%%

/* LIDANDO COM AS CRENCAS */

nome_crenca: ID { set_new_belief(agent, $1); }
    ;

crencas: /* Vazio */
	| nome_crenca ';' crencas 
    | '{' crencas '}'  
	;

/* LIDANDO COM OS OBJETIVOS */

nome_objetivo: ID { set_new_goal(agent, $1); }
    ;

objetivos: /* Vazio */
	| nome_objetivo ';' objetivos 
    | '{' objetivos '}'
	;

/* LIDANDO COM OS PLANOS */

event_trigger: ID              { currentEventoGat = $1; }
    ;

expressaoLogica: ID OPERADOR ID	{ currentExp = create_exp($1, $2, $3); }
	| OPERADOR ID               { currentExp = create_exp("", $1, $2); }  
    ;

contexto: /* Vazio */
    | ID                        { currentExp = create_exp("", "", $1); }
    | expressaoLogica	          
    ;

formula_corpo: /* Vazio */
    | ID ';' formula_corpo { currentCorpo = list(currentCorpo, $1); }     
    ;

corpo: '{' formula_corpo '}'
    ;

nome_plano: ID '(' event_trigger ';' contexto ';' corpo ')' 
            { 
                set_new_plan(agent, $1, currentEventoGat, currentExp, currentCorpo); 
                currentCorpo = NULL;
            }
    ;

planos: /* Vazio */
    | nome_plano ';' planos
    | '{' planos '}'	       
    ;

/* LIDANDO COM OS COMANDOS */ 

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



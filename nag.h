
#ifndef NAG_H
#define NAG_H

#define BUFFER 50
#include <stdio.h>

extern int yylineno;

struct list
{
  char name[BUFFER];
  struct list *next;
};

struct node
{
  char name[BUFFER];
  char event_trigger[BUFFER];
  struct expr *ctx_expr;
  struct list *list;
  struct node *next;
};

struct head
{
  char name_agent[BUFFER];
  struct node *crencas;
  struct node *objetivos;
  struct node *planos;
};

struct expr
{
  char contexto1[BUFFER];
  char complemento[BUFFER];
  char contexto2[BUFFER];
};


void free_agent(struct head *a);
static void free_beliefs(struct node *belief);
static void free_goals(struct node *goal);
static void free_plans(struct node *plan);

struct head *set_new_agent (const char *name);
void yyerror (char *s);
void set_new_belief (struct head *a, const char *name);
void set_new_goal (struct head *a, const char *name);

void set_new_plan (struct head *a, const char *name, const char *eg,
		   struct expr *exp, struct list *list);

struct expr *create_exp (const char *name1, const char *comp,
			 const char *name2);

void generate_jason_file (struct head *a);
struct list *set_new_list (struct list *cl, const char *name);

static void set_agent_name (struct head *tmp_agent, const char *name);
static void print_agent_details (struct head *tmp_agent);
static void initialize_agent (struct head *tmp_agent);
static struct node *create_belief (const char *name);
static void add_belief_to_agent (struct head *agent, struct node *belief);
static struct node *allocate_goal_node (const char *name);
static void add_new_goal (struct head *a, struct node *new);
static void print_goal_details (struct node *new);

static struct node *allocate_plan_node (const char *name, const char *eg,
					struct expr *exp, struct list *list);

static void print_plan_details (struct node *new);
static void add_new_plan (struct head *a, struct node *new);
static void print_expression_details (const struct expr *exp);
static void set_context2 (struct expr *exp, const char *name2);
static void set_complement (struct expr *exp, const char *comp);
static void set_context1 (struct expr *exp, const char *name1);
static struct expr *allocate_expression ();

static struct list *add_list_to_list (struct list *new_list,
				      struct list *existing_list);

static struct list *create_list (const char *name);
static void write_beliefs (FILE * fptr, const struct node *crencas);
static void write_plans (FILE * fptr, const struct node *planos);

#endif

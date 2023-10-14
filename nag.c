
#include "nag.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yyparse ();
extern int yylex ();
extern int yylineno;
extern char *yytext;

void
yyerror (char *s)
{
  fprintf (stderr, "%s: error: ", s);
}

struct head *
allocate_agent (const char *name)
{
  struct head *tmp_agent = (struct head *) malloc (sizeof (struct head));
  if (!tmp_agent)
    {
      yyerror ("[!] Memory allocation failure");
      exit (EXIT_FAILURE);
    }

  strncpy (tmp_agent->name_agent, name, sizeof (tmp_agent->name_agent) - 1);
  tmp_agent->name_agent[sizeof (tmp_agent->name_agent) - 1] = '\0';

  tmp_agent->crencas = tmp_agent->objetivos = tmp_agent->planos = NULL;

  return tmp_agent;
}

void
initialize_agent (struct head *tmp_agent)
{
  tmp_agent->crencas = tmp_agent->objetivos = tmp_agent->planos = NULL;
}

void
set_agent_name (struct head *tmp_agent, const char *name)
{
  strncpy (tmp_agent->name_agent, name, sizeof (tmp_agent->name_agent) - 1);
  tmp_agent->name_agent[sizeof (tmp_agent->name_agent) - 1] = '\0';
}

void
print_agent_details (struct head *tmp_agent)
{
  printf ("AGENTE \t\t<%s>\n", tmp_agent->name_agent);
}

struct head *
set_new_agent (const char *name)
{
  struct head *tmp_agent = allocate_agent (name);
  initialize_agent (tmp_agent);
  set_agent_name (tmp_agent, name);
  print_agent_details (tmp_agent);
  return tmp_agent;
}

struct node *
allocate_goal_node (const char *name)
{
  struct node *new = (struct node *) malloc (sizeof (struct node));
  if (!new)
    {
      yyerror ("[!] Memory allocation failed.\n");
      exit (EXIT_FAILURE);
    }

  strcpy (new->name, name);

  return new;
}

void
add_new_goal (struct head *a, struct node *new)
{
  new->next = a->objetivos;
  a->objetivos = new;
}

void
print_goal_details (struct node *new)
{
  printf ("OBJETIVO \t<%s>\n", new->name);
}

void
set_new_goal (struct head *a, const char *name)
{
  struct node *new = allocate_goal_node (name);
  add_new_goal (a, new);
  print_goal_details (new);
}

struct node *
allocate_plan_node (const char *name, const char *eg, struct expr *exp,
		    struct list *list)
{
  struct node *new = (struct node *) malloc (sizeof (struct node));
  if (!new)
    {
      yyerror ("[!] Memory allocation failed.\n");
      exit (EXIT_FAILURE);
    }

  strncpy (new->name, name, sizeof (new->name) - 1);
  new->name[sizeof (new->name) - 1] = '\0';

  strncpy (new->event_trigger, eg, sizeof (new->event_trigger) - 1);
  new->event_trigger[sizeof (new->event_trigger) - 1] = '\0';

  new->ctx_expr = exp;
  new->list = list;

  return new;
}

void
add_new_plan (struct head *a, struct node *new)
{
  new->next = a->planos;
  a->planos = new;
}

void
print_plan_details (struct node *new)
{
  printf (" PLANO <%s> : GATILHO -> %s | CONTEXTO -> %s %s %s | CORPO -> ",
	  new->name, new->event_trigger, new->ctx_expr->contexto1,
	  new->ctx_expr->complemento, new->ctx_expr->contexto2);

  for (struct list * tmp_list = new->list; tmp_list != NULL;
       tmp_list = tmp_list->next)
    {
      printf ("<%s> ", tmp_list->name);
    }

  printf ("\n");
}

void
set_new_plan (struct head *a, const char *name, const char *eg,
	      struct expr *exp, struct list *list)
{
  struct node *new = allocate_plan_node (name, eg, exp, list);
  add_new_plan (a, new);
  print_plan_details (new);
}

struct node *
create_belief (const char *name)
{
  struct node *new_belief = (struct node *) malloc (sizeof (struct node));
  if (!new_belief)
    {
      yyerror ("[!] Memory allocation failure");
      exit (EXIT_FAILURE);
    }

  strncpy (new_belief->name, name, sizeof (new_belief->name) - 1);
  new_belief->name[sizeof (new_belief->name) - 1] = '\0';
  new_belief->next = NULL;

  return new_belief;
}

void
add_belief_to_agent (struct head *agent, struct node *belief)
{
  belief->next = agent->crencas;
  agent->crencas = belief;
}

void
set_new_belief (struct head *agent, const char *name)
{
  struct node *new_belief = create_belief (name);
  add_belief_to_agent (agent, new_belief);
  printf ("CRENCA \t\t<%s>\n", new_belief->name);
}

struct expr *
allocate_expression ()
{
  struct expr *new = (struct expr *) malloc (sizeof (struct expr));
  if (!new)
    {
      yyerror ("[!] Memory allocation failure");
      exit (EXIT_FAILURE);
    }

  return new;
}

void
set_context1 (struct expr *exp, const char *name1)
{
  strncpy (exp->contexto1, name1, sizeof (exp->contexto1) - 1);
  exp->contexto1[sizeof (exp->contexto1) - 1] = '\0';
}

void
set_complement (struct expr *exp, const char *comp)
{
  strncpy (exp->complemento, comp, sizeof (exp->complemento) - 1);
  exp->complemento[sizeof (exp->complemento) - 1] = '\0';
}

void
set_context2 (struct expr *exp, const char *name2)
{
  strncpy (exp->contexto2, name2, sizeof (exp->contexto2) - 1);
  exp->contexto2[sizeof (exp->contexto2) - 1] = '\0';
}

void
print_expression_details (const struct expr *exp)
{
  printf ("%s", exp->contexto1);
}

struct expr *
create_exp (const char *name1, const char *comp, const char *name2)
{
  struct expr *new = allocate_expression ();
  set_context1 (new, name1);
  set_complement (new, comp);
  set_context2 (new, name2);
  print_expression_details (new);
  return new;
}

struct list *
create_list (const char *name)
{
  struct list *new_list = (struct list *) malloc (sizeof (struct list));
  if (!new_list)
    {
      yyerror ("[!] Memory allocation failure");
      exit (EXIT_FAILURE);
    }

  strncpy (new_list->name, name, sizeof (new_list->name) - 1);
  new_list->name[sizeof (new_list->name) - 1] = '\0';
  new_list->next = NULL;

  return new_list;
}

struct list *
add_list_to_list (struct list *new_list, struct list *existing_list)
{
  new_list->next = existing_list;
  return new_list;
}

struct list *
set_new_list (struct list *l, const char *name)
{
  struct list *new_list = create_list (name);

  if (!l)
    {
      return new_list;
    }

  return add_list_to_list (new_list, l);
}

void
free_beliefs (struct node *belief)
{
  while (belief != NULL)
    {
      struct node *temp = belief;
      belief = belief->next;
      free (temp);
    }
}

void
free_goals (struct node *goal)
{
  while (goal != NULL)
    {
      struct node *temp = goal;
      goal = goal->next;
      free (temp);
    }
}

void
free_plans (struct node *plan)
{
  while (plan != NULL)
    {
      struct node *temp = plan;
      plan = plan->next;
      free (temp);
    }
}

void
free_agent (struct head *a)
{
  free_beliefs (a->crencas);
  free_goals (a->objetivos);
  free_plans (a->planos);
  free (a);
}

void
generate_file_name (char *addr, const struct head *a)
{
  snprintf (addr, BUFFER * 2, "./aslgenerated/%s.asl", a->name_agent);
}

void
write_goals (FILE *fptr, const struct node *goals)
{
  for (const struct node * tmp_goal = goals; tmp_goal != NULL;
       tmp_goal = tmp_goal->next)
    {
      fprintf (fptr, "!%s.\n", tmp_goal->name);
    }
}

void
write_beliefs (FILE *fptr, const struct node *crencas)
{
  for (const struct node * tmp_crenca = crencas; tmp_crenca != NULL;
       tmp_crenca = tmp_crenca->next)
    fprintf (fptr, "%s.\n", tmp_crenca->name);
}

void
write_plans (FILE *fptr, const struct node *planos)
{
  int plan_number = 1;
  for (const struct node * tmp_plano = planos; tmp_plano != NULL;
       tmp_plano = tmp_plano->next)
    {
      fprintf (fptr, "\n@plano%d\n", plan_number);
      plan_number++;

      fprintf (fptr, "+!%s : ", tmp_plano->event_trigger);

      char *complemento = tmp_plano->ctx_expr->complemento;
      char *contexto1 = tmp_plano->ctx_expr->contexto1;
      char *contexto2 = tmp_plano->ctx_expr->contexto2;

      switch (complemento[0])
	{
	case 'E':
	  fprintf (fptr, "%s & %s\n", contexto1, contexto2);
	  break;
	case 'O':
	  fprintf (fptr, "%s | %s\n", contexto1, contexto2);
	  break;
	case 'N':
	  fprintf (fptr, "not %s\n", contexto2);
	  break;
	default:
	  fprintf (fptr, "%s\n", contexto2);
	  break;
	}

      struct list *struc = tmp_plano->list;

      if (struc != NULL)
	{
	  fprintf (fptr, "  <- ");
	  fprintf (fptr, "%s", struc->name);

	  struc = struc->next;

	  while (struc != NULL)
	    {
	      fprintf (fptr, " ;\n     %s", struc->name);
	      struc = struc->next;
	    }
	  fprintf (fptr, ".\n");
	}
      else
	fprintf (fptr, ".\n");
    }
}

void
generate_jason_file (struct head *a)
{
  char addr[BUFFER * 2] = { 0 };
  generate_file_name (addr, a);

  FILE *fptr = fopen (addr, "w");
  if (!fptr)
    {
      perror ("Error opening file");
      return;
    }

  write_beliefs (fptr, a->crencas);
  fprintf (fptr, "\n");
  write_goals (fptr, a->objetivos);
  write_plans (fptr, a->planos);

  fclose (fptr);
}

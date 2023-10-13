
#include "nag.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yyparse ();

void
yyerror (char *s, ...)
{
  va_list ap;
  va_start (ap, s);
  fprintf (stderr, "%d: error: ", yylineno);
  vfprintf (stderr, s, ap);
  fprintf (stderr, "\n");
}

struct head *
set_new_agent (const char *name)
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
  printf ("AGENTE <%s>\n", tmp_agent->name_agent);
  return tmp_agent;
}

void
set_new_goal (struct head *a, const char *name)
{
  struct node *new = (struct node *) malloc (sizeof (struct node));

  if (!new)
    {
      yyerror ("[!] Memory allocation failed.\n");
      exit (EXIT_FAILURE);
    }

  strcpy (new->name, name);
  new->next = a->objetivos;
  a->objetivos = new;
  printf ("OBJETIVO <%s>\n", new->name);
}

void
set_new_plan (struct head *a, const char *name, char *eg,
	      struct expr *exp, struct list *list)
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

  new->expressaoCont = exp;
  new->list = list;

  new->next = a->planos;
  a->planos = new;

  printf (" PLANO <%s> : GATILHO -> %s | CONTEXTO -> %s %s %s | CORPO -> ",
	  new->name, new->event_trigger, new->expressaoCont->contexto1,
	  new->expressaoCont->complemento, new->expressaoCont->contexto2);

  for (struct list * tmp_list = new->list; tmp_list != NULL;
       tmp_list = tmp_list->next)
    printf ("<%s> ", tmp_list->name);

  printf ("\n");
}

void
set_new_belief (struct head *a, const char *name)
{
  struct node *tmp_crenca = (struct node *) malloc (sizeof (struct node));
  if (!tmp_crenca)
    {
      yyerror ("[!] Memory allocation failure");
      exit (EXIT_FAILURE);
    }

  strncpy (tmp_crenca->name, name, sizeof (tmp_crenca->name) - 1);
  tmp_crenca->name[sizeof (tmp_crenca->name) - 1] = '\0';

  tmp_crenca->next = a->crencas;
  a->crencas = tmp_crenca;

  printf (" CRENCA <%s>\n", tmp_crenca->name);
}

struct expr *
create_exp (char *name1, char *comp, char *name2)
{
  struct expr *new =
    (struct expr *) malloc (sizeof (struct expr));
  if (!new)
    {
      yyerror ("[!] memory allocation failure");
      exit (EXIT_FAILURE);
    }

  printf ("%s", name1);

  strcpy (new->contexto1, name1);
  strcpy (new->complemento, comp);
  strcpy (new->contexto2, name2);

  return new;
}

struct list *
set_new_list (struct list *l, const char *name)
{
  struct list *new_val = malloc (sizeof (struct list));
  if (!new_val)
    {
      yyerror ("[!] memory allocation failure");
      exit (EXIT_FAILURE);
    }

  strcpy (new_val->name, name);
  new_val->next = NULL;

  if (l == NULL)
    return new_val;

  new_val->next = l;
  l = new_val;
  return l;
}

void
generate_jason_file (struct head *a)
{
  char addr[BUFFER];
  snprintf (addr, sizeof (addr), "./%s.asl", a->name_agent);

  FILE *fptr = fopen (addr, "w");

  if (!fptr)
    {
      yyerror ("[!] Error opening file for writing");
      return;
    }

  struct node *tmp_crenca = a->crencas;
  struct node *tmp_obj = a->objetivos;
  struct node *tmp_planos = a->planos;

  void print_list_formatted (FILE * f, struct list *list, const char *prefix,
			     const char *suffix)
  {
    if (list)
      {
	fprintf (f, "%s", prefix);
	for (struct list * tmp_list = list; tmp_list;
	     tmp_list = tmp_list->next)
	  {
	    fprintf (f, "!%s", tmp_list->name);
	    if (tmp_list->next)
	      fprintf (f, ";\n%s", prefix);
	    else
	      fprintf (f, "%s", suffix);
	  }
      }
  }

  for (struct node * tmp_crenca = a->crencas; tmp_crenca;
       tmp_crenca = tmp_crenca->next)
    fprintf (fptr, "%s.\n", tmp_crenca->name);

  fprintf (fptr, "\n");

  for (struct node * tmp_obj = a->objetivos; tmp_obj; tmp_obj = tmp_obj->next)
    fprintf (fptr, "%s.\n", tmp_obj->name);

  fprintf (fptr, "\n");

  for (struct node * tmp_planos = a->planos; tmp_planos;
       tmp_planos = tmp_planos->next)
    {
      fprintf (fptr, "+%s : ", tmp_planos->event_trigger);

      const char *complemento = tmp_planos->expressaoCont->complemento;

      if (*complemento)
	{
	  fprintf (fptr, "%s %s %s\n", tmp_planos->expressaoCont->contexto1,
		   complemento, tmp_planos->expressaoCont->contexto2);

	  fprintf (fptr, "  <- ");
	  print_list_formatted (fptr, tmp_planos->list, "! ", ".\n\n");
	}

      fclose (fptr);
    }
}

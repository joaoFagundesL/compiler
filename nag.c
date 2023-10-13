
#include "nag.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yyparse ();

void
yyerror (char *s)
{  
  fprintf (stderr, "%s: error: ", s);
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

  strncpy(new->contexto1, name1, sizeof(new->contexto1) - 1);
  new->contexto1[sizeof(new->contexto1) - 1] = '\0'; // Ensure null-termination

  strncpy(new->complemento, comp, sizeof(new->complemento) - 1);
  new->complemento[sizeof(new->complemento) - 1] = '\0'; // Ensure null-termination

  strncpy(new->contexto2, name2, sizeof(new->contexto2) - 1);
  new->contexto2[sizeof(new->contexto2) - 1] = '\0'; // Ensure null-termination


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

  strncpy(new_val->name, name, sizeof(new_val->name) - 1);
  new_val->name[sizeof(new_val->name) - 1] = '\0'; 
  new_val->next = NULL;

  if (!l)
    return new_val;

  new_val->next = l;
  l = new_val;
  return l;
}

void generate_jason_file(struct head *a) {
    char addr[BUFFER * 2] = { 0 };
    char termination[] = ".asl";

    snprintf(addr, sizeof(addr), "./%s%s", a->name_agent, termination);

    FILE *fptr = fopen(addr, "w");

    for (struct node *tmp_crenca = a->crencas; tmp_crenca != NULL; tmp_crenca = tmp_crenca->next)
        fprintf(fptr, "%s.\n", tmp_crenca->name);

    for (struct node *tmp_plano = a->planos; tmp_plano != NULL; tmp_plano = tmp_plano->next) {
        fprintf(fptr, "\n+%s : ", tmp_plano->event_trigger);

        char* complemento = tmp_plano->expressaoCont->complemento;
        char* contexto1 = tmp_plano->expressaoCont->contexto1;
        char* contexto2 = tmp_plano->expressaoCont->contexto2;

        switch (complemento[0]) {
            case 'E':
                fprintf(fptr, "%s & %s\n", contexto1, contexto2);
                break;
            case 'O':
                fprintf(fptr, "%s | %s\n", contexto1, contexto2);
                break;
            case 'N':
                fprintf(fptr, "not %s\n", contexto2);
                break;
            default:
                fprintf(fptr, "%s\n", contexto2);
                break;
        }

        struct list *struc = tmp_plano->list;

        if (struc != NULL) {
            fprintf(fptr, "  <- ");

            fprintf(fptr, "!%s;\n", struc->name);
            struc = struc->next;

            while (struc != NULL) {
                if(struc->next == NULL) 
                    fprintf(fptr, "     !%s.\n", struc->name);
                    
                else
                    fprintf(fptr, "     !%s;\n", struc->name);

                struc = struc->next;
            }

            fprintf(fptr, "\n");

        } else 
            fprintf(fptr, ".\n");
    }

    fclose(fptr);
}



#define BUFFER 50

extern int yylineno;
void yyerror(char *s, ...);

struct list {
  char name[BUFFER];
  struct list *next;
};

struct node {
  char name[BUFFER];
  char event_trigger[BUFFER];
  struct expr *expressaoCont;
  struct list *list;
  struct node *next;
};

struct head {
  char name_agent[BUFFER];
  struct node *crencas;
  struct node *objetivos;
  struct node *planos;
};

struct expr {
  char contexto1[BUFFER];
  char complemento[BUFFER];
  char contexto2[BUFFER];
};

struct head *set_new_agent(const char *name);

void set_new_belief(struct head *a, const char *name);
void set_new_goal(struct head *a, const char *name);

void set_new_plan(struct head *a, const char *name, char *eg,
               struct expr *exp, struct list *list);

struct expr *create_exp(char *name1, char *comp, char *name2);
struct list *set_new_list(struct list *cl, const char *name);

void generate_jason_file(struct head *a);

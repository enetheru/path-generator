#include <Eina.h>
#include <Ecore.h>
#include <limits.h>

typedef struct node node;
struct node {
   node *parent;
   int x, y, m;
   long f;
};

typedef struct sim sim;
struct sim {
   Eina_List *open, *closed;
   node *start, *fin, *best, *current;
   Evas_Object *height;
};

typedef struct sim_step sim_step;
struct sim_step {
   sim *mysim

node *
node_create(node *parent, int x, int y)
{
   node *n = malloc(sizeof(node));
   n->parent = parent;
   n->x = x;
   n->y = y;
   n->m = x + 1 * y;
   return n;
}

Eina_Bool simulate(void *data)
{
   int i,j,k;
   /* setup some stuff */
   Eina_List *open, *closed;
   Eina_List *l;
   void * list_data;

   node *start, *finish, *best, *current, *neighbours[4];
   node *worst = node_create(NULL, 0,0);
   worst->f = LONG_MAX;

   /* lets find a path through this craziness */
   Eina_Bool found = EINA_FALSE;
   long count = 0;
   while(!found && count < 10000)
   {
      /* get the next best node from the open list */
      EINA_LIST_FOREACH(open, l, list_data)
      {
         current = (node *)list_data;
         if(current->f < best->f)best = current;
      }

      /* are we at the finish line? */
      if(best->m == finish->m)
      {
         found = EINA_TRUE;
         continue;
      }

      /* move the node to the closed list */
      open = eina_list_remove(open, best);
      closed = eina_list_append(closed, best);

      /* clear the neighbours */
      for(i=0; i<4; i++)neighbours[i]=NULL;
      /* search for them in existing lists */
      EINA_LIST_FOREACH(open, l, list_data)
      {
         current = (node *)list_data;
         if(current->x == best->x+1)
         {
         }
      }
      EINA_LIST_FOREACH(closed, l, list_data)
      {
         current = (node *)list_data;

      }
   }
   return EINA_TRUE;
}

Eina_Bool sim_start(void *data)
{
   sim *path;
   path = malloc(sizeof(sim));
   
   
   return EINA_TRUE;
}

Eina_Bool sim_step(void *data)
{
   return EINA_TRUE;
}

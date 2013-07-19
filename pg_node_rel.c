#include "pg_data.h"
#include "pg_node_rel.h"


PG_Node_Rel *
pg_node_rel_new(PG_Node *node)
{
   int i;
   PG_Node_Rel *data = malloc(sizeof(PG_Node_Rel));
   for(i=0; i<8; i++)data->n[i] = NULL;
   data->node = node;
   data->prev = data->next = NULL;
   data->f = data->g = data->h = 0;
   data->state = 0;
   
   return data;
}

void
pg_node_rel_del(PG_Node_Rel *node)
{
   free(node);
}

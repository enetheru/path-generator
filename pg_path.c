#include "pg_data.h"
#include "pg_path.h"
#include "pg_world.h"
#include "pg_node_rel.h"

PG_Path *
pg_path_new(int x, int y)
{
   PG_Path *data = malloc(sizeof(PG_Path));
   data->nodes_r = NULL;

   data->start = data->end = data->current =
      pg_node_rel_new(pg_world_node_get(pg_data.world, x, y));

   /* add node to list */
   data->nodes_r = eina_list_append(data->nodes_r, data->start);

   return data;   
}

void
pg_path_del(PG_Path *path)
{
   PG_Node_Rel *node;
   EINA_LIST_FREE(path->nodes_r , node)
      pg_node_rel_del(node);
   free(path);
}

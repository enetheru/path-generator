#ifndef PG_WORLD_H
#define PG_WORLD_H
#include "pg_data.h"

PG_World *
pg_world_new();

int
pg_world_height_get(PG_World *world, int x, int y);

void
pg_world_layer_set(PG_World *world, Evas_Object *new, int id);

PG_Node *
pg_world_node_get(PG_World *world, int x, int y);

void
pg_world_create_nodes(PG_World *world);

#endif /* PG_WORLD_H */



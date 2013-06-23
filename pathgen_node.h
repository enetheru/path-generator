#ifndef PATHGEN_NODE_H
#define PATHGEN_NODE_H
#include "pathgen_map.h"

typedef struct pathgen_node Pathgen_Node;
struct pathgen_node {
   Pathgen_Map *parent_map;
   Pathgen_Node *parent_node;
   int x, y, m;
   long f;
};

Pathgen_Node *
pathgen_node_create(Pathgen_Map *parent_map, Pathgen_Node *parent_node, int x, int y, int f);

void
pathgen_node_paint(Pathgen_Node *node, int color);

void
pathgen_node_info(Pathgen_Node *node, char *prefix);

#endif /*PATHGEN_NODE_H*/

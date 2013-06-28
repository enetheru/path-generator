#ifndef PATHGEN_NODE_H
#define PATHGEN_NODE_H

typedef struct pathgen_node Pathgen_Node;
struct pathgen_node {
   Evas_Object *world;
   Pathgen_Node *parent;
   int x, y;
   double f;
};

Pathgen_Node *
pathgen_node_create(Evas_Object *world, int x, int y);

/* delete node, returning its parent or NULL */
void
pathgen_node_del(Pathgen_Node *node);

void
pathgen_node_info(Pathgen_Node *node);

#endif /*PATHGEN_NODE_H*/

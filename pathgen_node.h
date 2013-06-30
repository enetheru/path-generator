#ifndef PATHGEN_NODE_H
#define PATHGEN_NODE_H

typedef struct pathgen_node Pathgen_Node;
struct pathgen_node {
   Evas_Object *world;
   /* nodes of interest, n=neighbours 0=n, 1=ne, 2=e, ..., 7=nw */
   Pathgen_Node *parent, *n[8]; /* if i wanted to go 3d it would be [26] */
   /* node coordinates */
   int x, y, z;
   /* path variables */
   double g; /* path cost */ 
};

Pathgen_Node *
pathgen_node_create(Evas_Object *world, int x, int y);

void
pathgen_node_del(Pathgen_Node *node);

void
pathgen_node_info(Pathgen_Node *node);

double
pathgen_node_dist_euclid(Pathgen_Node *a, Pathgen_Node *b);

int
pathgen_node_dist_manhat(Pathgen_Node *a, Pathgen_Node *b);

#endif /*PATHGEN_NODE_H*/

#ifndef PG_NODE_REL_H
#define PG_NODE_REL_H

#include "pg_data.h"

PG_Node_Rel *
pg_node_rel_new(PG_Node *node);

void
pg_node_rel_del(PG_Node_Rel *node);

#endif /* PG_NODE_REL_H */


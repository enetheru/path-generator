#include "pg_data.h"
#include "pg_node.h"

double
pg_node_dist_manhat(int x1, int y1, int x2, int y2)
{
   int dx, dy;
   dx = abs(x1 - x2);
   dy = abs(y1 - y2);
   return dx + dy;
}

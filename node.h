
typedef struct node node;
struct node {
   node *parent;
   int x, y, m;
   long f;
};

node *
node_create(node *parent, int x, int y)
{
   node *n = malloc(sizeof(node));
   n->parent = parent;
   n->x = x;
   n->y = y;
   n->m = x + WIDTH * y;
   return n;
}


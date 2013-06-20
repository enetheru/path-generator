#include <Eina.h>
#include <Evas.h>
#include <Ecore.h>
#include <limits.h>

typedef struct pathgen_map pathgen_map;
typedef struct pathgen_path pathgen_path;
typedef struct pathgen_node pathgen_node;


struct pathgen_map {
   Evas_Object *result; // where to write the pixels when the pathmap is complete
   int w,h;
};

struct pathmap_path {
   pathgen_map *parent_map;// the pathmap that this path belongs to
   pathgen_node *start, *end, *next, *current;
   Eina_list *open, *closed;
};

struct pathgen_node {
   pathgen_path *parent_path;
   pathgen_node *previous_step;
   int x, y, m;
   long f;
};

pathgen_node *
pathgen_node_create(pathgen_path *parent_path, pathgen_node *parent_node, int x, int y);

Eina_Bool
pathgen_path_create(void *data);

Eina_Bool
pathgen_path_step_next(void *data);

#include "pathgen.h"

//#define VERBOSE

Pathgen_Map *
pathgen_map_create(Evas_Object *world)
{
   if(!world)return NULL;
   Pathgen_Map *map = malloc(sizeof(Pathgen_Map));

   pathgen_world_size_get(world, &map->w, &map->h);

   /* create image for visual */
   map->visual = evas_object_image_filled_add(evas_object_evas_get(world));
   evas_object_image_size_set(map->visual, map->w, map->h);
   evas_object_image_smooth_scale_set(map->visual, EINA_FALSE);
   evas_object_image_alpha_set(map->visual, EINA_TRUE);
   evas_object_show(map->visual);

   /* create data */
   int *data = malloc(map->w * map->h *4);
   evas_object_image_data_set(map->visual, data);
   evas_object_image_pixels_dirty_set(map->visual, EINA_TRUE);
   int i=0;
   int number;
   for(; i < map->w * map->h; i++)
   {
      data[i] = 0x00000000;
   }
   evas_object_resize(map->visual, 100, 100);

   return map;
}

Eina_Bool
pathgen_pathmap_del(Pathgen_Map *map)
{
   return EINA_TRUE;
}

Pathgen_Node *
pathgen_node_create(Pathgen_Map *parent_map, Pathgen_Node *parent_node, int x, int y, int f)
{
   Pathgen_Node *n = malloc(sizeof(Pathgen_Node));
   n->parent_map = (Pathgen_Map *)parent_map;
   n->parent_node = (Pathgen_Node *)parent_node;
   n->x = x;
   n->y = y;
   n->m = x + parent_map->w * y;
   n->f = f;
   return n;
}

Pathgen_Path *
pathgen_path_create(Pathgen_Map *map, Pathgen_Node *start, Pathgen_Node *end)
{
   Pathgen_Path *path = malloc(sizeof(Pathgen_Path));
   path->parent_map = map;

   path->start = start;
   _pathgen_node_paint(start, 0xFF00FF00);
   path->end = end;
   _pathgen_node_paint(end, 0xFF0000FF);

   path->open = NULL;
   path->open = eina_list_append(path->open, path->start);
   path->closed = NULL;

   path->step_counter = 0;
   return path;
}

Eina_Bool
pathgen_path_walk(void *data)
{
   Pathgen_Path *path = (Pathgen_Path *)data;

   //FIXME make timer configurable
   ecore_timer_add(0.0001, pathgen_path_step_next, path);
   return EINA_FALSE;
}

Eina_Bool
pathgen_path_step_next(void *data)
{
   Pathgen_Path *path = (Pathgen_Path *)data;

   Eina_List *l;
   void *list_data;
   Pathgen_Node *nesw[4], *next, *current, worst;
   worst.f = INT_MAX;
   int i,j,k;

   /* bail if its taking too long */
   /* FIXME, needs configurable value */
   if(path->step_counter > 10000) return EINA_FALSE;

   /* get next best node */
   next = &worst;
#ifdef VERBOSE
   fprintf(stderr, "\nSearching open list(%i) for next best node.\n", eina_list_count(path->open));
#endif
   EINA_LIST_FOREACH(path->open, l, list_data)
   {
      current = (Pathgen_Node *)list_data;
      if(current->f < next->f)
      {
#ifdef VERBOSE
         fprintf(stderr, "->");
#endif
         next = current;
      }
      pathgen_node_info(current, "\t");
   }
   
   /* if the next node is at the finish line, exit */
   if(next->m == path->end->m)
   {
      fprintf(stderr, "Goal reached.\n");
      path->end->parent_node = next->parent_node;
      ecore_timer_add(0.01, pathgen_path_step_trace, path);
      return EINA_FALSE;
   }

   /* move the node to the closed list */
   path->open = eina_list_remove(path->open, next);
   path->closed = eina_list_append(path->closed, next);
   _pathgen_node_paint(next, 0xFF888888);

   /* examine the neighbours */
   /* clear any data */
   for(i=0; i<4; i++) nesw[i] = NULL;

   /* search open list for neighbour */
#ifdef VERBOSE
   fprintf(stderr, "\nSearching open(%i) & closed(%i) for existing neighbours.\n",
      eina_list_count(path->open), eina_list_count(path->closed));
#endif
   EINA_LIST_FOREACH(path->open, l, list_data)
   {
      current = (Pathgen_Node *)list_data;
           if(current->x == next->x && current->y == next->y -1)nesw[0] = current;//north
      else if(current->x == next->x +1 && current->y == next->y)nesw[1] = current;//east
      else if(current->x == next->x && current->y == next->y +1)nesw[2] = current;//south
      else if(current->x == next->x -1 && current->y == next->y)nesw[3] = current;//west
   }

   /* search closed list for neighbour */
   EINA_LIST_FOREACH(path->closed, l, list_data)
   {
      current = (Pathgen_Node *)list_data;
           if(current->x == next->x && current->y == next->y -1)nesw[0] = current;//north
      else if(current->x == next->x +1 && current->y == next->y)nesw[1] = current;//east
      else if(current->x == next->x && current->y == next->y +1)nesw[2] = current;//south
      else if(current->x == next->x -1 && current->y == next->y)nesw[3] = current;//west
   }
   
   for(i=0; i<4; i++)
   {
      if(nesw[i])pathgen_node_info(nesw[i], "");
   }

   /* create the nodes if they arent found */
   if(!nesw[0] && next->y-1 > 0)//north
   {
      nesw[0] = pathgen_node_create(
         path->parent_map, next, next->x, next->y-1, next->f+1);
      path->open = eina_list_append(path->open, nesw[0]);
      _pathgen_node_paint(nesw[0], 0xFF008888);
   }
   if(!nesw[1] && next->x+1 < path->parent_map->w)//east
   {
      nesw[1] = pathgen_node_create(
         path->parent_map, next, next->x+1, next->y, next->f+1);
      path->open = eina_list_append(path->open, nesw[1]);
      _pathgen_node_paint(nesw[1], 0xFF008888);
   }
   if(!nesw[2] && next->y+1 < path->parent_map->h)//south
   {
      nesw[2] = pathgen_node_create(
         path->parent_map, next, next->x, next->y+1, next->f+1);
      path->open = eina_list_append(path->open, nesw[2]);
      _pathgen_node_paint(nesw[2], 0xFF008888);
   }
   if(!nesw[3] && next->x-1 > 0)//west
   {
      nesw[3] = pathgen_node_create(
         path->parent_map, next, next->x-1, next->y, next->f+1);
      path->open = eina_list_append(path->open, nesw[3]);
      _pathgen_node_paint(nesw[3], 0xFF008888);
   }

   evas_object_image_pixels_dirty_set(path->parent_map->visual, EINA_TRUE);
   evas_object_smart_changed(path->parent_map->world);

   /* increment step counter */
   path->step_counter++;
   return EINA_TRUE;
}

Eina_Bool
pathgen_path_step_trace(void *data)
{
   Pathgen_Path *path = (Pathgen_Path *)data;
   Pathgen_Node *current;
   current = path->end;
   while(current != path->start)
   {
      _pathgen_node_paint(current, 0xFFFF00FF);
      current = current->parent_node;
   }      
      
   return EINA_FALSE;
}

static void
_pathgen_node_paint(Pathgen_Node *node, int color)
{
   Pathgen_Map *map = node->parent_map;
   int *data = (int *)evas_object_image_data_get(map->visual, EINA_TRUE);
   data[node->m] = color;
}

void
pathgen_map_info(Pathgen_Map *map)
{
   fprintf(stderr,
      "==== Map Info ====\n"
      "Address\t%p\n"
      "visual\t%p\n"
      "world\t%p\n"
      "size\t%ix%i\n",
      map,
      map->visual,
      map->world,
      map->w, map->h);

   return;
}

void
pathgen_path_info(Pathgen_Path *path)
{
   fprintf(stderr,
      "==== Path Info ====\n"
      "Address\t%p\n"
      "Parent map\t%p\n"
      "open\t%i\n"
      "closed\t%i\n",
      path->parent_map,
      path,
      eina_list_count(path->open),
      eina_list_count(path->closed));
   return;
}

void
pathgen_node_info(Pathgen_Node *node, char *prefix)
{
#ifdef VERBOSE
   fprintf(stderr, "%sn:%p, %p, %p, (%3i,%3i), m:%6i, f:%lo\n", prefix,
      node->parent_map, node->parent_node, node,
      node->x, node->y, node->m, node->f);      
#endif
   return;
}

#include "pathgen_world.h"
#include "pathgen_path.h"
#include "misc.h"

Pathgen_Path *
pathgen_path_create(Evas_Object *world, Pathgen_Node *start, Pathgen_Node *end)
{
   Evas_Object *image;
   if(!world)return NULL;
   image = (Evas_Object *)pathgen_world_visual_get(world);

   Pathgen_Path *path = malloc(sizeof(Pathgen_Path));
   path->world = world;
   path->start = start;
   image_paint_node(image, start, 0xFF00FF00);
   path->end = end;
   image_paint_node(image, start, 0xFF0000FF);

   path->open = NULL;
   path->open = eina_list_append(path->open, path->start);
   path->closed = NULL;

   path->step_counter = 0;
   return path;
}

void
pathgen_path_del(Pathgen_Path *path)
{
   void *data;
   if(!path)return;
   /* delete lists of nodes */
   EINA_LIST_FREE(path->open, data)
      pathgen_node_del((Pathgen_Node *)data);

   EINA_LIST_FREE(path->closed, data)
      pathgen_node_del((Pathgen_Node *)data);

   /* delete start, end, current nodes */
   pathgen_node_del(path->start);
   pathgen_node_del(path->end);
   pathgen_node_del(path->current);

   free(path);
   path = NULL;
}

void
pathgen_path_search(Pathgen_Path *path)
{
   int iter = 0;
   while(iter)
   {
      pathgen_path_search_iter(path);
   }
   if(iter = -1)
      fprintf(stderr,
         "ERR: failed to find solution in time \n");
   if(iter = 1)
      fprintf(stderr, "solution found \n");
}

int
pathgen_path_search_iter(Pathgen_Path *path)
{
   return 1;
}

Eina_Bool
pathgen_path_search_slow(void *data)
{
   return EINA_FALSE;
}

Eina_Bool
pathgen_path_step_next(void *data)
{
   Pathgen_Path *path = (Pathgen_Path *)data;
   Evas_Object *image = (Evas_Object *)pathgen_world_visual_get(path->world);

   Eina_List *l;
   void *list_data;
   Pathgen_Node *nesw[4], *next, *current;
   int i, j, k, x, y, w, h;

   pathgen_world_size_get(path->world, &w, &h);
   path->step_counter++;

   next = pathgen_path_best(path);
   if(!next)
   {
      fprintf(stderr,
         "ERR: no next node\n");
      evas_object_smart_callback_call(path->world,
         EVT_SIM_TRAVELER_NEW, NULL);
      return EINA_FALSE;
   }

   /* bail if its taking too long */
   if(path->step_counter > path->step_count)
   {
      fprintf(stderr, "maximum steps reached stopping\n");
      path->end->parent = next->parent;
      path->current = path->end;
      ecore_timer_add(path->step_speed*5, pathgen_path_step_trace, path);
      evas_object_smart_callback_call(path->world,
         EVT_SIM_TRAVELER_NEW, NULL);
      return EINA_FALSE;
   }

   /* if the next node is at the finish line, exit */
   if(next->x == path->end->x && next->y == path->end->y)
   {
      fprintf(stderr, "Goal reached.\n");
      path->end->parent = next->parent;
      path->current = path->end;
      ecore_timer_add(path->step_speed*5, pathgen_path_step_trace, path);
      evas_object_smart_callback_call(path->world,
         EVT_SIM_TRAVELER_NEW, NULL);
      return EINA_FALSE;
   }

   /* move the node to the closed list */
   path->open = eina_list_remove(path->open, next);
   path->closed = eina_list_append(path->closed, next);
   image_paint_node(image, next, 0x88000000);

   /* examine the neighbours */
   /* clear any data */
   for(i=0; i<4; i++) nesw[i] = NULL;

   /* search open list for neighbour */
   EINA_LIST_FOREACH(path->open, l, list_data)
   {
      current = (Pathgen_Node *)list_data;
           if(current->x==next->x    && current->y==next->y -1)nesw[0]=current;//north
      else if(current->x==next->x +1 && current->y==next->y   )nesw[1]=current;//east
      else if(current->x==next->x    && current->y==next->y +1)nesw[2]=current;//south
      else if(current->x==next->x -1 && current->y==next->y   )nesw[3]=current;//west
   }

   /* search closed list for neighbour */
   EINA_LIST_FOREACH(path->closed, l, list_data)
   {
      current = (Pathgen_Node *)list_data;
           if(current->x==next->x    && current->y==next->y -1)nesw[0]=current;//north
      else if(current->x==next->x +1 && current->y==next->y   )nesw[1]=current;//east
      else if(current->x==next->x    && current->y==next->y +1)nesw[2]=current;//south
      else if(current->x==next->x -1 && current->y==next->y   )nesw[3]=current;//west
   }
   
   for(i=0; i<4; i++)
   {
      /* if the node already exists, skip */
      if(nesw[i])
      {
         continue;
      }
           if(next->x+1 > w-1) continue;
      else if(next->x-1 < 0) continue;
      else if(next->y+1 > h-1) continue;
      else if(next->y-1 < 0) continue;
      /* setup new coordinates changes */
              if(i==0){ x = next->x  ; y = next->y-1;}//north
         else if(i==1){ x = next->x+1; y = next->y  ;}//east
         else if(i==2){ x = next->x  ; y = next->y+1;}//south
         else if(i==3){ x = next->x-1; y = next->y  ;}//west
      /* create the new node*/
      nesw[i] = pathgen_node_create(path->world, x, y);
      if(!nesw[i])
      {
         fprintf(stderr,
            "ERR: node not created as expected\n");
         continue;
      }
      nesw[i]->parent = next;

      /* change the F value based on influences */
      k = pathgen_world_height_get_xy(path->world, x, y);
      k = pow(k, 2) / pow(255.0, 2) * 100;
      float mag = pythag_node(nesw[i], path->end);
      mag = mag / sqrt(pow(w, 2)+pow(h, 2)) * 100;
      nesw[i]->f = (mag*3 + k*97 / 100);

      /* add the node to the open list */
      path->open = eina_list_append(path->open, nesw[i]);

      /* paint the node */
      image_paint_node(image, nesw[i], 0x88008888);
   }
   evas_object_smart_changed(path->world);
   return EINA_TRUE;
}

Eina_Bool
pathgen_path_step_trace(void *data)
{
   Pathgen_Path *path = (Pathgen_Path *)data;
   Evas_Object *visual = (Evas_Object *)pathgen_world_visual_get(path->world);
   Evas_Object *heat = (Evas_Object *)pathgen_world_heat_get(path->world);
   if(path->current != path->start)
   {
      image_paint_node(visual, path->current, 0xFFFF00FF);
      path->current = path->current->parent;
   }
   else
   {
      image_paint_path(heat, path, 0xFF000001);
      image_fill_color(visual, 0x00000000);
      return EINA_FALSE;
   }

   evas_object_smart_changed(path->world);
   return EINA_TRUE;
}

void
pathgen_path_info(Pathgen_Path *path)
{
   fprintf(stderr,
      "p: w=%p, s=%p, e=%p, c=%p, o=%i, c=%i, s=%i, s=%f\n", 
      path->world, path->start, path->end, path->current, 
      eina_list_count(path->open),
      eina_list_count(path->closed),
      path->step_count, path->step_speed);
   return;
}

Pathgen_Node *
pathgen_path_best(Pathgen_Path *path)
{
   //fprintf(stderr, "Searching for the best node.\n");
   Eina_List *l;
   void *list_data;
   Pathgen_Node *best, *current;
   float maga, magb;

   if(!path)
   {
      fprintf(stderr,
         "ERR: no path given\n");  
      return NULL;
   }
   if(eina_list_count(path->open) == 0)
   {
      fprintf(stderr,
         "ERR: no nodes in open list\n");  
      return NULL;
   }

   /* get next best node */
   best = eina_list_data_get(path->open);
   EINA_LIST_FOREACH(path->open, l, list_data)
   {
      current = (Pathgen_Node *)list_data;
      if(current->f < best->f)best = current;
   }
//   pathgen_node_info(best);

   return best;
}

void
pathgen_path_neighbours(Pathgen_Path *path)
{
   Eina_List *l;
   void *list_data;
   Pathgen_Node *node;
   Eina_Bool nesw[4];
   int i,x,y,k;
   /* examine the neighbours */
   /* clear any data */
   for(i=0; i<4; i++) nesw[i] = EINA_FALSE;

   /* search open list for neighbour */
   EINA_LIST_FOREACH(path->open, l, list_data)
   {
      node = (Pathgen_Node *)list_data;
           if( node->x == path->current->x    &&
               node->y == path->current->y -1 )  nesw[0]=EINA_TRUE;//north
      else if( node->x == path->current->x +1 &&
               node->y == path->current->y    )  nesw[1]=EINA_TRUE;//east
      else if( node->x == path->current->x    &&
               node->y == path->current->y +1 )  nesw[2]=EINA_TRUE;//south
      else if( node->x == path->current->x -1 && 
               node->y == path->current->y    )  nesw[3]=EINA_TRUE;//west
   }

   /* search closed list for neighbour */
   EINA_LIST_FOREACH(path->closed, l, list_data)
   {
      node = (Pathgen_Node *)list_data;
           if( node->x == path->current->x    &&
               node->y == path->current->y -1 )  nesw[0]=EINA_TRUE;//north
      else if( node->x == path->current->x +1 &&
               node->y == path->current->y    )  nesw[1]=EINA_TRUE;//east
      else if( node->x == path->current->x    &&
               node->y == path->current->y +1 )  nesw[2]=EINA_TRUE;//south
      else if( node->x == path->current->x -1 && 
               node->y == path->current->y    )  nesw[3]=EINA_TRUE;//west
   }
   
   for(i=0; i<4; i++)
   {
      /* if the node already exists, skip */
      if(nesw[i])
      {
         continue;
      }
      /* setup new coordinates changes */
              if(i==0){ x = path->current->x  ; y = path->current->y-1;}//north
         else if(i==1){ x = path->current->x+1; y = path->current->y  ;}//east
         else if(i==2){ x = path->current->x  ; y = path->current->y+1;}//south
         else if(i==3){ x = path->current->x-1; y = path->current->y  ;}//west
      /* create the new node*/
      node = pathgen_node_create(path->world, x, y);
      if(!node)continue;
      node->parent = path->current;

      /* change the F value based on the height map */
      k = pathgen_world_height_get_xy(path->world, x, y);
      node->f = path->current->f + k*k;

      /* add the node to the open list */
      path->open = eina_list_append(path->open, node);

      /* paint the node */
//      image_paint_node(image, nesw[i], 0x88008888);
   }

   return;
}

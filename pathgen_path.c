#include "pathgen_path.h"

Pathgen_Path *
pathgen_path_create(Pathgen_Map *map, Pathgen_Node *start, Pathgen_Node *end)
{
   Pathgen_Path *path = malloc(sizeof(Pathgen_Path));
   path->parent_map = map;
   path->start = start;
   pathgen_node_paint(start, 0xFF00FF00);
   path->end = end;
   pathgen_node_paint(end, 0xFF0000FF);

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

   ecore_timer_add(path->step_speed, pathgen_path_step_next, path);
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
   int i,j,k,x,y;

   /* bail if its taking too long */
   if(path->step_counter > path->step_count)
   {
      fprintf(stderr, "maximum steps reached stopping\n");
      return EINA_FALSE;
   }
   path->step_counter++;


   /* get next best node */
   next = &worst;
   EINA_LIST_FOREACH(path->open, l, list_data)
   {
      current = (Pathgen_Node *)list_data;
      if(current->f < next->f)next = current;
      pathgen_node_info(current, "\t");
   }
   
   /* if the next node is at the finish line, exit */
   if(next->m == path->end->m)
   {
      fprintf(stderr, "Goal reached.\n");
      path->end->parent_node = next->parent_node;
      path->current = path->end;
      ecore_timer_add(path->step_speed*10, pathgen_path_step_trace, path);
      return EINA_FALSE;
   }

   /* move the node to the closed list */
   path->open = eina_list_remove(path->open, next);
   path->closed = eina_list_append(path->closed, next);
   pathgen_node_paint(next, 0x88000000);

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
         pathgen_node_info(nesw[i], "");
         continue;
      }
      /* if it is out of bounds, skip */
           if(next->x+1 > path->parent_map->w-1) continue;
      else if(next->x-1 < 0) continue;
      else if(next->y+1 > path->parent_map->h-1) continue;
      else if(next->y-1 < 0) continue;
      /* setup new coordinates changes */
              if(i==0){ x = next->x  ; y = next->y-1;}//north
         else if(i==1){ x = next->x+1; y = next->y  ;}//east
         else if(i==2){ x = next->x  ; y = next->y+1;}//south
         else if(i==3){ x = next->x-1; y = next->y  ;}//west
      /* create the new node*/
      nesw[i] = pathgen_node_create(path->parent_map, next, x, y, next->f+1);

      /* change the F value based on the height map */
      int *data = pathgen_world_height_get(path->parent_map->parent_world);
      int value = data[nesw[i]->m] & 0x000000FF;
      nesw[i]->f = next->f + (value *value * value)/100;

      /* add the node to the open list */
      path->open = eina_list_append(path->open, nesw[i]);

      /* paint the node */
      pathgen_node_paint(nesw[i], 0x88008888);
   }
   
   evas_object_image_pixels_dirty_set(path->parent_map->visual, EINA_TRUE);
   evas_object_smart_changed(path->parent_map->parent_world);

   return EINA_TRUE;
}

Eina_Bool
pathgen_path_step_trace(void *data)
{
   Pathgen_Path *path = (Pathgen_Path *)data;
   if(path->current != path->start)
   {
      pathgen_node_paint(path->current, 0xFFFF00FF);
      path->current = path->current->parent_node;
   }
   else return EINA_FALSE;

   evas_object_image_pixels_dirty_set(path->parent_map->visual, EINA_TRUE);
   evas_object_smart_changed(path->parent_map->parent_world);
      
   return EINA_TRUE;
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



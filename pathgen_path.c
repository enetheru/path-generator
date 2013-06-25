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
   Evas_Object *image = (Evas_Object *)pathgen_world_visual_get(path->world);

   Eina_List *l;
   void *list_data;
   Pathgen_Node *nesw[4], *next, *current, worst;
   int i, j, k, x, y, w, h;

   pathgen_world_size_get(path->world, &w, &h);

   /* bail if its taking too long */
   if(path->step_counter > path->step_count)
   {
      fprintf(stderr, "maximum steps reached stopping\n");
      return EINA_FALSE;
   }
   path->step_counter++;

   if(eina_list_count(path->open) == 0)return EINA_FALSE;

   /* get next best node */
   next = eina_list_data_get(path->open);
   EINA_LIST_FOREACH(path->open, l, list_data)
   {
      current = (Pathgen_Node *)list_data;
      if(current->f < next->f)next = current;
   }
   pathgen_node_info(next);
   
   /* if the next node is at the finish line, exit */
   if(next->x == path->end->x && next->y == path->end->y)
   {
      fprintf(stderr, "Goal reached.\n");
      path->end->parent = next->parent;
      path->current = path->end;
      ecore_timer_add(path->step_speed*5, pathgen_path_step_trace, path);
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
         pathgen_node_info(nesw[i]);
         continue;
      }
      /* if it is out of bounds, skip */
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
      nesw[i]->parent = next;

      /* change the F value based on the height map */
      k = pathgen_world_height_get_xy(path->world, x, y);
      nesw[i]->f = next->f + (k * k * k)/100;

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
   Evas_Object *image = (Evas_Object *)pathgen_world_visual_get(path->world);
   if(path->current != path->start)
   {
      image_paint_node(image, path->current, 0xFFFF00FF);
      path->current = path->current->parent;
   }
   else
   {
      image_paint_path(image, path, 0xFFFF0000);
      return EINA_FALSE;
   }

   evas_object_smart_changed(path->world);
   return EINA_TRUE;
}

void
pathgen_path_info(Pathgen_Path *path)
{
   fprintf(stderr,"p: w=%p, s=%p, e=%p, c=%p, o=%i, c=%i, s=%i, s=%f\n", 
      path->world, path->start, path->end, path->current, 
      eina_list_count(path->open),
      eina_list_count(path->closed),
      path->step_count, path->step_speed);
   return;
}



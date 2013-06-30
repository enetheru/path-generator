#include "pathgen_world.h"
#include "pathgen_path.h"
#include "image.h"

Pathgen_Path *
pathgen_path_create(Evas_Object *world, Pathgen_Node *start, Pathgen_Node *end)
{
   if(!world)return NULL;

   Pathgen_Path *path = malloc(sizeof(Pathgen_Path));
   path->world = world;
   path->start = start;
   path->end = end;

   path->open = NULL;
   path->open = eina_list_append(path->open, path->start);
   path->closed = NULL;

   path->iter = 1;
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
pathgen_path_info(Pathgen_Path *path)
{
   fprintf(stderr,
      "p: w=%p, s=%p, e=%p, c=%p, o=%i, c=%i, s=%i, s=%f\n", 
      path->world, path->start, path->end, path->current, 
      eina_list_count(path->open),
      eina_list_count(path->closed),
      path->iter_max, path->iter_speed);
   return;
}

Eina_Bool
pathgen_path_search_fast(void *data)
{
   Pathgen_Path *path = data;

   Eina_List *l;
   void *list_data;
   Pathgen_Node *peers[4], *best, *node;
   int i,j,k,x,y;

   PATHGEN_WORLD_DATA_GET(path->world, priv);

   /* get next best node */
   best = pathgen_path_best(path);

   /* bail if no next node */
   if(!best)
      return EINA_FALSE;

   /* setup the return path */
   path->end->parent = best->parent;
   path->current = path->end;

   /* bail if its taking too long */
   if(path->iter >= path->iter_max)
      return EINA_FALSE;

   /* if the next node is at the finish line, exit */
   if(best->x == path->end->x && best->y == path->end->y)
      return EINA_FALSE;

   /* move the node to the closed list */
   path->open = eina_list_remove(path->open, best);
   path->closed = eina_list_append(path->closed, best);
   /* examine the neighbours */
   /* clear any data */
   for(i=0; i<4; i++) peers[i] = NULL;

   /* search open list for neighbour */
   EINA_LIST_FOREACH(path->open, l, list_data)
   {
      node = (Pathgen_Node *)list_data;
           if(node->x==best->x    && node->y==best->y -1)peers[0]=node;//north
      else if(node->x==best->x +1 && node->y==best->y   )peers[1]=node;//east
      else if(node->x==best->x    && node->y==best->y +1)peers[2]=node;//south
      else if(node->x==best->x -1 && node->y==best->y   )peers[3]=node;//west
   }

   /* search closed list for neighbour */
   EINA_LIST_FOREACH(path->closed, l, list_data)
   {
      node = (Pathgen_Node *)list_data;
           if(node->x==best->x    && node->y==best->y -1)peers[0]=node;//north
      else if(node->x==best->x +1 && node->y==best->y   )peers[1]=node;//east
      else if(node->x==best->x    && node->y==best->y +1)peers[2]=node;//south
      else if(node->x==best->x -1 && node->y==best->y   )peers[3]=node;//west
   }
   
   for(i=0; i<4; i++)
   {
      /* if the node already exists, skip */
      if(peers[i])
      {
         continue;
      }
           if(best->x+1 > priv->w-1) continue;
      else if(best->x-1 < 0) continue;
      else if(best->y+1 > priv->h-1) continue;
      else if(best->y-1 < 0) continue;
      /* setup new coordinates changes */
              if(i==0){ x = best->x  ; y = best->y-1;}//north
         else if(i==1){ x = best->x+1; y = best->y  ;}//east
         else if(i==2){ x = best->x  ; y = best->y+1;}//south
         else if(i==3){ x = best->x-1; y = best->y  ;}//west
      /* create the new node*/
      peers[i] = pathgen_node_create(path->world, x, y);
      peers[i]->parent = best;

      /* == build hueristic data == */
      /* manhattan distance from origin */
      int inf_dist_m = (float)pathgen_node_dist_manhat(peers[i], path->end);
      
      /* euclidean distance to target */
      double inf_dist_e = pathgen_node_dist_euclid(peers[i], path->end);

      /* change of height */
      int inf_desasc = abs(peers[i]->z - best->z);

      /* adherance to roads */
      int inf_path = 255 - (float)image_pixel_value_get(priv->heatmap, x, y, 0xFF000000, 24);

      double f =
          inf_dist_m * priv->i_path_inf_dist_manhat
        + inf_dist_e * priv->i_path_inf_dist_euclid
        + inf_desasc * priv->i_path_inf_desasc
        + inf_path   * priv->i_path_inf_path;
            
      peers[i]->g = f;


      /* add the node to the open list */
      path->open = eina_list_append(path->open, peers[i]);
   }

   path->iter++;
   return EINA_TRUE;
}

Eina_Bool
pathgen_path_search_slow(void *data)
{
   fprintf(stderr, "slow search\n");

   Pathgen_Path *path;
   Pathgen_Node *nesw[4], *next, *node;

   Eina_List *l;
   void *list_data;
   int i, j, x, y;
   double f;

   path = data;
   PATHGEN_WORLD_DATA_GET(path->world, priv);

   fprintf(stderr, "current_iteration %i of %i\n", path->iter, path->iter_max);

   fprintf(stderr, "searching for best node\n");
   next = pathgen_path_best(path);
   if(!next)
   {
      fprintf(stderr,
         "ERR: no next node\n");
      evas_object_smart_callback_call(path->world,
         EVT_PATH_SEARCH_COMPLETE, path);
      return EINA_FALSE;
   }
   path->end->parent = next->parent;
   path->current = next;

   /* bail if its taking too long */
   if(path->iter >= path->iter_max)
   {
      fprintf(stderr, "maximum steps reached stopping\n");
      evas_object_smart_callback_call(path->world,
         EVT_PATH_SEARCH_COMPLETE, path);
      return EINA_FALSE;
   }

   /* if the next node is at the finish line, exit */
   if(next->x == path->end->x && next->y == path->end->y)
   {
      fprintf(stderr, "Goal reached.\n");
      evas_object_smart_callback_call(path->world,
         EVT_PATH_SEARCH_COMPLETE, path);
      return EINA_FALSE;
   }

   /* move the node to the closed list */
   path->open = eina_list_remove(path->open, next);
   path->closed = eina_list_append(path->closed, next);

   image_paint_node(priv->search, next, 0x88000000);

   /* examine the neighbours */
   /* clear any data */
   for(i=0; i<4; i++) nesw[i] = NULL;

   fprintf(stderr, "searching for neighbours\n");
   /* search open list for neighbour */
   EINA_LIST_FOREACH(path->open, l, list_data)
   {
      node = (Pathgen_Node *)list_data;
           if(node->x==next->x    && node->y==next->y -1)nesw[0]=node;//north
      else if(node->x==next->x +1 && node->y==next->y   )nesw[1]=node;//east
      else if(node->x==next->x    && node->y==next->y +1)nesw[2]=node;//south
      else if(node->x==next->x -1 && node->y==next->y   )nesw[3]=node;//west
   }

   /* search closed list for neighbour */
   EINA_LIST_FOREACH(path->closed, l, list_data)
   {
      node = (Pathgen_Node *)list_data;
           if(node->x==next->x    && node->y==next->y -1)nesw[0]=node;//north
      else if(node->x==next->x +1 && node->y==next->y   )nesw[1]=node;//east
      else if(node->x==next->x    && node->y==next->y +1)nesw[2]=node;//south
      else if(node->x==next->x -1 && node->y==next->y   )nesw[3]=node;//west
   }
   
   fprintf(stderr, "generating neighbours\n");
   for(i=0; i<4; i++)
   {
      /* if the node already exists, skip */
      if(nesw[i])
      {
         continue;
      }
           if(next->x+1 > priv->w-1) continue;
      else if(next->x-1 < 0) continue;
      else if(next->y+1 > priv->h-1) continue;
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

      /* == build hueristic data == */
      /* manhattan distance from origin */
      int inf_dist_m = (float)pathgen_node_dist_manhat(nesw[i], path->start);
      fprintf(stderr, "manhattan distance to end = %i\n", inf_dist_m);
      
      /* euclidean distance to target */
      double inf_dist_e = pathgen_node_dist_euclid(nesw[i], path->start);
      fprintf(stderr, "euclidean distance to end = %f\n", inf_dist_e);

      /* change of height */
      int inf_desasc = abs(nesw[i]->z - next->z);
      fprintf(stderr, "ascent/descent difficulty = %i\n", inf_desasc);

      /* adherance to roads */
      int inf_path = 255 - (float)image_pixel_value_get(priv->heatmap, x, y, 0xFF000000, 24);
      fprintf(stderr, "path difficulty           = %i\n", inf_path);

      f = inf_dist_m * priv->i_path_inf_dist_manhat
        + inf_dist_e * priv->i_path_inf_dist_euclid
        + inf_desasc * priv->i_path_inf_desasc
        + inf_path   * priv->i_path_inf_path;
            
      fprintf(stderr, "f value = %f\n", f);
      nesw[i]->g = f;

      /* add the node to the open list */
      path->open = eina_list_append(path->open, nesw[i]);

      /* paint the node */
      image_paint_node(priv->search, nesw[i], 0x88008888);
   }
   fprintf(stderr, "incrementing iterater\n");
   evas_object_smart_changed(path->world);
   path->iter++;
   return EINA_TRUE;
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
      if(current)
         if(current->g < best->g)best = current;
         else if(current->g == best->g && rand() % 10 < 5)best = current;
   }
//   pathgen_node_info(best);

   return best;
}

Eina_Bool
pathgen_path_walk(void *data)
{
   if(!data)return EINA_FALSE;
   Pathgen_Path *path = data;

   if(!path->current)return EINA_FALSE;

   if(!path->current->parent)
   {
      path->current = path->end;
      return EINA_FALSE;
   }
   path->current = path->current->parent;

   return EINA_TRUE;
}

Eina_Bool
pathgen_path_walk_slow(void *data)
{
   Eina_Bool ret = EINA_TRUE;
   Pathgen_Path *path = data;
   PATHGEN_WORLD_DATA_GET(path->world, priv);
   if(!data)ret = EINA_FALSE;
   else
   {
      image_paint_node(priv->path, path->current, 0xFFFFFF00);
      if(!pathgen_path_walk(path))
      {
         ret = EINA_FALSE;
      }
   }
   if(!ret)
   {
      image_paint_path(priv->heatmap, path, 0x01000000);
      evas_object_smart_callback_call(path->world, EVT_SIM_TRAVELER_NEW, NULL);
   }
   return ret;
}

/***********************
* Path Smart Callbacks *
************************/

void
pathgen_path_search_complete( void *data, __UNUSED__
    Evas_Object *o, void *event_info )
{
   Pathgen_Path *path = event_info;
   PATHGEN_WORLD_DATA_GET(o, priv);


   if(priv->i_display_path)
   {
      image_fill_color(priv->path, 0x00000000);
      ecore_timer_add(path->iter_speed, pathgen_path_walk_slow, path);
   }
   else
   {
      image_paint_path(priv->heatmap, path, 0x01000000);
      evas_object_smart_callback_call(o, EVT_SIM_TRAVELER_NEW, NULL);
   }
   evas_object_smart_changed(o);
}

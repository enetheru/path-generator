#include "pathgen_world.h"
#include "pathgen_path.h"
#include "r_image.h"
#include "r_pixel.h"

Pathgen_Path *
pathgen_path_create(Evas_Object *world, Pathgen_Node *start, int gx, int gy)
{
   if(!world)return NULL;

   Pathgen_Path *path = malloc(sizeof(Pathgen_Path));
   path->world = world;
   path->start = start;
   path->gx = gx;
   path->gy = gy;

   path->open = NULL;
   path->open = eina_list_append(path->open, path->start);
   path->start->open = EINA_TRUE;
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
   free(path);
}

Eina_Bool
pathgen_path_search(void *data)
{
   Pathgen_Path *path;
   Pathgen_Node *peers[8], *best, *node;

   Eina_List *l, *l_next;
   void *list_data;
   int i, j;

   int x, y, elev, avoid;
   double f, g, h;

   path = data;
   PATHGEN_WORLD_DATA_GET(path->world, priv);

   /* get the next best node */
   best = pathgen_path_best(path);

   /* bail if there is no next best */
   if(!best)
   {
      evas_object_smart_callback_call(path->world,
         EVT_PATH_SEARCH_COMPLETE, path);
      return EINA_FALSE;
   }

   /* bail if its taking too long */
   if(path->iter >= priv->i_sim_search_iter_max)
   {
      evas_object_smart_callback_call(path->world,
         EVT_PATH_SEARCH_COMPLETE, path);
      return EINA_FALSE;
   }

   /* Bail if we have reached our goal */
   if(best->x == path->gx && best->y == path->gy)
   {
      evas_object_smart_callback_call(path->world,
         EVT_PATH_SEARCH_COMPLETE, path);
      return EINA_FALSE;
   }

   /* set our goal node*/
   path->end = best;
   path->current = best;

   /* move the node to the closed list */
   path->open = eina_list_remove(path->open, best);
   best->open == EINA_FALSE;
   path->closed = eina_list_append(path->closed, best);
   best->closed == EINA_TRUE;

   if(priv->i_display_[6])
      image_func_pixel(priv->l[6], best->x, best->y, NULL, 0x88000000);

   /* examine the neighbours */
   if(priv->i_path_search_diagonal)j = 8;
   else j = 4;
   for(i=0; i<j; i++)
   {
      /* setup new coordinates changes */
           if(i==0){ x = best->x  ; y = best->y-1;}//north
      else if(i==1){ x = best->x+1; y = best->y  ;}//east
      else if(i==2){ x = best->x  ; y = best->y+1;}//south
      else if(i==3){ x = best->x-1; y = best->y  ;}//west
      else if(i==4){ x = best->x+1; y = best->y-1;}//north east
      else if(i==5){ x = best->x-1; y = best->y-1;}//north west
      else if(i==6){ x = best->x+1; y = best->y+1;}//south east
      else if(i==7){ x = best->x-1; y = best->y+1;}//south west

      /* skip the neighbour if it is out of bounds*/
           if(x > priv->w-1) continue;
      else if(x < 0) continue;
      else if(y > priv->h-1) continue;
      else if(y < 0) continue;

      /* skip the neighbour if the elevation is out of limits*/
      elev = best->z - image_pixel_value_get(priv->l[0], x, y, 0x000000FF, 0);
      if(priv->i_path_climb_up_limit < elev)continue;
      if(priv->i_path_climb_down_limit < abs(elev))continue;

      /* skip the neighbour if the avoid is out of limits */
      if(priv->l[2])
      {
         avoid = image_pixel_value_get(priv->l[2], x, y, 0xFF000000, 24);
         if(priv->i_path_avoid_limit < avoid)continue;
      }

      /* determine the movement cost */
      if(priv->i_path_search_diagonal && i > 3)
      {
         g = best->g + 1.414; 
      }
      else g= best->g+1;
      g += abs(elev) * priv->i_world_height_mult;

      /* Calculate the Hueristic */ 
      // Distance to target
      h =  priv->distance_to_goal(x, y, path->gx, path->gy)
         * priv->i_path_distance_goal_mult;
      // Path Heat
      h += fmin(255 - image_pixel_value_get(priv->l[5], x, y, 0xFF000000, 24)
            + priv->i_path_heat_tolerance, 255)
         * priv->i_path_heat_value;
      // Path Map
      h += fmin(255 - image_pixel_value_get(priv->l[3], x, y, 0xFF000000, 24)
            + priv->i_path_map_tolerance, 255 )
         * priv->i_path_map_value;
      // Path Avoidance
      h += fmax( image_pixel_value_get(priv->l[2], x, y, 0xFF000000, 24)
            - priv->i_path_avoid_tolerance, 0 )
         * priv->i_path_avoid_value;
      // Climb Up/down
      if( elev > priv->i_path_climb_up_tolerance)
         h += ( elev
            - priv->i_path_climb_up_tolerance)
            * priv->i_path_climb_up_value;
      else if( abs(elev) > priv->i_path_climb_down_tolerance )
         h += ( abs(elev)
            - priv->i_path_climb_down_tolerance )
            * priv->i_path_climb_down_value;


      f = g + h;

      EINA_LIST_FOREACH_SAFE(path->open, l, l_next, list_data)
      {
         node = (Pathgen_Node *)list_data;
         if(node->x == x && node->y == y)
         {
            best->n[i]=node;
            if(node->g > g)
            {
               path->open = eina_list_remove_list(path->open, l);
               node->open = EINA_FALSE;
            }
         }
      }

      EINA_LIST_FOREACH_SAFE(path->closed, l, l_next, list_data)
      {
         node = (Pathgen_Node *)list_data;
         if(node->x == x && node->y == y)
         {
            best->n[i]=node;
            if(node->g > g)
            {
               path->closed = eina_list_remove_list(path->closed, l);
               node->closed = EINA_FALSE;
            }
         }
      }

      /* if there isnt a neighbour, create one */
      if(!best->n[i])
      {
         best->n[i] = pathgen_node_create(path->world, x, y);
      }

      if(!(best->n[i]->open) && !(best->n[i]->closed))
      {
         best->n[i]->g = g;
         best->n[i]->h = h;
         best->n[i]->f = f;
         path->open = eina_list_append(path->open, best->n[i]);
         best->n[i]->open = EINA_TRUE;
         best->n[i]->parent = best;
         if(priv->i_display_[6])
            image_func_pixel(priv->l[6],
               best->n[i]->x, best->n[i]->y, NULL, 0x88008888);
      }

   }

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

   if(!path)return NULL;

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
         if(current->f < best->f)best = current;
         else if(current->f == best->f && rand() % 10 < 5)best = current;
   }
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
      image_func_pixel(priv->l[7], path->current->x, path->current->y, NULL, 0xFFFFFF00);
      if(!pathgen_path_walk(path))
      {
         ret = EINA_FALSE;
      }
   }
   if(!ret)
   {
      while(pathgen_path_walk(path))
         image_func_image(priv->l[5], path->current->x, path->current->y,
            pixel_add, priv->i_path_walk_brush, priv->i_path_tread_weight);
  
      evas_object_smart_callback_call(path->world, EVT_SIM_TRAVELER_NEW, NULL);
      pathgen_path_del(path);
   }
   return ret;
}

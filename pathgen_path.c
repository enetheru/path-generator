#include "pathgen_world.h"
#include "pathgen_path.h"
#include "r_image.h"
#include "r_pixel.h"

Pathgen_Path *
pathgen_path_create(Evas_Object *world, Pathgen_Node *start, Pathgen_Node *goal)
{
   if(!world)return NULL;

   Pathgen_Path *path = malloc(sizeof(Pathgen_Path));
   path->world = world;
   path->start = start;
   path->goal = goal;

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
   pathgen_node_del(path->goal);
   pathgen_node_del(path->current);

   free(path);
   path = NULL;
}


void
pathgen_path_info(Pathgen_Path *path)
{
   fprintf(stderr,
      "p: w=%p, s=%p, e=%p, c=%p, o=%i, c=%i\n", 
      path->world, path->start, path->goal, path->current, 
      eina_list_count(path->open),
      eina_list_count(path->closed));
   return;
}

Eina_Bool
pathgen_path_search(void *data)
{
   Pathgen_Path *path;
   Pathgen_Node *peers[8], *best, *node;

   Eina_List *l;
   void *list_data;
   int i, j, x, y;
   double f;

   path = data;
   PATHGEN_WORLD_DATA_GET(path->world, priv);

   best = pathgen_path_best(path);
   if(!best)
   {
      fprintf(stderr, "ERR:path_search, no best node\n");
      evas_object_smart_callback_call(path->world,
         EVT_PATH_SEARCH_COMPLETE, path);
      return EINA_FALSE;
   }
   path->goal->parent = best->parent;
   path->end = best;
   path->current = best;

   /* bail if its taking too long */
   if(path->iter >= priv->i_path_search_iter_max)
   {
//      fprintf(stderr, "INF:path_search, maximum steps reached stopping\n");
      evas_object_smart_callback_call(path->world,
         EVT_PATH_SEARCH_COMPLETE, path);
      return EINA_FALSE;
   }

   /* if the best node is at the finish line, exit */
   if(best->x == path->goal->x && best->y == path->goal->y)
   {
//      fprintf(stderr, "INF:path_search, Goal reached.\n");
      evas_object_smart_callback_call(path->world,
         EVT_PATH_SEARCH_COMPLETE, path);
      return EINA_FALSE;
   }

   /* move the node to the closed list */
   path->open = eina_list_remove(path->open, best);
   path->closed = eina_list_append(path->closed, best);

   if(priv->i_display_search)
      image_func_pixel(priv->search, best->x, best->y, NULL, 0x88000000);

   /* examine the neighbours */
   /* clear any data */
   if(priv->i_path_search_diagonal)j = 8;
   else j = 4;
   for(i=0; i<j; i++) peers[i] = NULL;

   /* search open list for neighbour */
   EINA_LIST_FOREACH(path->open, l, list_data)
   {
      node = (Pathgen_Node *)list_data;
         // North, east, south, west
           if(node->x==best->x    && node->y==best->y -1)peers[0]=node;
      else if(node->x==best->x +1 && node->y==best->y   )peers[1]=node;
      else if(node->x==best->x    && node->y==best->y +1)peers[2]=node;
      else if(node->x==best->x -1 && node->y==best->y   )peers[3]=node;
      else if(priv->i_path_search_diagonal)
      {  // north east, north west, south east, south west
           if(node->x==best->x +1 && node->y==best->y -1)peers[4]=node;
      else if(node->x==best->x -1 && node->y==best->y -1)peers[5]=node;
      else if(node->x==best->x +1 && node->y==best->y +1)peers[6]=node;
      else if(node->x==best->x -1 && node->y==best->y +1)peers[7]=node;
         
      }
   }

   /* search closed list for neighbour */
   EINA_LIST_FOREACH(path->closed, l, list_data)
   {
      node = (Pathgen_Node *)list_data;
         // North, east, south, west
           if(node->x==best->x    && node->y==best->y -1)peers[0]=node;
      else if(node->x==best->x +1 && node->y==best->y   )peers[1]=node;
      else if(node->x==best->x    && node->y==best->y +1)peers[2]=node;
      else if(node->x==best->x -1 && node->y==best->y   )peers[3]=node;
      {  // north east, north west, south east, south west
           if(node->x==best->x +1 && node->y==best->y -1)peers[4]=node;
      else if(node->x==best->x -1 && node->y==best->y -1)peers[5]=node;
      else if(node->x==best->x +1 && node->y==best->y +1)peers[6]=node;
      else if(node->x==best->x -1 && node->y==best->y +1)peers[7]=node;
         
      }

   }
   
   for(i=0; i<j; i++)
   {
      /* if the node already exists, skip */
      if(peers[i])
      {
         continue;
      }

      /* if the node is out of bounds skip */
           if(best->x+1 > priv->w-1) continue;
      else if(best->x-1 < 0) continue;
      else if(best->y+1 > priv->h-1) continue;
      else if(best->y-1 < 0) continue;

      /* setup new coordinates changes */
           if(i==0){ x = best->x  ; y = best->y-1;}//north
      else if(i==1){ x = best->x+1; y = best->y  ;}//east
      else if(i==2){ x = best->x  ; y = best->y+1;}//south
      else if(i==3){ x = best->x-1; y = best->y  ;}//west
      else if(i==4){ x = best->x+1; y = best->y-1;}//north east
      else if(i==5){ x = best->x-1; y = best->y-1;}//north west
      else if(i==6){ x = best->x+1; y = best->y+1;}//south east
      else if(i==7){ x = best->x-1; y = best->y+1;}//south west

      /* create the new node*/
      peers[i] = pathgen_node_create(path->world, x, y);
      if(!peers[i])
      {
         fprintf(stderr,
            "ERR:path_search, node not created as expected\n");
         continue;
      }
      peers[i]->parent = best;
      
      peers[i]->g = priv->hueristic(path, peers[i]);
     
      /* add the node to the open list */
      path->open = eina_list_append(path->open, peers[i]);

      /* paint the node */
      if(priv->i_display_search)
         image_func_pixel(priv->search, peers[i]->x, peers[i]->y, NULL, 0x88008888);
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
      image_func_pixel(priv->path, path->current->x, path->current->y, NULL, 0xFFFFFF00);
      if(!pathgen_path_walk(path))
      {
         ret = EINA_FALSE;
      }
   }
   if(!ret)
   {
      while(pathgen_path_walk(path))
         image_func_image(priv->heatmap, path->current->x, path->current->y,
            pixel_add, priv->i_path_walk_brush, priv->i_path_walk_strength);
  
      evas_object_smart_callback_call(path->world, EVT_SIM_TRAVELER_NEW, NULL);
   }
   return ret;
}

/*************
* Hueristics *
*************/

double
hueristic_dijkstra(Pathgen_Path *path, Pathgen_Node *node)
{
   int dist_m, dist_d, pathmap;
   float desasc, dist_e;
   PATHGEN_WORLD_DATA_GET(path->world, priv);

   dist_m = pathgen_node_dist_manhat(node, path->start);
   dist_d = pathgen_node_dist_diagon(node, path->start);
   dist_e = pathgen_node_dist_euclid(node, path->start);
   desasc = abs(node->z - path->current->z);
   pathmap = 255 - image_pixel_value_get(priv->heatmap,
      node->x, node->y, 0xFF000000, 24);

   return
       dist_m * priv->i_path_inf_dist_manhat
     + dist_d * priv->i_path_inf_dist_diagon
     + dist_e * priv->i_path_inf_dist_euclid
     + desasc * priv->i_path_inf_desasc
     + pathmap* priv->i_path_inf_path;
}

double
hueristic_best_first(Pathgen_Path *path, Pathgen_Node *node)
{
   int dist_m, dist_d, pathmap;
   float desasc, dist_e;
   PATHGEN_WORLD_DATA_GET(path->world, priv);

   dist_m = pathgen_node_dist_manhat(node, path->goal);
   dist_d = pathgen_node_dist_diagon(node, path->goal);
   dist_e = pathgen_node_dist_euclid(node, path->goal);

   desasc = abs(node->z - path->current->z);

   pathmap = 255 - image_pixel_value_get(priv->heatmap,
      node->x, node->y, 0xFF000000, 24);

   return
       dist_m * priv->i_path_inf_dist_manhat
     + dist_d * priv->i_path_inf_dist_diagon
     + dist_e * priv->i_path_inf_dist_euclid
     + desasc * priv->i_path_inf_desasc
     + pathmap* priv->i_path_inf_path;
}


double
hueristic_astar(Pathgen_Path *path, Pathgen_Node *node)
{
   int dist_m, dist_d, pathmap;
   float desasc, dist_e;
   PATHGEN_WORLD_DATA_GET(path->world, priv);

   dist_m = pathgen_node_dist_manhat(node, path->goal) + pathgen_node_dist_manhat(node, path->start);
   dist_d = pathgen_node_dist_diagon(node, path->goal) + pathgen_node_dist_diagon(node, path->start);
   dist_e = pathgen_node_dist_euclid(node, path->goal) + pathgen_node_dist_euclid(node, path->start);

   desasc = abs(node->z - path->current->z);

   pathmap = 255 - image_pixel_value_get(priv->heatmap,
      node->x, node->y, 0xFF000000, 24);

   return
       dist_m * priv->i_path_inf_dist_manhat
     + dist_d * priv->i_path_inf_dist_diagon
     + dist_e * priv->i_path_inf_dist_euclid
     + desasc * priv->i_path_inf_desasc
     + pathmap* priv->i_path_inf_path;
}

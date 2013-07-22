#include <Eina.h>
#include "pg_data.h"
#include "pg_path_finder.h"
#include "pg_node_rel.h"
#include "pg_world.h"

extern PG_Data pg_data;

 /* to create more path searches */
extern int _event_id_path_more;
/* for when the simulation is cancelled or finishes */
extern int _event_id_sim_stop;
/* used to fade the heatmap */
extern int _event_id_path_fade;
/* used to draw the path */
extern int _event_id_path_draw;


PG_Path_Finder *
pg_path_finder_new(PG_Path *path, int gx, int gy)
{
   int i,j,k,l,m,x,y,n;
   PG_World *w = pg_data.world;

   PG_Path_Finder *pf = malloc(sizeof(PG_Path_Finder));
   pf->open = NULL;
   pf->path = path;
   pf->open = eina_list_append(pf->open, path->start);
   path->start->state = 1;

   pf->goal = pg_node_rel_new(pg_world_node_get(pg_data.world, gx, gy));

   pf->iteration_count = 0;
   pf->iteration_max = 100000;//FIXME get from interface
   pf->diagonal_search = EINA_FALSE; //FIXME, get from interface

   // FIXME, get from interface */
   pf->heat_minl = pf->map_minl = pf->avoid_minl = pf->climb_minl =   0; 
   pf->heat_min  = pf->map_min  = pf->avoid_min  = pf->climb_min  =   0;
   pf->heat_max  = pf->map_max  = pf->avoid_max  = pf->climb_max  = 255;
   pf->heat_maxl = pf->map_maxl = pf->avoid_maxl = pf->climb_maxl = 255;

   /* create the node relationship graph */
   pf->all = malloc(w->width * w->length * sizeof *pf->all);
   for(i=0; i < w->width; i++)
      for(j=0; j < w->length; j++)
   {
      n = i+ w->width * j;
      if(i == gx && j == gy)
      {
         pf->all[n] = pf->goal;
         continue;
      }
      if(i == path->start->node->x && j == path->start->node->y)
      {
         pf->all[n] = pf->path->start;
         continue;
      }
      pf->all[n] = pg_node_rel_new(w->nodes[n]);
   }

   /* assign neighbour relationships */
   for(i=0; i < w->width; i++)
      for(j=0; j < w->length; j++)
   {
      n = i + w->width * j;
      for(k=0; k < 8; k++)
      {
         /* setup new coordinates changes */
              if(k==0){ x = i  ; y = j-1;}//north
         else if(k==1){ x = i+1; y = j  ;}//east
         else if(k==2){ x = i  ; y = j+1;}//south
         else if(k==3){ x = i-1; y = j  ;}//west
         else if(k==4){ x = i+1; y = j-1;}//north east
         else if(k==5){ x = i-1; y = j-1;}//north west
         else if(k==6){ x = i+1; y = j+1;}//south east
         else if(k==7){ x = i-1; y = j+1;}//south west
         /* skip the neighbour if it is out of bounds*/
              if(x > w->width-1) continue;
         else if(x < 0) continue;
         else if(y > w->length-1) continue;
         else if(y < 0) continue;
         pf->all[n]->n[k] = pf->all[x + w->width * y];
      }
   }

   return pf;
}

void
pg_path_finder_del(PG_Path_Finder *pf)
{
   int i, j, n;
   Eina_Bool ok;
   /* PG_Path *path; - dont delete */
   /* PG_World *world; - dont delete */
   /* Eina_List *open; */ eina_list_free(pf->open);
   /* PG_Node_Rel *goal; - dont delete */

   /* PG_Node_Rel **all; - contents*/
//   for(i=0; i< pf->world->width; i++)
//      for(j=0; j<pf->world->length; j++)
//   {
      /* selectively delete remaining nodes that arent in the path */
//      n = i + pf->world->width * j;
//      ok = EINA_TRUE;
//      while(1)
//      {
//         if(pf->path->current->node->x == i && pf->path->current->node->y == j)ok = EINA_FALSE;
//         if(pf->path->current == pf->path->start) {
//            pf->path->current = pf->path->end;
//            break;
//         }
//         pf->path->current = pf->path->current->prev;
//      }
      //FIXME if(ok)free(pf->all[n]);
//   }
   /* PG_Node_Rel **all; */
   free(pf->all); 
   free(pf); 
}

PG_Node_Rel *
pg_path_finder_best_get(PG_Path_Finder *pf)
{
   Eina_List *l;
   void *list_data;
   PG_Node_Rel *best, *current;

   if(!pf)return NULL;
   if(eina_list_count(pf->open) == 0)
   {
      fprintf(stderr, "%i nodes in open list\n",
         eina_list_count(pf->open));
      return NULL;
   }

   /* get next best node */
   best = eina_list_data_get(pf->open);
   EINA_LIST_FOREACH(pf->open, l, list_data)
   {
      current = (PG_Node_Rel *)list_data;
      if(current)
         if(current->f < best->f)best = current;
         else if(current->f == best->f && rand() % 10 < 5)best = current;
   }
   return best;
}

/*************************/
/* threaded path finding */
/*************************/
void
pg_path_finder_th_do(void *data, Ecore_Thread *th)
{
   PG_Node_Rel *best, *n;
   int i, d;

   PG_Path_Finder *pf = data;

   while(pf->iteration_count < pf->iteration_max)
   {
      /* bail if no next best node */
      best = pg_path_finder_best_get(pf);
      if(!best) {
         fprintf(stderr, "no best node\n");
         return;
      }

      pf->iteration_count++;
      pf->path->end = best;
      pf->path->current = best;

      /* bail if we have reached our goal */
      if(best->node == pf->goal->node){
         fprintf(stderr, "goal reached\n");
         return;
      }

      /* move the best node to the closed list */
      pf->open = eina_list_remove(pf->open, best);
      best->state = -1;

      /* examine the neighbours */
      if(pf->diagonal_search)d=8;
      else d=4;

      for(i=0; i<d; i++)
      {
         if(!best->n[i]) continue;
         if(best->n[i]->state != 0) continue;
         pf->open = eina_list_append(pf->open, best->n[i]);
         best->n[i]->state = 1;
         best->n[i]->prev = best;
         best->n[i]->g = best->g + 0.9;
         best->n[i]->h = pg_node_dist_manhat(
            best->n[i]->node->x, best->n[i]->node->y,
            pf->goal->node->x, pf->goal->node->y);
         best->n[i]->f = best->n[i]->g + best->n[i]->h;
      }
   }
   fprintf(stderr, "iteration limit reached\n");
}

void // thread job finished
pg_path_finder_th_end(void *data, Ecore_Thread *th)
{
   Evas_Object *ui;
   PG_Path_Finder *th_data = data;

   /* decrement path que */
   pg_data.path_que_count--;

   if(pg_data.path_que_count == 0)
   {
      /* construct new event data and add the event */
      ecore_event_add(_event_id_sim_stop, NULL, NULL, NULL);
   }
   else
   {
      /* construct new event data and add the event */
      ecore_event_add(_event_id_path_more, NULL, NULL, NULL);
   }
   /* if fade counter is at max, call fade callback and reset fade counter */
   ui = evas_object_name_find(pg_data.evas, "ui,fade");
   if(pg_data.path_fade_count > elm_spinner_value_get(ui))
   {
      ecore_event_add(_event_id_path_fade, NULL, NULL, NULL);
      pg_data.path_fade_count = 0;
   }
   /* create event data to pass the resulting path to a callback func */
   Evt_Path *event_info = malloc(sizeof(Evt_Path));
   event_info->path = th_data->path;
   ecore_event_add(_event_id_path_draw, event_info, NULL, NULL);

   /* delete our path finder object */
   pg_path_finder_del(th_data);
}

void // if the thread is cancelled
pg_path_finder_th_cancel(void *data, Ecore_Thread *th)
{
   PG_Path_Finder *th_data = data;
   fprintf(stderr, "thread cancel\n\n\n");
   free(th_data);
}


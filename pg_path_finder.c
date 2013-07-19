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
pg_path_finder_new(PG_Path *path, int x, int y)
{
   PG_Path_Finder *pf = malloc(sizeof(PG_Path_Finder));
   pf->open = NULL;
   pf->path = path;
   pf->open = eina_list_append(pf->open, path->start);
   path->start->state = 1;

   pf->goal = pg_node_rel_new(pg_world_node_get(pg_data.world,x,y));

   pf->iteration_count = 0;
   pf->iteration_max = 100;//FIXME get from interface
   pf->diagonal_search = EINA_FALSE; //FIXME, get from interface

   // FIXME, get from interface */
   pf->heat_minl = pf->map_minl = pf->avoid_minl = pf->climb_minl =0; 
   pf->heat_min = pf->map_min = pf->avoid_min = pf->climb_min = 0;
   pf->heat_max = pf->map_max = pf->avoid_max = pf->climb_max = 255;
   pf->heat_maxl = pf->map_maxl = pf->avoid_maxl = pf->climb_maxl = 255;

   /* create the node graph */
   int i,j,k,l,m,u,v;
   PG_Node_Rel *node;

   k = pg_data.world->width * pg_data.world->length;
   pf->all = malloc(k * sizeof (void *));

   for(i=0; i < k; i++)
      pf->all[k] = pg_node_rel_new(pg_data.world->nodes[k]);

   for(i=0; i < k; i++)
   {
      node = pf->all[k];
      for(j=0; j<8; j++)
      {
         /* setup new coordinates changes */
              if(j==0)l = i - pg_data.world->width;    //north
         else if(j==1)l = i + 1;                       //east
         else if(j==2)l = i + pg_data.world->width;    //south
         else if(j==3)l = i - 1;                       //west
         else if(j==4)l = i - pg_data.world->width + 1;//north east
         else if(j==5)l = i - pg_data.world->width - 1;//north west
         else if(j==6)l = i + pg_data.world->width + 1;//south east
         else if(j==7)l = i + pg_data.world->width - 1;//south west
         /* skip the neighbour if it is out of bounds*/
         if(l < 0) continue;
         if(l > k) continue;
         if(l % pg_data.world->width == 0) continue;
         if(l % pg_data.world->width == pg_data.world->width - 1)continue;
         node->n[j] = pf->all[l];
      }
   }
   return pf;
}

void
pg_path_finder_del(PG_Path_Finder *pf)
{
   int i, j, k;
   PG_Node_Rel *node;
   eina_list_free(pf->open);
   pg_node_rel_del(pf->goal);
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
   fprintf(stderr, "%p\n", pf);
   fprintf(stderr, "%p\n", pf->open);
   fprintf(stderr, "%i\n", eina_list_count(pf->open));

   while(pf->iteration_count < pf->iteration_max)
   {
      fprintf(stderr, "i=%i\n", pf->iteration_count);
      /* bail if no next best node */
      best = pg_path_finder_best_get(pf);
      if(!best)
      {
         fprintf(stderr, "no best node\n");
         return;
      }
      fprintf(stderr, "b=(%i, %i)\n", best->node->x, best->node->y);

      pf->iteration_count++;
      pf->path->end = best;
      pf->path->current = best;

      /* bail if we have reached our goal */
      if(best->node == pf->goal->node)return;

      /* move the best node to the closed list */
      pf->open = eina_list_remove(pf->open, best);
      best->state = -1;

      /* examine the neighbours */
      if(pf->diagonal_search)d=8;
      else d=4;

      for(i=0; i<d; i++)
      {
         fprintf(stderr, "bn=%p\n", best->n[i]);
//         if(!best->n[i] || best->n[i]->state != 0) continue;
//         pf->open = eina_list_append(pf->open, best->n[i]);
//         best->n[i]->state = 1;
//         best->n[i]->prev = best;
//         best->n[i]->f = best->f + 1;
      }
   }
}

void // thread job finished - collect results and put in img obj
pg_path_finder_th_end(void *data, Ecore_Thread *th)
{
   fprintf(stderr, "thread finish\n");
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
   Evt_Path *event_info = malloc(sizeof(Evt_Path));
   event_info->path = th_data->path;
   ecore_event_add(_event_id_path_draw, event_info, NULL, NULL);
   pg_path_finder_del(th_data);
}

void // if the thread is cancelled - free pix, keep obj tho
pg_path_finder_th_cancel(void *data, Ecore_Thread *th)
{
   PG_Path_Finder *th_data = data;
   fprintf(stderr, "thread cancel\n\n\n");
   free(th_data);
}


static Eina_Bool
_path_more(void *data, int type, void *event_info)
{
   Evas_Object *ui;
   int i, j, x, y;

   fprintf(stderr, "_path_more: type=%d\n", type);

   while(pg_data.path_que_count < pg_data.path_que_size)
   {
      ui = evas_object_name_find(pg_data.evas, "ui,path_max");
      if(pg_data.path_count >= elm_spinner_value_get(ui))break;

      /* conditions cleared for a new path to be generated */

      /* increment counters */
      pg_data.path_count++;
      pg_data.path_que_count++;
      pg_data.path_fade_count++;

      /* set random position to use as start location */
      x = rand() % pg_data.world->width;
      y = rand() % pg_data.world->length;
      fprintf(stderr, "start location (%d, %d)\n", x, y);

      /* Create a new path */
      PG_Path *new_path = pg_path_new(x,y);

      /* set a goal */
      x = rand() % pg_data.world->width;
      y = rand() % pg_data.world->length;

      /* create a path finder */
      PG_Path_Finder *pf = pg_path_finder_new(new_path, x, y);
      fprintf(stderr, "%p\n", pf);
      fprintf(stderr, "%p\n", pf->open);
      fprintf(stderr, "%i\n", eina_list_count(pf->open));

      /* push of onto new thread to solve without blocking the interface */
      ecore_thread_run(
         pg_path_finder_th_do,
         pg_path_finder_th_end,
         pg_path_finder_th_cancel, pf);
   }

   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
_sim_stop(void *data, int type, void *event_info)
{
   Evas_Object *ui;
   int i;

   fprintf(stderr, "_sim_stop: type=%d\n", type);

   /* re-enable user interface */
   ui = evas_object_name_find(pg_data.evas, "ui,start");
   elm_object_disabled_set(ui, EINA_FALSE);

   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
_path_fade(void *data, int type, void *event_info)
{
   fprintf(stderr, "_path_fade\n");
   //FIXME actually fade something 
}

static Eina_Bool
_path_draw(void *data, int type, void *event_info)
{
   fprintf(stderr, "_path_draw\n");
   Evt_Path *evt_info = event_info;
   PG_Path *path = evt_info->path;
   fprintf(stderr, "from (%i, %i,) to (%i, %i)\n",
      path->start->node->x,
      path->start->node->y,
      path->end->node->x,
      path->end->node->y);
   //FIXME
}

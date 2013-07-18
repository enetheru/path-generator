static void
th_do(void *data, Ecore_Thread *th)
{
   PG_Path_Finder *th_data = data;
   int i;
   sleep(rand()%3);
      fprintf(stderr, "pendig=%d, active=%d\n",
         ecore_thread_pending_get(), ecore_thread_active_get() );
}
//
// END - code running in my custom thread instance

static void // thread job finished - collect results and put in img obj
th_end(void *data, Ecore_Thread *th)
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
   free(th_data);
}

static void // if the thread is cancelled - free pix, keep obj tho
th_cancel(void *data, Ecore_Thread *th)
{
   PG_Path_Finder *th_data = data;
   fprintf(stderr, "thread cancel\n\n\n");
   free(th_data);
}

static Eina_Bool
_path_more(void *data, int type, void *event_info)
{
   Evas_Object *ui;
   int i, j, x, y, z;

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


   /* thoughts
* create a new path
* set its start node
* create a path finder using the path as its data
* set a goal
* solve path
* delete path finder and related data leaving the path for other things

      /* allocate data for the search */
      PG_Path_Finder *new_search = pg_path_finder_new();
      new_search->path = malloc(sizeof(PG_Path));
      /* set the start */
      x = rand() % pg_data.world->width;
      y = rand() % pg_data.world->length;
      z = rand() % pg_data.world->height;
      fprintf(stderr, "start location (%d, %d, %d)\n", x, y, z);
//      new_search->start = malloc(sizeof(PG_Node_Rel));
//      new_search->start->node = pg_world_get_node(pg_data.world, x, y, z);
//      new_search->open = eina_list_append(new_search->open, new_search->start);
      /* set the goal */
      x = rand() % pg_data.world->width;
      y = rand() % pg_data.world->length;
      z = rand() % pg_data.world->height;
      fprintf(stderr, "goal location (%d, %d, %d)\n", x, y, z);
      new_search->goal = malloc(sizeof(PG_Node_Rel));
      new_search->goal->node = pg_world_get_node(pg_data.world, x, y, z);
      
      /* push of onto new thread to solve without blocking the interface */
      ecore_thread_run(th_do, th_end, th_cancel, new_search);
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


static void 
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   // quit the mainloop (elm_run function will return)
   free(pg_data.world);

   elm_exit();
}

static void
_ui_load_image(void *data, Evas_Object *obj, void *event_info)
{
   Evas *evas;
   Evas_Object *image;
   int id;
   const char *file = event_info;
   if(!file) return;

   evas = evas_object_evas_get(obj);

   image = evas_object_image_filled_add(evas);
   evas_object_image_file_set(image, file, NULL);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);

   if(!strcmp(evas_object_name_get(obj), ui_labels[1]))id=0;
   if(!strcmp(evas_object_name_get(obj), ui_labels[4]))id=1;
   if(!strcmp(evas_object_name_get(obj), ui_labels[7]))id=2;
   if(!strcmp(evas_object_name_get(obj), ui_labels[10]))id=3;
   if(!strcmp(evas_object_name_get(obj), ui_labels[13]))id=4;

   pg_world_layer_set(pg_data.world, image, id);
   pg_display_layer_set(pg_data.display, image, id);
   fprintf(stderr, "%s, %i\n", evas_object_name_get(obj), id);
}

static void
_ui_toggle_image(void *data, Evas_Object *obj, void *event_info)
{
   int id;
   PG_DISPLAY_DATA_GET_OR_RETURN(pg_data.display, priv);

   if(!strcmp(evas_object_name_get(obj), ui_labels[0]))id=0;
   if(!strcmp(evas_object_name_get(obj), ui_labels[3]))id=1;
   if(!strcmp(evas_object_name_get(obj), ui_labels[6]))id=2;
   if(!strcmp(evas_object_name_get(obj), ui_labels[9]))id=3;
   if(!strcmp(evas_object_name_get(obj), ui_labels[12]))id=4;

   if(elm_check_state_get(obj))
      evas_object_show(priv->l[id]);
   else
      evas_object_hide(priv->l[id]);
   fprintf(stderr, "%s, %i\n", evas_object_name_get(obj), id);
   evas_object_smart_changed(pg_data.display);
}

static void
_ui_sim_start(void *data, Evas_Object *obj, void *event_info)
{
   /* check data for OK to proceed */
   

   /* clear counters */
   pg_data.path_count = 0;
   pg_data.path_fade_count = 0;
   pg_data.path_que_count=0;

   /* disable user intarface that would cause problems */
   elm_object_disabled_set(obj, EINA_TRUE);

   /* begin simulation */
   ecore_event_add(_event_id_path_more, NULL, NULL, NULL);
}

static void
_ui_sim_stop(void *data, Evas_Object *obj, void *event_info)
{
   /* cancel path solving */
   ecore_event_add(_event_id_sim_stop, NULL, NULL, NULL);
}




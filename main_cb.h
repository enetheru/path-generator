static Eina_Bool
_path_more(void *data, int type, void *event_info)
{
   Event_Data_Path_More *event_data = event_info;
   Evas_Object *ui;
   int i;

   fprintf(stderr, "_path_more: type=%d\n", type);

   ui = evas_object_name_find(event_data->evas, "ui,path_max");
   i = (int)elm_spinner_value_get(ui);
   while(pg_data.path_que_count < pg_data.path_que_size)
   {
      if(pg_data.path_count >= i)break;

      fprintf(stderr, "moar paths!! %d\n", pg_data.path_count+1);
      pg_data.path_count++;

      Event_Data_Path_More *evt_data = malloc(sizeof(Event_Data_Path_More));
      evt_data->evas = event_data->evas;
      ecore_event_add(_event_id_path_more, evt_data, NULL, NULL);

      pg_data.path_que_count++;
   }
   if(pg_data.path_que_count == 0)
      ecore_event_add(_event_id_sim_stop, NULL, NULL, NULL);
   pg_data.path_que_count--;
   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
_sim_stop(void *data, int type, void *event)
{
   int *number = event;
   const char *str = data;
   fprintf(stderr, "_sim_stop: type=%d\n", type);

   return ECORE_CALLBACK_PASS_ON;
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
   Evas *evas = evas_object_evas_get(obj);
   /* check data for OK to proceed */
   pg_data.path_count = 0;
   pg_data.path_fade_count = 0;
   pg_data.path_que_count=0;
   /* disable user intarface that would cause problems */
   elm_object_disabled_set(obj, EINA_TRUE);
   /* begin simulation */
   Event_Data_Path_More *evt_data = malloc(sizeof(Event_Data_Path_More));
   evt_data->evas = evas;
   ecore_event_add(_event_id_path_more, evt_data, NULL, NULL);
}

static void
_ui_sim_stop(void *data, Evas_Object *obj, void *event_info)
{
   Evas *evas = evas_object_evas_get(obj);
   Evas_Object *ui;
   /* cancel path solving */
   ecore_event_add(_event_id_sim_stop, NULL, NULL, NULL);
   /* re-enable user interface */
   ui = evas_object_name_find(evas, "ui,start");
   elm_object_disabled_set(ui, EINA_FALSE);
}

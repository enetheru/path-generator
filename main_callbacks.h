static void 
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   // quit the mainloop (elm_run function will return)
   elm_exit();
}

/* hook on the file,chosen smart callback */

static void
_wheel( void *data, Evas *evas, Evas_Object *o, void *event_info)
{
   Evas_Object *world = evas_object_name_find(evas, "world");
   evas_object_smart_callback_call( world, EVT_ZOOM, event_info);
}

static void
_btn_sim_start(void *data, Evas_Object *o, void *event_info)
{
   fprintf(stderr, "want to start sim\n");
   Evas_Object *world = evas_object_name_find(evas_object_evas_get(o), "world");
   evas_object_smart_callback_call( world, EVT_SIM_START, event_info);
   return;
}

static void
_btn_sim_stop(void *data, Evas_Object *o, void *event_info)
{
   fprintf(stderr, "want to stop sim\n");
   return;
}

static void
_btn_sim_reset(void *data, Evas_Object *o, void *event_info)
{
   fprintf(stderr, "want to reset sim\n");
   return;
}

static void 
_fs_load_height(void *data, Evas_Object *obj, void *event_info)
{
   const char *file = event_info;
   Evas *evas;
   Evas_Object *image, *world;

   if (file) printf("File chosen: %s\n", file);
   else
   {
      printf("File selection canceled.\n");
      return;
   }

   evas = evas_object_evas_get(obj);
   world = evas_object_name_find(evas, "world");

   image = evas_object_image_filled_add(evas);
   evas_object_image_file_set(image, file, NULL);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);

   pathgen_world_height_set(world, image);
}

static void
_btn_generate(void *data, Evas_Object *o, void *event_info)
{
   fprintf(stderr, "want to generate world\n");
   Evas_Object *image, *world;
   Evas *evas;

   evas = evas_object_evas_get(o);
   world = evas_object_name_find(evas, "world");

   image = image_generate_random(evas, 100, 100);

   pathgen_world_height_set(world, image);
}

static void
_spinner_path_speed_change(void *data, Evas_Object *o, void *event_info)
{
   float value;
   Evas *evas;
   Evas_Object *world;
   
   evas = evas_object_evas_get(o);
   world = evas_object_name_find(evas, "world");
   if(!world)return;
   
   PATHGEN_WORLD_DATA_GET(world, priv);
   priv->i_path_search_iter_speed = (float)elm_spinner_value_get(o);
}

static void
_spinner_sim_travelers(void *data, Evas_Object *o, void *event_info)
{
   float value;
   Evas *evas;
   Evas_Object *world;
   
   evas = evas_object_evas_get(o);
   world = evas_object_name_find(evas, "world");
   if(!world)return;
   
   PATHGEN_WORLD_DATA_GET(world, priv);
   priv->i_world_travelers = (int)elm_spinner_value_get(o);
}

static void
_spinner_path_search_iter_max(void *data, Evas_Object *o, void *event_info)
{
   float value;
   Evas *evas;
   Evas_Object *world;
   
   evas = evas_object_evas_get(o);
   world = evas_object_name_find(evas, "world");
   if(!world)return;
   
   PATHGEN_WORLD_DATA_GET(world, priv);
   priv->i_path_search_iter_max = (int)elm_spinner_value_get(o);
}

static void
_btn_save_path_heatmap(void *data, Evas_Object *o, void *event_info)
{
   Evas *evas;
   Evas_Object *world;
   if(!event_info)return;
   
   evas = evas_object_evas_get(o);
   world = evas_object_name_find(evas, "world");
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->heat)return;

   evas_object_image_save(priv->heat, (const char *)event_info, NULL, NULL);
   return;
}


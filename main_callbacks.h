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
   Evas_Object *world = evas_object_name_find(evas_object_evas_get(o), "world");
   evas_object_smart_callback_call( world, EVT_SIM_START, event_info);
   fprintf(stderr, "want to start sim\n");
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
_file_chosen(void *data, Evas_Object *obj, void *event_info)
{
   const char *file = event_info;

   if (file) printf("File chosen: %s\n", file);
   else
   {
      printf("File selection canceled.\n");
      return;
   }
   Evas *evas = evas_object_evas_get(obj);
   Evas_Object *world = evas_object_name_find(evas, "world");
   Evas_Object *image, *old;

   image = evas_object_image_filled_add(evas);
   evas_object_image_file_set(image, file, NULL);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);
   evas_object_show(image);

   old = pathgen_world_set_height(world, image);
   evas_object_del(old);
}

static void
_btn_generate(void *data, Evas_Object *o, void *event_info)
{
   fprintf(stderr, "want to generate world\n");
   Evas_Object *world = evas_object_name_find(evas_object_evas_get(o), "world");
   evas_object_smart_callback_call( world, EVT_WORLD_GENERATE, event_info);
}

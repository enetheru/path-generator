
static void 
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   // quit the mainloop (elm_run function will return)
   free(pg_data.world);

   elm_exit();
}

static void
_load_image(void *data, Evas_Object *obj, void *event_info)
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

   if(!strcmp(evas_object_name_get(obj), ui_labels[0]))id=0;
   if(!strcmp(evas_object_name_get(obj), ui_labels[2]))id=1;
   if(!strcmp(evas_object_name_get(obj), ui_labels[4]))id=2;
   if(!strcmp(evas_object_name_get(obj), ui_labels[6]))id=3;
   if(!strcmp(evas_object_name_get(obj), ui_labels[8]))id=4;

   pg_world_layer_set(pg_data.world, image, id);
   pg_display_layer_set(pg_data.display, image, id);
   fprintf(stderr, "%s, %i\n", evas_object_name_get(obj), id);
}

static void
_toggle_image(void *data, Evas_Object *obj, void *event_info)
{
   int id;
   PG_DISPLAY_DATA_GET_OR_RETURN(pg_data.display, priv);

   if(!strcmp(evas_object_name_get(obj), ui_labels[12]))id=0;
   if(!strcmp(evas_object_name_get(obj), ui_labels[14]))id=1;
   if(!strcmp(evas_object_name_get(obj), ui_labels[16]))id=2;
   if(!strcmp(evas_object_name_get(obj), ui_labels[18]))id=3;
   if(!strcmp(evas_object_name_get(obj), ui_labels[20]))id=4;

   if(elm_check_state_get(obj))
      evas_object_show(priv->l[id]);
   else
      evas_object_hide(priv->l[id]);
   fprintf(stderr, "%s, %i\n", evas_object_name_get(obj), id);
   evas_object_smart_changed(pg_data.display);
}

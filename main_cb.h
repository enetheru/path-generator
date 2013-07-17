
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

   if(strcmp(evas_object_name_get(obj), ui_labels[0]))id=0;
   if(strcmp(evas_object_name_get(obj), ui_labels[2]))id=1;
   if(strcmp(evas_object_name_get(obj), ui_labels[4]))id=2;
   if(strcmp(evas_object_name_get(obj), ui_labels[6]))id=3;
   if(strcmp(evas_object_name_get(obj), ui_labels[8]))id=4;

   pg_world_layer_set(pg_data.world, image, id);
}

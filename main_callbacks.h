static void 
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   // quit the mainloop (elm_run function will return)
   elm_exit();
}

/* hook on the file,chosen smart callback */
static void
_key_down( void *data, Evas *evas, Evas_Object *o, void *event_info)
{
   Evas_Object *splash;
   Evas_Event_Key_Down * key = event_info;
   splash = evas_object_name_find(evas_object_evas_get(o), "splash");
   fprintf(stderr, "%s\n", key->keyname); 
   if(!strcmp(key->keyname, "Escape"))
   {
      if(evas_object_visible_get(splash))evas_object_hide(splash);
      else evas_object_show(splash);
   }
}

static void
_wheel( void *data, Evas *evas, Evas_Object *o, void *event_info)
{
   Evas_Object *world = evas_object_name_find(evas, "world");
   evas_object_smart_callback_call( world, EVT_ZOOM, event_info);
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

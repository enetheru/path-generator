#include "r_image.h"

static void 
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   Evas *evas = evas_object_evas_get(obj);
   Evas_Object *world = evas_object_name_find(evas, "world");

   PATHGEN_WORLD_DATA_GET(world, priv);

   if(priv->timer_search)
   {
      fprintf(stderr, "not finished searching yet\n");
      ecore_timer_del(priv->timer_search);
      priv->timer_search = NULL;
   }
   if(priv->timer_path)
   {
      fprintf(stderr, "not finished walking the path yet\n");
      ecore_timer_del(priv->timer_path);
      priv->timer_path = NULL;
   }
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
   if(!priv->l[5])return;

   evas_object_image_save(priv->l[5], (const char *)event_info, NULL, NULL);
   return;
}

static void
_btn_sim_start(void *data, Evas_Object *o, void *event_info)
{
   evas_object_smart_callback_call( data, "sim,start", event_info);
   return;
}

static void
_btn_sim_reset(void *data, Evas_Object *o, void *event_info)
{
   fprintf(stderr, "want to reset sim\n");
   PATHGEN_WORLD_DATA_GET(data, priv);

   image_func_fill(priv->l[5], NULL, 0x00000000);
   return;
}


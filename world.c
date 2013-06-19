#include "world.h"

/* add a new world to canvas */
Evas_Object *
pathgen_world_add( Evas *evas)
{
   const Evas_Smart_Cb_Description **descriptions;
   unsigned int count;

   Evas_Object * world = evas_object_smart_add(evas, _pathgen_world_smart_class_new());

   evas_object_smart_callbacks_descriptions_get(
     world, &descriptions, &count, NULL, NULL);

   for (; *descriptions; descriptions++)
   {
      fprintf(stdout, "We've found a smart callback on the smart object!"
         "\n\tname: %s\n\ttype: %s\n", (*descriptions)->name,
         (*descriptions)->type);

      if (strcmp((*descriptions)->type, "i")) continue;
   }

   /* add callbacks */
   evas_object_smart_callback_add( world, "zoom", _pathgen_world_zoom, NULL);

   evas_object_smart_callback_add( world, "heat,reset", _pathgen_world_heatmap_reset, NULL);

   evas_object_smart_callback_add( world, "heat,clear", _pathgen_world_heatmap_clear, NULL);

   
   return world;
}

/* remove child element, return its pointer( or NULL on errors) */
Evas_Object *
pathgen_world_remove(Evas_Object *o, Evas_Object *child)
{
   long idx;

   PATHGEN_WORLD_DATA_GET_OR_RETURN_VAL(o, priv, NULL);

   int i=0;
   while(EINA_TRUE)
   {
      if (i > 5)
      {
         fprintf(stderr, "you are trying to remove something not belonging to"
            " the example smart object!\n");
         return NULL;
      }
      if (priv->children[i] == child) break;
      i++;
   }
      
   idx = (long)evas_object_data_get(child, "index");
   idx--;

   _pathgen_world_remove_do(priv, child, idx);

   evas_object_smart_callback_call(
      o, EVT_CHILDREN_NUMBER_CHANGED, (void *)(long)priv->child_count);
   evas_object_smart_changed(o);

   return child;
}

/* set to return any previous object set to the height of the
 * world or NULL, if any (or on errors) */
Evas_Object *
pathgen_world_set_height(Evas_Object *o, Evas_Object *height)
{
   Evas_Object *ret;
   int w, h;

   PATHGEN_WORLD_DATA_GET_OR_RETURN_VAL(o, priv, NULL);
   if(!height)
      return NULL;

   if(priv->children[PG_HEIGHT] == height)
   {
      fprintf(stderr, "Maps must be unique objects\n");
      return NULL;
   }

   if (priv->children[PG_HEIGHT])
   {
      if (priv->children[PG_HEIGHT] != height)
      {
         ret = priv->children[PG_HEIGHT];
         _pathgen_world_remove_do(priv, priv->children[PG_HEIGHT], PG_HEIGHT);
      }
      else return height;
   }

   // Assign new object
   priv->children[PG_HEIGHT] = height;
   evas_object_image_size_get(height, &w, &h);
   evas_object_size_hint_min_set(o, w, h);
   
   
   _pathgen_world_child_callbacks_register(o, height, PG_HEIGHT);
   evas_object_smart_member_add(height, o);
   evas_object_smart_changed(o);

   priv->child_count++;
      evas_object_smart_callback_call(o, EVT_HEAT_RESET, height);

   return ret;
}

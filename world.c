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

   evas_object_smart_callback_add( world, "heat,reset", (Evas_Smart_Cb) _pathgen_world_heatmap_reset, NULL);

   evas_object_smart_callback_add( world, "heat,clear", (Evas_Smart_Cb) _pathgen_world_heatmap_clear, NULL);

   PATHGEN_WORLD_DATA_GET(world, priv);
   /* create default 100x100 randomly generated maze */
   Evas_Object *image = evas_object_image_filled_add(evas);
   evas_object_name_set(image, "maze");
   evas_object_image_size_set(image, 100, 100);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);
   evas_object_show(image);

   /* setting the world dimensions */
   priv->w = 100;
   priv->h = 100;

   /* generating the contents of the image */   
   int *data = malloc(100*100*4);
   evas_object_image_data_set(image, data);
   evas_object_image_pixels_dirty_set(image, EINA_TRUE);
   int i = 0;
   int number;
   for(; i < 100*100; i++)
   {
      number = rand() % 3;
      if(number == 0) data[i] = 0xFF000000;
      else data[i] = 0xFFFFFFFF;
   }

   /* sorting out the smart object bullshit(i think) */
   priv->children[PG_HEIGHT] = image;

   _pathgen_world_child_callbacks_register(world, image, PG_HEIGHT);
   evas_object_smart_member_add(image, world);
   evas_object_smart_changed(world);

   priv->child_count++;
      evas_object_smart_callback_call(world, EVT_HEAT_RESET, image);
   
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
   evas_object_image_size_get(height, &(priv->w), &(priv->h));
   evas_object_size_hint_min_set(o, priv->w, priv->h);
   
   
   _pathgen_world_child_callbacks_register(o, height, PG_HEIGHT);
   evas_object_smart_member_add(height, o);
   evas_object_smart_changed(o);

   priv->child_count++;
      evas_object_smart_callback_call(o, EVT_HEAT_RESET, height);

   

   return ret;
}

void
pathgen_world_size_get(Evas_Object *world, int *w, int *h)
{
   PATHGEN_WORLD_DATA_GET(world, priv);

   *w = priv->w;
   *h = priv->h;   
}

Pathgen_Map *
pathgen_world_pathmap_set(Evas_Object *world, Pathgen_Map *map)
{
   Pathgen_Map *ret;
   PATHGEN_WORLD_DATA_GET_OR_RETURN_VAL(world, priv, NULL);
   if(!map) return NULL;

   if(priv->pathmap == map)
   {
      fprintf(stderr, "Maps must be unique objects\n");
      return NULL;
   }

   if (priv->pathmap)
   {
         ret = priv->pathmap;
         _pathgen_world_remove_do(priv, priv->children[PG_VISUAL], PG_VISUAL);
   }

   // Assign new object
   priv->pathmap = map;
   priv->children[PG_VISUAL] = map->visual;

   //set the map to have the world as parent
   map->parent_world = world;
   
   
   _pathgen_world_child_callbacks_register(world, map->visual, PG_VISUAL);
   evas_object_smart_member_add(map->visual, world);
   evas_object_smart_changed(world);

   priv->child_count++;

   return ret;
 
}

void *
pathgen_world_height_get(Evas_Object *world)
{
   PATHGEN_WORLD_DATA_GET(world, priv);
   void * data = evas_object_image_data_get(priv->children[PG_HEIGHT], EINA_FALSE);
   return data;
}

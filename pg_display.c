#include "pg_display.h"

static const Evas_Smart_Cb_Description _smart_callbacks[] = 
{
   {EVT_NULL, "i"},
   {NULL, NULL}
};

EVAS_SMART_SUBCLASS_NEW(_pg_display_type, _pg_display,
   Evas_Smart_Class, Evas_Smart_Class,
   evas_object_smart_clipped_class_get, _smart_callbacks);

/***********************
* evas smart functions *
************************/

/* create and setup our pg_display internals*/
static void
_pg_display_smart_add(Evas_Object *o)
{
   EVAS_SMART_DATA_ALLOC(o, PG_Display_Data);
   _pg_display_parent_sc->add(o);
}

static void
_pg_display_smart_del(Evas_Object *o)
{
   PG_DISPLAY_DATA_GET(o, priv);
   _pg_display_parent_sc->del(o);
}

static void
_pg_display_smart_show(Evas_Object *o)
{
   int i;
   PG_DISPLAY_DATA_GET(o, priv);
   evas_object_show(priv->background);
   for(i=0; i<8; i++)evas_object_show(priv->l[i]);

   _pg_display_parent_sc->show(o);
}

static void
_pg_display_smart_hide(Evas_Object *o)
{
   int i;
   PG_DISPLAY_DATA_GET(o, priv);
   evas_object_hide(priv->background);
   for(i=0; i<8; i++)evas_object_hide(priv->l[i]);

   _pg_display_parent_sc->hide(o);
}

static void
_pg_display_smart_resize(Evas_Object *o,
                                 Evas_Coord w,
                                 Evas_Coord h)
{
   Evas_Coord ow, oh;
   evas_object_geometry_get(o, NULL, NULL, &ow, &oh);
   if ((ow == w) && (oh == h)) return;

   /* this will trigger recalculation */
   evas_object_smart_changed(o);
}

/* act on child objects' properties, before rendering */
static void
_pg_display_smart_calculate(Evas_Object *o)
{
   Evas_Coord x, y, w, h;
   int i;

   PG_DISPLAY_DATA_GET_OR_RETURN(o, priv);
   evas_object_geometry_get(o, &x, &y, &w, &h);

   evas_object_resize(priv->background, w, h);
   evas_object_move(priv->background, x, y);

   evas_object_resize(priv->l[0], w, h);
   evas_object_move(priv->l[0], x, y);

   for(i=1; i<8; i++)
   {
      evas_object_resize(priv->l[i], w, h);
      evas_object_move(priv->l[i], x, y);
      evas_object_raise(priv->l[i]);
   }

}

/* setting our smart interface */
static void
_pg_display_smart_set_user(Evas_Smart_Class *sc)
{
  /* specializing these two */
  sc->add = _pg_display_smart_add;
  sc->del = _pg_display_smart_del;
  sc->show = _pg_display_smart_show;
  sc->hide = _pg_display_smart_hide;
  /* clipped smart object has no hook on resizing or calculations */
  sc->resize = _pg_display_smart_resize;
  sc->calculate = _pg_display_smart_calculate;
}

/*******************
* Public Interface *
*******************/

/* add a new world to canvas */
Evas_Object *
pg_display_add( Evas *evas)
{
   Evas_Object * world = evas_object_smart_add(evas, _pg_display_smart_class_new());

//   evas_object_smart_callback_add( world, EVT_NULL,
//      (Evas_Smart_Cb) _some_function, NULL);

   /* get the private data struct */
   PG_DISPLAY_DATA_GET(world, priv);

   /* add the background object */
   priv->background = evas_object_rectangle_add(evas);
   evas_object_color_set(priv->background, 200, 200, 200, 255);
   evas_object_smart_member_add(priv->background, world);
   evas_object_lower(priv->background);

   return world;
}


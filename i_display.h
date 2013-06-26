#ifndef I_DISPLAY_H
#define I_DISPLAY_H

static void
_chk_toggle_heatmap(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->heat)return;

   if(elm_check_state_get(o))evas_object_show(priv->heat);
   else evas_object_hide(priv->heat);

   return;

}

static void
i_display_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *lab, *chk, *hbox;

   world = evas_object_name_find(evas_object_evas_get(win),"world");

   lab = elm_label_add(win);
   elm_object_text_set(lab, "Display Options");
   elm_box_pack_end(vbox, lab);
   evas_object_show(lab);

   chk = elm_check_add(win);
   elm_check_state_set(chk, EINA_TRUE);
   elm_object_part_text_set(chk, NULL,  "Path Heatmap Display");
   elm_box_pack_end(vbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_toggle_heatmap, world);

}

#endif /*I_DISPLAY_H*/

#ifndef I_DISPLAY_H
#define I_DISPLAY_H

static void
_chk_display_heatmap(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->heat)return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr,
         "i_display_heatmap = on\n");
      priv->i_display_heatmap = EINA_TRUE;
      evas_object_show(priv->heat);
   }
   else
   {
      fprintf(stderr,
         "i_display_heatmap = off\n");
      priv->i_display_heatmap = EINA_FALSE;
      evas_object_hide(priv->heat);
   }

   return;

}

static void
_chk_display_path_search(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->heat)return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr,
         "i_display_path_search = on\n");
      priv->i_display_path_search = EINA_TRUE;
      evas_object_show(priv->visual);
   }
   else 
   {
      fprintf(stderr,
         "i_display_path_search = off\n");
      priv->i_display_path_search = EINA_FALSE;
      evas_object_hide(priv->visual);
   }

   return;

}

static void
i_display_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *lab, *chk, *hbox, *frm;

   world = evas_object_name_find(evas_object_evas_get(win),"world");

   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "Display Options");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox);
   evas_object_show(vbox);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, EINA_TRUE);
   elm_object_part_text_set(chk, NULL,  "Path Heatmap Display");
   elm_box_pack_end(vbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_heatmap, world);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, EINA_TRUE);
   elm_object_part_text_set(chk, NULL,  "Display Path Search");
   elm_box_pack_end(vbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_path_search, world);
}

#endif /*I_DISPLAY_H*/

#ifndef I_DISPLAY_H
#define I_DISPLAY_H

static void
_chk_display_height(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->height)return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr, "i_display_height = true\n");
      priv->i_display_height = EINA_TRUE;
      evas_object_show(priv->height);
   } else {
      fprintf(stderr, "i_display_height = false\n");
      priv->i_display_height = EINA_FALSE;
      evas_object_hide(priv->height);
   }
}

static void
_chk_display_heatmap(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->heatmap)return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr,
         "i_display_heatmap = on\n");
      priv->i_display_heatmap = EINA_TRUE;
      evas_object_show(priv->heatmap);
   }
   else
   {
      fprintf(stderr,
         "i_display_heatmap = off\n");
      priv->i_display_heatmap = EINA_FALSE;
      evas_object_hide(priv->heatmap);
   }
}

static void
_chk_display_search(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->search)return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr,
         "i_display_search = on\n");
      priv->i_display_search = EINA_TRUE;
      evas_object_show(priv->search);
   }
   else 
   {
      fprintf(stderr,
         "i_display_search = off\n");
      priv->i_display_search = EINA_FALSE;
      evas_object_hide(priv->search);
   }
}
static void
_chk_display_path(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->path)return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr,
         "i_display_path = on\n");
      priv->i_display_path = EINA_TRUE;
      evas_object_show(priv->path);
   }
   else 
   {
      fprintf(stderr,
         "i_display_path = off\n");
      priv->i_display_path = EINA_FALSE;
      evas_object_hide(priv->path);
   }
}

static void
_spin_display_speed(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_display_speed = (float)elm_spinner_value_get(o);
   fprintf(stderr, "i_display_speed = %f\n", priv->i_display_speed);
}

static void
i_display_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *lab, *chk, *hbox, *frm, *spin;

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

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(chk, EVAS_HINT_EXPAND, 0.0);
   elm_check_state_set(chk, I_DISPLAY_HEIGHT_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Height");
   elm_box_pack_end(hbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_height, world);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, I_DISPLAY_HEATMAP_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Heatmap");
   elm_box_pack_end(hbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_heatmap, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(chk, EVAS_HINT_EXPAND, 0.0);
   elm_check_state_set(chk, I_DISPLAY_SEARCH_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Path Search");
   elm_box_pack_end(hbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_search, world);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, I_DISPLAY_PATH_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Path");
   elm_box_pack_end(hbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_path, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* speed */
   lab = elm_label_add(win);
   elm_object_text_set(lab, "Speed limit");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%0.3f");
   elm_spinner_min_max_set(spin, 0.001, 1.0);
   elm_spinner_step_set(spin, 0.001);
   elm_spinner_value_set(spin, I_DISPLAY_SPEED_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_display_speed, world);

}

#endif /*I_DISPLAY_H*/

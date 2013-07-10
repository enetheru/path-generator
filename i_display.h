#ifndef I_DISPLAY_H
#define I_DISPLAY_H

static void
_chk_display_height(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[0])return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr, "i_display_height = true\n");
      priv->i_display_[0] = EINA_TRUE;
      evas_object_show(priv->l[0]);
   } else {
      fprintf(stderr, "i_display_height = false\n");
      priv->i_display_[0] = EINA_FALSE;
      evas_object_hide(priv->l[0]);
   }
}

static void
_chk_display_teleport(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[1])return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr, "i_display_teleport = true\n");
      priv->i_display_[1] = EINA_TRUE;
      evas_object_show(priv->l[1]);
   } else {
      fprintf(stderr, "i_display_teleport = false\n");
      priv->i_display_[1] = EINA_FALSE;
      evas_object_hide(priv->l[1]);
   }
}

static void
_chk_display_avoid(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[2])return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr, "i_display_avoid = true\n");
      priv->i_display_[2] = EINA_TRUE;
      evas_object_show(priv->l[2]);
   } else {
      fprintf(stderr, "i_display_avoid = false\n");
      priv->i_display_[2] = EINA_FALSE;
      evas_object_hide(priv->l[2]);
   }
}

static void
_chk_display_pathmap(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[3])return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr, "i_display_pathmap = true\n");
      priv->i_display_[3] = EINA_TRUE;
      evas_object_show(priv->l[3]);
   } else {
      fprintf(stderr, "i_display_pathmap = false\n");
      priv->i_display_[3] = EINA_FALSE;
      evas_object_hide(priv->l[3]);
   }
}

static void
_chk_display_spawnmap(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[4])return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr, "i_display_spawnmap = true\n");
      priv->i_display_[4] = EINA_TRUE;
      evas_object_show(priv->l[4]);
   } else {
      fprintf(stderr, "i_display_spawnmap = false\n");
      priv->i_display_[4] = EINA_FALSE;
      evas_object_hide(priv->l[4]);
   }
}

static void
_chk_display_heatmap(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[5])return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr,
         "i_display_heatmap = on\n");
      priv->i_display_[5] = EINA_TRUE;
      evas_object_show(priv->l[5]);
   }
   else
   {
      fprintf(stderr,
         "i_display_heatmap = off\n");
      priv->i_display_[5] = EINA_FALSE;
      evas_object_hide(priv->l[5]);
   }
}

static void
_chk_display_search(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[6])return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr,
         "i_display_search = on\n");
      priv->i_display_[6] = EINA_TRUE;
      evas_object_show(priv->l[6]);
   }
   else 
   {
      fprintf(stderr,
         "i_display_search = off\n");
      priv->i_display_[6] = EINA_FALSE;
      evas_object_hide(priv->l[6]);
   }
}
static void
_chk_display_path(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[7])return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr,
         "i_display_path = on\n");
      priv->i_display_[7] = EINA_TRUE;
      evas_object_show(priv->l[7]);
   }
   else 
   {
      fprintf(stderr,
         "i_display_path = off\n");
      priv->i_display_[7] = EINA_FALSE;
      evas_object_hide(priv->l[7]);
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
   Evas_Object *world, *vbox1, *lab, *chk, *hbox, *frm, *spin;

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

    /* button divider */
   vbox1 = elm_box_add(win);
   evas_object_size_hint_weight_set(vbox1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(vbox1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(hbox, vbox1);
   evas_object_show(vbox1);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(chk, EVAS_HINT_EXPAND, 0.0);
   elm_check_state_set(chk, I_DISPLAY_HEIGHT_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Spawnmap");
   elm_box_pack_end(vbox1, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_spawnmap, world);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, I_DISPLAY_HEATMAP_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Pathmap");
   elm_box_pack_end(vbox1, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_pathmap, world);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(chk, EVAS_HINT_EXPAND, 0.0);
   elm_check_state_set(chk, I_DISPLAY_SEARCH_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Avoidmap");
   elm_box_pack_end(vbox1, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_avoid, world);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, I_DISPLAY_PATH_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Teleport");
   elm_box_pack_end(vbox1, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_teleport, world);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, I_DISPLAY_PATH_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Heightmap");
   elm_box_pack_end(vbox1, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_height, world);

    /* button divider */
   vbox1 = elm_box_add(win);
   evas_object_size_hint_weight_set(vbox1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(vbox1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(hbox, vbox1);
   evas_object_show(vbox1);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, I_DISPLAY_PATH_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Paths");
   elm_box_pack_end(vbox1, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_path, world);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, I_DISPLAY_PATH_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Path Search");
   elm_box_pack_end(vbox1, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_search, world);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, I_DISPLAY_PATH_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Show Heatmap");
   elm_box_pack_end(vbox1, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_display_heatmap, world);

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

#ifndef I_PATH_H
#define I_PATH_H

static void
_chk_path_search_diagonal(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->height)return;

   if(elm_check_state_get(o))
   {
      fprintf(stderr,
         "i_path_search_diagonal = on\n");
      priv->i_path_search_diagonal = EINA_TRUE;
   }
   else
   {
      fprintf(stderr,
         "i_path_search_diagonal = off\n");
      priv->i_path_search_diagonal = EINA_FALSE;
   }
}

static void
_hoversel_path_distance_start_manhat(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
}

static void
_hoversel_path_distance_start_diagon(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
}

static void
_hoversel_path_distance_start_euclid(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
}

static void
_spin_path_distance_start_mult(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_distance_start_mult = (float)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_distance_start_mult = %f\n",
      priv->i_path_distance_start_mult);
}

static void
_hoversel_path_distance_goal_manhat(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
}

static void
_hoversel_path_distance_goal_diagon(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
}

static void
_hoversel_path_distance_goal_euclid(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
}

static void
_spin_path_distance_goal_mult(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_distance_goal_mult = (float)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_distance_goal_mult = %f\n",
      priv->i_path_distance_goal_mult);
}

static void
_spin_path_tread_weight(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_tread_weight = (float)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_tread_weight = %f\n",
      priv->i_path_tread_weight);
}

static void
_spin_path_climb_up_min(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_climb_up_min = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_climb_up_min = %i\n",
      priv->i_path_climb_up_min);
}

static void
_spin_path_climb_up_max(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_climb_up_max = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_climb_up_max = %i\n",
      priv->i_path_climb_up_max);
}

static void
_spin_path_climb_down_min(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_climb_down_min = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_climb_down_min = %i\n",
      priv->i_path_climb_down_min);
}

static void
_spin_path_climb_down_max(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_climb_down_max = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_climb_down_max = %i\n",
      priv->i_path_climb_down_max);
}

static void
i_path_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *vbox1, *hbox, *frm, *hov, *chk, *lab, *spin, *slid;

   world = evas_object_name_find(evas_object_evas_get(win),"world");

   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "Travel Options");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox);
   evas_object_show(vbox);

   chk = elm_check_add(win);
   evas_object_size_hint_align_set(chk, 0.0, EVAS_HINT_FILL);
   elm_check_state_set(chk, I_PATH_SEARCH_DIAGONAL_DEFAULT);
   elm_object_part_text_set(chk, NULL,  "Can Move Diagonally");
   elm_box_pack_end(vbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_path_search_diagonal, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   hov = elm_hoversel_add(win);
   elm_hoversel_hover_parent_set(hov, win);
   evas_object_size_hint_align_set(hov, EVAS_HINT_FILL, 0);
   evas_object_size_hint_weight_set(hov, EVAS_HINT_EXPAND, 0);
   elm_object_text_set(hov, "Distance from Start");
   elm_hoversel_item_add(hov, "Manhatten", NULL, ELM_ICON_NONE, _hoversel_path_distance_start_manhat, world);
   elm_hoversel_item_add(hov, "Diagonal", NULL, ELM_ICON_NONE, _hoversel_path_distance_start_diagon, world);
   elm_hoversel_item_add(hov, "Euclidean", NULL, ELM_ICON_NONE, _hoversel_path_distance_start_euclid, world);
   elm_box_pack_end(hbox, hov);
   evas_object_show(hov);

  spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%0.3f");
   elm_spinner_min_max_set(spin, 0.0, 10.0);
   elm_spinner_step_set(spin, 0.001);
   elm_spinner_value_set(spin, I_PATH_DISTANCE_START_MULT_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_distance_start_mult, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   hov = elm_hoversel_add(win);
   elm_hoversel_hover_parent_set(hov, win);
   evas_object_size_hint_align_set(hov, EVAS_HINT_FILL, 0);
   evas_object_size_hint_weight_set(hov, EVAS_HINT_EXPAND, 0);
   elm_object_text_set(hov, "Distance to Goal");
   elm_hoversel_item_add(hov, "Manhatten", NULL, ELM_ICON_NONE, _hoversel_path_distance_goal_manhat, world);
   elm_hoversel_item_add(hov, "Diagonal", NULL, ELM_ICON_NONE, _hoversel_path_distance_goal_diagon, world);
   elm_hoversel_item_add(hov, "Euclidean", NULL, ELM_ICON_NONE, _hoversel_path_distance_goal_euclid, world);
   elm_box_pack_end(hbox, hov);
   evas_object_show(hov);

  spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%0.3f");
   elm_spinner_min_max_set(spin, 0.0, 10.0);
   elm_spinner_step_set(spin, 0.001);
   elm_spinner_value_set(spin, I_PATH_DISTANCE_GOAL_MULT_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_distance_goal_mult, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* speed */
   lab = elm_label_add(win);
   elm_object_text_set(lab, "Tread Weight");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%0.3f");
   elm_spinner_min_max_set(spin, 0.0, 1.0);
   elm_spinner_step_set(spin, 0.003);
   elm_spinner_value_set(spin, I_PATH_TREAD_WEIGHT_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_tread_weight, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* speed */
   lab = elm_label_add(win);
   elm_object_text_set(lab, "Climb Up");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "min: %0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_CLIMB_UP_MIN_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_climb_up_min, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "max: %0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_CLIMB_UP_MAX_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_climb_up_max, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* speed */
   lab = elm_label_add(win);
   elm_object_text_set(lab, "Climb Down");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "min: %0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_CLIMB_DOWN_MIN_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_climb_down_min, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "max: %0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_CLIMB_DOWN_MAX_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_climb_down_max, world);
}

#endif /*I_PATH_H*/

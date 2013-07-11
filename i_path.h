#ifndef I_PATH_H
#define I_PATH_H

static void
_chk_path_search_diagonal(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *world;
   
   world = (Evas_Object *)data;
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[0])return;

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
_spin_path_tread_weight(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_tread_weight = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_tread_weight = %i\n",
      priv->i_path_tread_weight);
}

static void
_spin_path_climb_up_value(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_climb_up_value = (float)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_climb_up_value = %f\n",
      priv->i_path_climb_up_value);
}

static void
_spin_path_climb_up_tolerance(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_climb_up_tolerance = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_climb_up_tolerance = %i\n",
      priv->i_path_climb_up_tolerance);
}

static void
_spin_path_climb_up_limit(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_climb_up_limit = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_climb_up_limit = %i\n",
      priv->i_path_climb_up_limit);
}

static void
_spin_path_climb_down_value(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_climb_down_value = (float)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_climb_down_value = %f\n",
      priv->i_path_climb_down_value);
}

static void
_spin_path_climb_down_tolerance(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_climb_down_tolerance = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_climb_down_tolerance = %i\n",
      priv->i_path_climb_down_tolerance);
}

static void
_spin_path_climb_down_limit(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_climb_down_limit = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_climb_down_limit = %i\n",
      priv->i_path_climb_down_limit);
}

static void
_spin_path_avoid_value(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_avoid_value = (float)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_avoid_value = %f\n",
      priv->i_path_avoid_value);
}

static void
_spin_path_avoid_tolerance(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_avoid_tolerance = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_avoid_tolerance = %i\n",
      priv->i_path_avoid_tolerance);
}

static void
_spin_path_avoid_limit(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_avoid_limit = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_avoid_limit = %i\n",
      priv->i_path_avoid_limit);
}

static void
_spin_path_map_value(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_map_value = (float)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_map_value = %f\n",
      priv->i_path_map_value);
}

static void
_spin_path_map_tolerance(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_map_tolerance = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_map_tolerance = %i\n",
      priv->i_path_map_tolerance);
}

static void
_spin_path_map_limit(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_map_limit = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_map_limit = %i\n",
      priv->i_path_map_limit);
}

static void
_spin_path_heat_value(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_heat_value = (float)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_heat_value = %f\n",
      priv->i_path_heat_value);
}

static void
_spin_path_heat_tolerance(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_heat_tolerance = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_heat_tolerance = %i\n",
      priv->i_path_heat_tolerance);
}

static void
_spin_path_heat_limit(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_heat_limit = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_heat_limit = %i\n",
      priv->i_path_heat_limit);
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


   /* tread Strength */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   lab = elm_label_add(win);
   elm_object_text_set(lab, "Tread Weight");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%0.0f");
   elm_spinner_min_max_set(spin, 0.0, 255.0);
   elm_spinner_step_set(spin, 1.0);
   elm_spinner_value_set(spin, I_PATH_TREAD_WEIGHT_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_tread_weight, world);

   /* Climbing Up */
   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "Climbing Up");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   vbox1 = elm_box_add(win);
   evas_object_size_hint_align_set(vbox1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox1);
   evas_object_show(vbox);

   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox1, hbox);
   evas_object_show(hbox);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "val:%0.3f");
   elm_spinner_min_max_set(spin, 0, 10);
   elm_spinner_step_set(spin, .01);
   elm_spinner_value_set(spin, I_PATH_CLIMB_UP_VALUE_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_climb_up_value, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "tol:%0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_CLIMB_UP_TOLERANCE_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_climb_up_tolerance, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "lim:%0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_CLIMB_UP_LIMIT_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_climb_up_limit, world);

   /* Climbing Down */
   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "Climbing Down");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   vbox1 = elm_box_add(win);
   evas_object_size_hint_align_set(vbox1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox1);
   evas_object_show(vbox);

   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox1, hbox);
   evas_object_show(hbox);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "val:%0.3f");
   elm_spinner_min_max_set(spin, 0, 10);
   elm_spinner_step_set(spin, .01);
   elm_spinner_value_set(spin, I_PATH_CLIMB_DOWN_VALUE_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_climb_down_value, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "tol:%0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_CLIMB_DOWN_TOLERANCE_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_climb_down_tolerance, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "lim:%0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_CLIMB_DOWN_LIMIT_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_climb_down_limit, world);

   /* Area Avoidance */
   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "Avoidance Map");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   vbox1 = elm_box_add(win);
   evas_object_size_hint_align_set(vbox1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox1);
   evas_object_show(vbox);

   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox1, hbox);
   evas_object_show(hbox);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "val:%0.3f");
   elm_spinner_min_max_set(spin, 0, 10);
   elm_spinner_step_set(spin, .01);
   elm_spinner_value_set(spin, I_PATH_AVOID_VALUE_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_avoid_value, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "tol:%0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_AVOID_TOLERANCE_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_avoid_tolerance, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "lim:%0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_AVOID_LIMIT_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_avoid_limit, world);

   /* Map Following */
   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "Map Follow");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   vbox1 = elm_box_add(win);
   evas_object_size_hint_align_set(vbox1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox1);
   evas_object_show(vbox);

   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox1, hbox);
   evas_object_show(hbox);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "val:%0.3f");
   elm_spinner_min_max_set(spin, 0, 10);
   elm_spinner_step_set(spin, .01);
   elm_spinner_value_set(spin, I_PATH_MAP_VALUE_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_map_value, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "tol:%0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_MAP_TOLERANCE_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_map_tolerance, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "lim:%0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_MAP_LIMIT_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_map_limit, world);

   /* Heat Following */
   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "Heat Follow");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   vbox1 = elm_box_add(win);
   evas_object_size_hint_align_set(vbox1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox1);
   evas_object_show(vbox);

   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox1, hbox);
   evas_object_show(hbox);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "val:%0.3f");
   elm_spinner_min_max_set(spin, 0, 10);
   elm_spinner_step_set(spin, .01);
   elm_spinner_value_set(spin, I_PATH_HEAT_VALUE_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_heat_value, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "tol:%0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_HEAT_TOLERANCE_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_heat_tolerance, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "lim:%0.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_HEAT_LIMIT_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_heat_limit, world);


}

#endif /*I_PATH_H*/

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
_spin_path_speed_change(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_search_iter_speed = (float)elm_spinner_value_get(o);
   fprintf(stderr,
      "path search iterator speed = %f\n",
      (float)elm_spinner_value_get(o));
}

static void
_spin_path_search_iter_max(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_search_iter_max = (int)elm_spinner_value_get(o);
   fprintf(stderr,
      "path search iterations = %i\n",
      (int)elm_spinner_value_get(o));
}

static void
_slid_path_inf_dist_manhat(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_inf_dist_manhat = (float)elm_slider_value_get(o);
   fprintf(stderr,
      "i_path_inf_dist_manhat = %f\n",
      (float)elm_slider_value_get(o));
}

static void
_slid_path_inf_dist_euclid(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_inf_dist_euclid = (float)elm_slider_value_get(o);
   fprintf(stderr,
      "i_path_inf_dist_euclid = %f\n",
      (float)elm_slider_value_get(o));
}

static void
_slid_path_inf_desasc(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_inf_desasc = (float)elm_slider_value_get(o);
   fprintf(stderr,
      "i_path_inf_descasc = %f\n",
      (float)elm_slider_value_get(o));
}

static void
_slid_path_inf_path(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_inf_path = (float)elm_slider_value_get(o);
   fprintf(stderr,
      "i_path_inf_path = %f\n",
      (float)elm_slider_value_get(o));
}

static void
_slid_path_walk_strength(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_walk_strength = (int)elm_slider_value_get(o);
   fprintf(stderr,
      "i_path_walk_strength = %i\n",
      (int)elm_slider_value_get(o));
}

static void 
_hoversel_path_algorithm_dijkstra(void *data, Evas_Object *o, void *event_info)
{
   PATHGEN_WORLD_DATA_GET(data, priv);
  priv->hueristic = hueristic_dijkstra;
   fprintf(stderr, "dijkstra Selected\n");
}

static void 
_hoversel_path_algorithm_custom(void *data, Evas_Object *o, void *event_info)
{
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->hueristic = hueristic_custom;
   fprintf(stderr, "dijkstra Selected\n");
}

static void
i_path_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *frm, *lab, *spin, *hbox, *slid, *chk, *hov;

   world = evas_object_name_find(evas_object_evas_get(win),"world");

   hov = elm_hoversel_add(win);
   elm_hoversel_hover_parent_set(hov, win);
   evas_object_size_hint_align_set(hov, EVAS_HINT_FILL, 0);
   evas_object_size_hint_weight_set(hov, EVAS_HINT_EXPAND, 0);
   elm_object_text_set(hov, "Choose Path Finding Algorithm");
   elm_hoversel_item_add(hov, "Dijkstra", NULL, ELM_ICON_NONE, _hoversel_path_algorithm_dijkstra, world);
   elm_hoversel_item_add(hov, "Custom", NULL, ELM_ICON_NONE, _hoversel_path_algorithm_custom, world);
   elm_hoversel_item_add(hov, "A*", NULL, ELM_ICON_NONE, NULL, NULL);
   elm_hoversel_item_add(hov, "Jump Point", NULL, ELM_ICON_NONE, NULL, NULL);
   elm_box_pack_end(vbox, hov);
   evas_object_show(hov);

   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "Path Options");
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
   elm_object_part_text_set(chk, NULL,  "Allow Diagonal Movement");
   elm_box_pack_end(vbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_path_search_diagonal, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* speed of sim */
   lab = elm_label_add(win);
   evas_object_size_hint_weight_set(frm, 0.5, 0.0);
   evas_object_size_hint_align_set(lab, 0.5, 0.5);
   elm_object_text_set(lab, "Search Iteration Speed");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%0.3f");
   elm_spinner_min_max_set(spin, 0.001, 5.0);
   elm_spinner_step_set(spin, 0.001);
   elm_spinner_value_set(spin, I_PATH_SEARCH_ITER_SPEED_DEFAULT);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_speed_change, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* max iterations before quit */
   lab = elm_label_add(win);
//   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(lab, "Max Search Iterations");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%.0f iterations");
   elm_spinner_min_max_set(spin, 1, INT_MAX);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_PATH_SEARCH_ITER_MAX_DEFAULT);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_search_iter_max, world);

   slid = elm_slider_add(win);
   evas_object_size_hint_align_set(slid, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(slid, "Walk Strength");
   elm_slider_indicator_format_set(slid, "%.0f");
   elm_slider_min_max_set(slid, 1, 254);
   elm_slider_value_set(slid, I_PATH_WALK_STRENGTH_DEFAULT);
   elm_box_pack_end(vbox, slid);
   evas_object_show(slid);

   evas_object_smart_callback_add(slid, "delay,changed",
      _slid_path_walk_strength, world);

   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "Pathing Influences");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox);
   evas_object_show(vbox);

   slid = elm_slider_add(win);
   evas_object_size_hint_align_set(slid, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(slid, "Manhattan Distance");
   elm_slider_indicator_format_set(slid, "%1.3f");
   elm_slider_min_max_set(slid, 0.0, 1.0);
   elm_slider_value_set(slid, I_PATH_INF_DIST_MANHAT_DEFAULT);
   elm_box_pack_end(vbox, slid);
   evas_object_show(slid);

   evas_object_smart_callback_add(slid, "delay,changed",
      _slid_path_inf_dist_manhat, world);

   slid = elm_slider_add(win);
   evas_object_size_hint_align_set(slid, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(slid, "Euclidean Distance");
   elm_slider_indicator_format_set(slid, "%1.3f");
   elm_slider_min_max_set(slid, 0.0, 1.0);
   elm_slider_value_set(slid, I_PATH_INF_DIST_EUCLID_DEFAULT);
   elm_box_pack_end(vbox, slid);
   evas_object_show(slid);

   evas_object_smart_callback_add(slid, "delay,changed",
      _slid_path_inf_dist_euclid, world);

   slid = elm_slider_add(win);
   evas_object_size_hint_align_set(slid, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(slid, "Descent/Ascent");
   elm_slider_indicator_format_set(slid, "%1.3f");
   elm_slider_min_max_set(slid, 0.0, 1.0);
   elm_slider_value_set(slid, I_PATH_INF_DESASC_DEFAULT);
   elm_box_pack_end(vbox, slid);
   evas_object_show(slid);

   evas_object_smart_callback_add(slid, "delay,changed",
      _slid_path_inf_desasc, world);

   slid = elm_slider_add(win);
   evas_object_size_hint_align_set(slid, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(slid, "Path");
   elm_slider_indicator_format_set(slid, "%1.3f");
   elm_slider_min_max_set(slid, 0.0, 1.0);
   elm_slider_value_set(slid, I_PATH_INF_PATH_DEFAULT);
   elm_box_pack_end(vbox, slid);
   evas_object_show(slid);

   evas_object_smart_callback_add(slid, "delay,changed",
      _slid_path_inf_path, world);

}

#endif /*I_PATH_H*/

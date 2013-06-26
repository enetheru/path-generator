#ifndef I_PATH_H
#define I_PATH_H

static void
_spin_path_speed_change(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_search_iter_speed = (float)elm_spinner_value_get(o);
}

static void
_spin_path_search_iter_max(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_search_iter_max = (int)elm_spinner_value_get(o);
}

static void
i_path_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *lab, *spin, *hbox;

   world = evas_object_name_find(evas_object_evas_get(win),"world");

   lab = elm_label_add(win);
   elm_object_text_set(lab, "Path Options");
   elm_box_pack_end(vbox, lab);
   evas_object_show(lab);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* speed of sim */
   lab = elm_label_add(win);
//   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(lab, "Search Iteration Speed");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%1.6f Seconds");
   elm_spinner_min_max_set(spin, 0.000001, 5.0);
   elm_spinner_step_set(spin, 0.000001);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_speed_change, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
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
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_search_iter_max, world);

}

#endif /*I_PATH_H*/

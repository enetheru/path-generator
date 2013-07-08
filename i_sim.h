#ifndef I_SIM_H
#define I_SIM_H

#include "r_image.h"

static void
_spin_sim_path_max(void *data, Evas_Object *o, void *event_info) {
   if(!data)return;   
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_sim_path_max = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_sim_path_max = %i\n", priv->i_sim_path_max);

}

static void
_spin_sim_search_iter_max(void *data, Evas_Object *o, void *event_info) {
   if(!data)return;   
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_sim_search_iter_max = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_sim_search_iter_max = %i\n", priv->i_sim_search_iter_max);
}

static void
_spin_sim_path_fade_strength(void *data, Evas_Object *o, void *event_info) {
   if(!data)return;   
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_sim_path_fade_strength = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_sim_path_fade_strength = %i\n", priv->i_sim_path_fade_strength);
}

static void
_spin_sim_path_fade_interval(void *data, Evas_Object *o, void *event_info) {
   if(!data)return;   
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_sim_path_fade_interval = (int)elm_spinner_value_get(o);
   fprintf(stderr, "i_sim_path_fade_interval = %i\n", priv->i_sim_path_fade_interval);
}


static void
i_sim_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *lab, *btn, *hbox, *spin, *frm;

   world = evas_object_name_find(evas_object_evas_get(win),"world");

   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "Simulation Options");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_homogeneous_set(vbox, EINA_TRUE);
   elm_object_content_set(frm, vbox);
   evas_object_show(vbox);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* number of paths to cast */
   lab = elm_label_add(win);
   elm_object_text_set(lab, "Number of Paths");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%.0f");
   elm_spinner_min_max_set(spin, 1, INT_MAX);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_SIM_PATHS_MAX_DEFAULT);

   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_sim_path_max, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* max iterations before quit */
   lab = elm_label_add(win);
   elm_object_text_set(lab, "Search Iteration Limit");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%.0f");
   elm_spinner_min_max_set(spin, 1, INT_MAX);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_SIM_SEARCH_ITER_MAX_DEFAULT);

   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_sim_search_iter_max, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* max iterations before quit */
   lab = elm_label_add(win);
   elm_object_text_set(lab, "Path Fade Strength");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%.0f");
   elm_spinner_min_max_set(spin, 0, 255);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_SIM_PATH_FADE_STRENGTH_DEFAULT);

   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_sim_path_fade_strength, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* max iterations before quit */
   lab = elm_label_add(win);
   elm_object_text_set(lab, "Path Fade Interval");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%.0f");
   elm_spinner_min_max_set(spin, 0, INT_MAX);
   elm_spinner_step_set(spin, 1);
   elm_spinner_value_set(spin, I_SIM_PATH_FADE_INTERVAL_DEFAULT);

   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_sim_path_fade_interval, world);
}

#endif /*I_SIM_H*/

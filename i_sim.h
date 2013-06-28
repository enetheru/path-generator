#ifndef I_SIM_H
#define I_SIM_H

static void
_btn_sim_start(void *data, Evas_Object *o, void *event_info)
{
   fprintf(stderr, "want to start sim\n");
   evas_object_smart_callback_call( data, "sim,start", event_info);
   return;
}

static void
_btn_sim_stop(void *data, Evas_Object *o, void *event_info)
{
   fprintf(stderr, "want to stop sim\n");
   return;
}

static void
_btn_sim_reset(void *data, Evas_Object *o, void *event_info)
{
   fprintf(stderr, "want to reset sim\n");
   return;
}

static void
_spin_sim_travelers(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_sim_travelers = (int)elm_spinner_value_get(o);
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

   /* add button to start sim */
   btn = elm_button_add(win);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Start Sim");
   elm_box_pack_end(vbox, btn);
   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_sim_start, world);

   /* add button to Stop sim */
   btn = elm_button_add(win);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Stop Sim");
//   elm_box_pack_end(vbox, btn);
//   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_sim_stop, world);

   /* add button to reset sim */
   btn = elm_button_add(win);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Reset Sim");
//   elm_box_pack_end(vbox, btn);
//   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_sim_reset, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* number of paths to cast */
   lab = elm_label_add(win);
   elm_object_text_set(lab, "number of travelers");
   elm_box_pack_end(hbox, lab);
   evas_object_show(lab);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%.0f travelers");
   elm_spinner_min_max_set(spin, 1, INT_MAX);
   elm_spinner_step_set(spin, 1);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   elm_spinner_value_set(spin, I_SIM_TRAVELERS_DEFAULT);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_sim_travelers, world);
}

#endif /*I_SIM_H*/

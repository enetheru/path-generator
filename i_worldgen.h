#ifndef I_WORLDGEN_H
#define I_WORLDGEN_H

#include "r_image.h"
#include "r_noise.h"

static void
_btn_worldgen_generate(void *data, Evas_Object *o, void *event_info)
{
   evas_object_smart_callback_call(data, "world,generate", event_info);
}

static void
_spin_worldgen_w(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_worldgen_w = (int)elm_spinner_value_get(o);
}

static void
_spin_worldgen_h(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_worldgen_h = (int)elm_spinner_value_get(o);
}

static void
_spin_worldgen_density(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;

   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_worldgen_density = (int)elm_spinner_value_get(o);
}

static void
i_worldgen_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *frm, *lab, *spin, *btn, *fs_entry, *hbox,
      *vbox1, *vbox2;

   world = evas_object_name_find(evas_object_evas_get(win),"world");

   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "World Generation Options");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox);
   evas_object_show(vbox);

   /* add button to reset sim */
   btn = elm_button_add(win);
   evas_object_name_set(btn, "world,generate");
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Generate Random World");
   elm_box_pack_end(vbox, btn);
   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_worldgen_generate, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "w=%.0f");
   elm_spinner_min_max_set(spin, I_WORLDGEN_W_MIN, I_WORLDGEN_W_MAX);
   elm_spinner_value_set(spin, I_WORLDGEN_W_DEFAULT);
   elm_spinner_step_set(spin, I_WORLDGEN_W_STEP);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_worldgen_w, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "h=%.0f");
   elm_spinner_min_max_set(spin, I_WORLDGEN_H_MIN, I_WORLDGEN_H_MAX);
   elm_spinner_value_set(spin, I_WORLDGEN_H_DEFAULT);
   elm_spinner_step_set(spin, I_WORLDGEN_H_STEP);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_worldgen_h, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "Density=%.0f");
   elm_spinner_min_max_set(spin, I_WORLDGEN_DENSITY_MIN,
      I_WORLDGEN_DENSITY_MAX);
   elm_spinner_value_set(spin, I_WORLDGEN_DENSITY_DEFAULT);
   elm_spinner_step_set(spin, I_WORLDGEN_DENSITY_STEP);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   elm_box_pack_end(vbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_worldgen_density, world);
}

#endif /*I_WORLDGEN_H*/

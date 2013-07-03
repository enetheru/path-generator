#ifndef I_WORLD_H
#define I_WORLD_H

#include "image.h"
#include "noise.h"

static void 
_fs_load_height(void *data, Evas_Object *obj, void *event_info)
{
   const char *file = event_info;
   Evas *evas;
   Evas_Object *image;

   if (file) printf("File chosen: %s\n", file);
   else
   {
      printf("File selection canceled.\n");
      return;
   }

   evas = evas_object_evas_get(obj);

   image = evas_object_image_filled_add(evas);
   evas_object_image_file_set(image, file, NULL);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);

   pathgen_world_height_set(data, image);
}

static void
_btn_generate(void *data, Evas_Object *o, void *event_info)
{
   Evas_Object *image;
   Evas *evas;

   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   evas = evas_object_evas_get(o);

   image = image_generate_random(evas,
      priv->i_world_gen_w, priv->i_world_gen_h);
   image_paint_noise(image, priv->i_world_gen_density);
   pathgen_world_height_set(data, image);
}

static void
_spin_world_gen_size_w(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;

   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_world_gen_w = (int)elm_spinner_value_get(o);
}

static void
_spin_world_gen_size_h(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;

   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_world_gen_h = (int)elm_spinner_value_get(o);
}

static void
_spin_world_gen_density(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;

   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_world_gen_density = (int)elm_spinner_value_get(o);
}


static void
i_world_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *frm, *lab, *spin, *btn, *fs_entry, *hbox;

   world = evas_object_name_find(evas_object_evas_get(win),"world");

   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "World Options");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox);
   evas_object_show(vbox);

   /* file selector entry */
   fs_entry = elm_fileselector_entry_add(win);
   evas_object_name_set(fs_entry, "world,height,load");
   evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, 0.0);
   elm_fileselector_entry_path_set(fs_entry, "select height map");
   elm_fileselector_entry_expandable_set(fs_entry, EINA_FALSE);
   elm_object_text_set(fs_entry, "Select height map");
   elm_box_pack_end(vbox, fs_entry);
   evas_object_show(fs_entry);

   evas_object_smart_callback_add(
      fs_entry, "file,chosen", _fs_load_height, world);

  frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "World Generation");
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

   evas_object_smart_callback_add(btn, "clicked", _btn_generate, world);

   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "World Dimensions");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox);
   evas_object_show(vbox);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "w=%.0f");
   elm_spinner_min_max_set(spin, 128, 2048);
   elm_spinner_step_set(spin, 64);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_world_gen_size_w, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "h=%.0f");
   elm_spinner_min_max_set(spin, 128, 2048);
   elm_spinner_step_set(spin, 64);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_world_gen_size_h, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "density=%.0f");
   elm_spinner_min_max_set(spin, 1, 2048);
   elm_spinner_step_set(spin, 1);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_world_gen_density, world);


}

#endif /*I_WORLD_H*/

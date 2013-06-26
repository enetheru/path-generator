#ifndef I_WORLD_H
#define I_WORLD_H

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
   fprintf(stderr, "want to generate world\n");
   Evas_Object *image;
   Evas *evas;

   evas = evas_object_evas_get(o);

   image = image_generate_random(evas, 100, 100);
   pathgen_world_height_set(data, image);
}

static void
i_world_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *frm, *lab, *btn, *fs_entry, *hbox;

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
   elm_box_homogeneous_set(vbox, EINA_TRUE);
   elm_object_content_set(frm, vbox);
   evas_object_show(vbox);

   /* file selector entry */
   fs_entry = elm_fileselector_entry_add(win);
   evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, 0.0);
   elm_fileselector_entry_path_set(fs_entry, "select height map");
   elm_fileselector_entry_expandable_set(fs_entry, EINA_FALSE);
   elm_object_text_set(fs_entry, "Select height map");
   elm_box_pack_end(vbox, fs_entry);
   evas_object_show(fs_entry);

   evas_object_smart_callback_add(
      fs_entry, "file,chosen", _fs_load_height, world);

   /* == Begin WorldGen Options == */
   lab = elm_label_add(win);
   elm_object_text_set(lab, "World Generation Options");
   elm_box_pack_end(vbox, lab);
   evas_object_show(lab);

   /* add button to reset sim */
   btn = elm_button_add(win);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Generate Random World");
   elm_box_pack_end(vbox, btn);
   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_generate, world);

}

#endif /*I_WORLD_H*/

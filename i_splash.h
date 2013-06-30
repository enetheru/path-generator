#ifndef I_SPLASH_H
#define I_SPLASH_H

#define HELP_TEXT "Path-Generator by Samuel Nicholas" \

static void
i_splash_setup(Evas_Object *win)
{
   Evas_Object *vbox, *vboxl, *vboxr, *hbox, *inwin, *lab, *sep;

   inwin = elm_win_inwin_add(win);
   evas_object_name_set(inwin, "splash");
   evas_object_show(inwin);

   vbox = elm_box_add(win);
   evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_win_inwin_content_set(inwin, vbox);
   evas_object_show(vbox);

   lab = elm_label_add(win);
   elm_object_text_set(lab, HELP_TEXT);
   evas_object_show(lab);
   elm_box_pack_end(vbox, lab);

   sep = elm_separator_add(win);
   elm_separator_horizontal_set(sep, EINA_TRUE);
   elm_box_pack_end(vbox, sep);
   evas_object_show(sep);

   /* keybindings */
   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   vboxl = elm_box_add(win);
   elm_box_pack_end(hbox, vboxl);
   evas_object_show(vboxl);

   vboxr = elm_box_add(win);
   elm_box_pack_end(hbox, vboxr);
   evas_object_show(vboxr);

   lab = elm_label_add(win);
   elm_object_text_set(lab, "ESC");
   evas_object_size_hint_align_set(lab, 1.0, 0.0);
   elm_box_pack_end(vboxl, lab);
   evas_object_show(lab);

   lab = elm_label_add(win);
   elm_object_text_set(lab, "Toggle this window");
   evas_object_size_hint_align_set(lab, 0.0, 0.0);
   elm_box_pack_end(vboxr, lab);
   evas_object_show(lab);

   lab = elm_label_add(win);
   elm_object_text_set(lab, "CTRL + Mouse Wheel");
   evas_object_size_hint_align_set(lab, 1.0, 0.0);
   elm_box_pack_end(vboxl, lab);
   evas_object_show(lab);

   lab = elm_label_add(win);
   elm_object_text_set(lab, "Zoom");
   evas_object_size_hint_align_set(lab, 0.0, 0.0);
   elm_box_pack_end(vboxr, lab);
   evas_object_show(lab);

   lab = elm_label_add(win);
   elm_object_text_set(lab, "CTRL + ALT + Mouse Wheel");
   evas_object_size_hint_align_set(lab, 1.0, 0.0);
   elm_box_pack_end(vboxl, lab);
   evas_object_show(lab);

   lab = elm_label_add(win);
   elm_object_text_set(lab, "Fine Zoom");
   evas_object_size_hint_align_set(lab, 0.0, 0.0);
   elm_box_pack_end(vboxr, lab);
   evas_object_show(lab);


}

#endif /*I_SPLASH_H*/

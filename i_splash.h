#ifndef I_SPLASH_H
#define I_SPLASH_H

#define HELP_TEXT "Path-Generator by Samuel Nicholas<br><br>" \
   "ESC  -  Toggle this window<br>" \
   "CTRL+Mouse Wheel  -  Zoom<br>" \
   "CTRL+ALT+Mouse Wheel  -  Fine Zoom"

static void
i_splash_setup(Evas_Object *win)
{
   Evas_Object *vbox, *hbox, *inwin, *lab;

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
}

#endif /*I_SPLASH_H*/

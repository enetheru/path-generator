/* Primary button Divider */
vbox1 = elm_box_add(win);
evas_object_size_hint_weight_set(vbox1, EVAS_HINT_EXPAND, 0);
elm_object_content_set(scroller, vbox1);
evas_object_show(vbox1);

for(i=0; i<5; i++)
{
   /* display image load */
   button = elm_fileselector_button_add(win);
   evas_object_name_set(button, ui_labels[i*2]);
   elm_fileselector_button_expandable_set(button, EINA_FALSE);
   elm_object_text_set(button, ui_labels[i*2+1]);
   evas_object_size_hint_align_set(button, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox1, button);
   evas_object_show(button);

   evas_object_smart_callback_add(
      button, "file,chosen", _load_image, NULL);
}


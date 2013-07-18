/* Primary button Divider */
vbox1 = elm_box_add(win);
evas_object_size_hint_weight_set(vbox1, EVAS_HINT_EXPAND, 0);
evas_object_size_hint_align_set(vbox1, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_object_content_set(scroller, vbox1);
evas_object_show(vbox1);

/* Start Simulation */
button = elm_button_add(win);
evas_object_name_set(button, "ui,start");
evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_object_text_set(button, "Start Sim");
elm_box_pack_end(vbox1, button);
evas_object_show(button);

evas_object_smart_callback_add(
   button, "clicked", _ui_sim_start, NULL);

/* Stop Simulation */
button = elm_button_add(win);
evas_object_name_set(button, "ui,stop");
evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_object_text_set(button, "Stop Sim");
elm_box_pack_end(vbox1, button);
evas_object_show(button);

evas_object_smart_callback_add(
   button, "clicked", _ui_sim_stop, NULL);

spinner = elm_spinner_add(win);
evas_object_name_set(spinner, "ui,path_max");
evas_object_size_hint_weight_set(spinner, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
evas_object_size_hint_align_set(spinner, EVAS_HINT_FILL, EVAS_HINT_FILL);
elm_spinner_min_max_set(spinner, 1, INT_MAX);
elm_box_pack_end(vbox1, spinner);
evas_object_show(spinner);



for(i=0; i<5; i++)
{
   hbox = elm_box_add(win);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_pack_end(vbox1, hbox);
   evas_object_show(hbox);

   chk = elm_check_add(win);
   evas_object_name_set(chk, ui_labels[i*3]);
   evas_object_size_hint_align_set(chk, 0, 0.5);
   elm_box_pack_end(hbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _ui_toggle_image, NULL);

   /* display image load */
   button = elm_fileselector_button_add(win);
   evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(button, EVAS_HINT_FILL, 0.5);
   evas_object_name_set(button, ui_labels[i*3+1]);
   elm_fileselector_button_expandable_set(button, EINA_FALSE);
   elm_object_text_set(button, ui_labels[i*3+2]);
   elm_box_pack_end(hbox, button);
   evas_object_show(button);

   evas_object_smart_callback_add(
      button, "file,chosen", _ui_load_image, NULL);
}


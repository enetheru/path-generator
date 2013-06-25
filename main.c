#include <Elementary.h>
#include "pathgen_world.h"
#include "misc.h"
#include "main_callbacks.h"

EAPI_MAIN int

elm_main(int argc, char **argv)
{
   Evas_Object *win, *bg, *vbox, *hbox, *fs_entry, *sep, *btn, *scroll,
      *label, *spinner;

   win = elm_win_add(NULL, "path-generator", ELM_WIN_BASIC);
   elm_win_title_set(win, "path-generator");

   evas_object_smart_callback_add(win, "delete,request", _on_done, NULL);

   /* set background */
   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   /* primary divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, hbox);
   evas_object_show(hbox);

   /* scroll frame for map view */
   scroll = elm_scroller_add(win);
   evas_object_name_set(scroll, "scroller");
   evas_object_size_hint_weight_set(scroll, 0.8, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(scroll, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_start(hbox, scroll);
   evas_object_show(scroll);

   evas_object_event_callback_add(scroll,
      EVAS_CALLBACK_MOUSE_WHEEL, _wheel, NULL);

   /* add our pathgen_world smart object */
   Evas_Object *world;
   world = pathgen_world_add(evas_object_evas_get(win));
   evas_object_name_set(world, "world");
   evas_object_size_hint_min_set(world, 500, 500);
   elm_object_content_set(scroll, world);

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_weight_set(vbox, 0.2, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, 0.0);
   elm_box_homogeneous_set(vbox, EINA_TRUE);
   elm_box_pack_end(hbox, vbox);
   evas_object_show(vbox);

   /* == Begin SIM Options == */
   label = elm_label_add(win);
   elm_object_text_set(label, "Sim Options");
   elm_box_pack_end(vbox, label);
   evas_object_show(label);

   /* add button to start sim */
   btn = elm_button_add(win);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Start Sim");
   elm_box_pack_end(vbox, btn);
   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_sim_start, NULL);

   /* add button to Stop sim */
   btn = elm_button_add(win);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Stop Sim");
//   elm_box_pack_end(vbox, btn);
//   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_sim_stop, NULL);

   /* add button to reset sim */
   btn = elm_button_add(win);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Reset Sim");
//   elm_box_pack_end(vbox, btn);
//   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_sim_reset, NULL);

   /* == Begin World Options == */
   label = elm_label_add(win);
   elm_object_text_set(label, "World Options");
   elm_box_pack_end(vbox, label);
   evas_object_show(label);

   /* file selector entry */
   fs_entry = elm_fileselector_entry_add(win);
   evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, 0.0);
   elm_fileselector_entry_path_set(fs_entry, "select height map");
   elm_fileselector_entry_expandable_set(fs_entry, EINA_FALSE);
   elm_object_text_set(fs_entry, "Select height map");
   elm_box_pack_end(vbox, fs_entry);
   evas_object_show(fs_entry);

   evas_object_smart_callback_add(fs_entry, "file,chosen", _fs_load_height, NULL);

   /* == Begin WorldGen Options == */
   label = elm_label_add(win);
   elm_object_text_set(label, "World Generation Options");
   elm_box_pack_end(vbox, label);
   evas_object_show(label);

   /* add button to reset sim */
   btn = elm_button_add(win);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Generate Random World");
   elm_box_pack_end(vbox, btn);
   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_generate, NULL);

/*********************
* begin path options *
*********************/
   label = elm_label_add(win);
   elm_object_text_set(label, "Path Options");
   elm_box_pack_end(vbox, label);
   evas_object_show(label);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* speed of sim */
   label = elm_label_add(win);
//   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(label, "Search Iteration Speed");
   elm_box_pack_end(hbox, label);
   evas_object_show(label);

   spinner = elm_spinner_add(win);
   elm_spinner_label_format_set(spinner, "%1.6f Seconds");
   elm_spinner_min_max_set(spinner, 0.000001, 5.0);
   elm_spinner_step_set(spinner, 0.000001);
   evas_object_size_hint_align_set(spinner, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spinner, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, spinner);
   evas_object_show(spinner);
   
   evas_object_smart_callback_add(spinner, "delay,changed",
      _spinner_path_speed_change, NULL);

   // now we are done, show the window
   evas_object_resize(win, 800, 600);
   evas_object_show(win);

   elm_run(); // run main loop

   elm_shutdown(); // after mainloop finishes running, shutdown

   return 0; // exit 0 for exit code

}

ELM_MAIN()

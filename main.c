#include <Elementary.h>
#include "pathgen_world.h"
#include "misc.h"
#include "main_callbacks.h"

#define HELP_TEXT "Path-Generator by Samuel Nicholas<br><br>" \
   "ESC  -  Toggle this window<br>" \
   "CTRL+Mouse Wheel  -  Zoom<br>" \
   "CTRL+ALT+Mouse Wheel  -  Fine Zoom"

EAPI_MAIN int

elm_main(int argc, char **argv)
{
   Evas_Object *win, *bg, *vbox, *hbox, *fs_entry, *sep, *btn, *scroll,
      *label, *spinner, *chk, *inwin;

   win = elm_win_add(NULL, "path-generator", ELM_WIN_BASIC);
   elm_win_title_set(win, "path-generator");

   evas_object_smart_callback_add(win, "delete,request", _on_done, NULL);

   /***************
    * HELP WINDOW *
    **************/
   inwin = elm_win_inwin_add(win);
   evas_object_show(inwin);
   evas_object_event_callback_add(win, EVAS_CALLBACK_KEY_DOWN, _key_press, inwin); 

   vbox = elm_box_add(win);
   evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_win_inwin_content_set(inwin, vbox);
   evas_object_show(vbox);

   label = elm_label_add(win);
   elm_object_text_set(label, HELP_TEXT);
   evas_object_show(label);

   elm_box_pack_end(vbox, label);

   /******************
    * MAIN INTERFACE *
    *****************/

   /* set background */
   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_color_set(bg, 250, 250, 249, 255);
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

   /* == Begin Display Options == */
   label = elm_label_add(win);
   elm_object_text_set(label, "Display Options");
   elm_box_pack_end(vbox, label);
   evas_object_show(label);

   chk = elm_check_add(win);
   elm_check_state_set(chk, EINA_TRUE);
   elm_object_part_text_set(chk, NULL,  "Path Heatmap Display");
   elm_box_pack_end(vbox, chk);
   evas_object_show(chk);

   evas_object_smart_callback_add(chk, "changed", _chk_toggle_heatmap, world);

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

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* number of paths to cast */
   label = elm_label_add(win);
   elm_object_text_set(label, "number of travelers");
   elm_box_pack_end(hbox, label);
   evas_object_show(label);

   spinner = elm_spinner_add(win);
   elm_spinner_label_format_set(spinner, "%.0f travelers");
   elm_spinner_min_max_set(spinner, 1, INT_MAX);
   elm_spinner_step_set(spinner, 1);
   evas_object_size_hint_align_set(spinner, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spinner, 0.5, 0.0);
   elm_box_pack_end(hbox, spinner);
   evas_object_show(spinner);
   
   evas_object_smart_callback_add(spinner, "delay,changed",
      _spinner_sim_travelers, NULL);


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

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* max iterations before quit */
   label = elm_label_add(win);
//   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(label, "Max Search Iterations");
   elm_box_pack_end(hbox, label);
   evas_object_show(label);

   spinner = elm_spinner_add(win);
   elm_spinner_label_format_set(spinner, "%.0f iterations");
   elm_spinner_min_max_set(spinner, 1, INT_MAX);
   elm_spinner_step_set(spinner, 1);
   evas_object_size_hint_align_set(spinner, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spinner, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, spinner);
   evas_object_show(spinner);
   
   evas_object_smart_callback_add(spinner, "delay,changed",
      _spinner_path_search_iter_max, NULL);

   /* add button to save heatmap */
   btn = elm_fileselector_button_add(win);
   elm_fileselector_button_window_title_set(btn, "save Path Heatmap");
   elm_fileselector_button_is_save_set(btn, EINA_TRUE);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Save Path Heatmap");
   elm_box_pack_end(vbox, btn);
   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "file,chosen",
      _btn_save_path_heatmap, NULL);

   // now we are done, show the window
   evas_object_resize(win, 800, 600);
   evas_object_show(win);

   elm_win_inwin_activate(inwin);

   elm_run(); // run main loop

   elm_shutdown(); // after mainloop finishes running, shutdown

   return 0; // exit 0 for exit code

}

ELM_MAIN()

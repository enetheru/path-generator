#include <Elementary.h>
#include "pathgen_world.h"
#include "main_callbacks.h"
#include "i_defaults.h"
#include "i_splash.h"
#include "i_display.h"
#include "i_sim.h"
#include "i_world.h"
#include "i_worldgen.h"
#include "i_path.h"
#include "r_noise.h"

EAPI_MAIN int

elm_main(int argc, char **argv)
{
   Evas *evas;
   Evas_Object *win, *bg, *vbox, *hbox, *fs_entry, *sep, *btn, *scroll,
      *label, *spinner, *chk, *inwin, *pbar;


   win = elm_win_add(NULL, "path-generator", ELM_WIN_BASIC);
   elm_win_title_set(win, "path-generator");

   evas_object_smart_callback_add(win, "delete,request", _on_done, NULL);
   evas_object_event_callback_add(win, EVAS_CALLBACK_KEY_DOWN, _key_down, NULL);
   if(!evas_object_key_grab(win, "Escape", 0, 0, EINA_TRUE))fprintf(stderr, "could not grab");;

   evas = evas_object_evas_get(win);

   i_splash_setup(win);

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

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_start(hbox, vbox);
   evas_object_show(vbox);

   /* scroll frame for map view */
   scroll = elm_scroller_add(win);
   evas_object_name_set(scroll, "scroller");
   evas_object_size_hint_weight_set(scroll, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(scroll, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_start(vbox, scroll);
   evas_object_show(scroll);

   evas_object_event_callback_add(scroll,
      EVAS_CALLBACK_MOUSE_WHEEL, _wheel, NULL);

   /* add our pathgen_world smart object */
   Evas_Object *world;
   world = pathgen_world_add(evas_object_evas_get(win));
   evas_object_name_set(world, "world");
   evas_object_size_hint_min_set(world, 500, 500);
   elm_object_content_set(scroll, world);

   /* progress bar for the simulation */
   pbar = elm_progressbar_add(win);
   evas_object_name_set(pbar, "pbar");
   elm_progressbar_unit_format_set(pbar, "%0.2f");
   evas_object_size_hint_weight_set(pbar, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(pbar, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(vbox, pbar);
   evas_object_show(pbar);

/***************
* side buttons *
****************/

   /* scroll frame for buttons view */
   scroll = elm_scroller_add(win);
   elm_scroller_content_min_limit(scroll, EINA_TRUE, EINA_FALSE);
   evas_object_size_hint_weight_set(scroll, 0.0, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(scroll, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(hbox, scroll);
   evas_object_show(scroll);

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_weight_set(vbox, 0.0, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, 0.0);
   elm_object_content_set(scroll, vbox);
   evas_object_show(vbox);

   /* add button to start sim */
   btn = elm_button_add(win);
   evas_object_name_set(btn, "sim,start");
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Start Sim");
   elm_box_pack_end(vbox, btn);
   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_sim_start, world);

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* add button to reset sim */
   btn = elm_button_add(win);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, 0.0);
   elm_object_text_set(btn, "Reset Heatmap");
   elm_box_pack_end(hbox, btn);
   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "clicked", _btn_sim_reset, world);

   /* add button to save heatmap */
   btn = elm_fileselector_button_add(win);
   elm_fileselector_button_window_title_set(btn, "save Path Heatmap");
   elm_fileselector_button_is_save_set(btn, EINA_TRUE);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(btn, "Save Path Heatmap");
   elm_box_pack_end(hbox, btn);
   evas_object_show(btn);

   evas_object_smart_callback_add(btn, "file,chosen",
      _btn_save_path_heatmap, NULL);


   i_display_setup(win, vbox);
   i_sim_setup(win, vbox);
   i_world_setup(win, vbox);
   i_worldgen_setup(win, vbox);
   i_path_setup(win, vbox);

/***********
* the rest *
************/


   // now we are done, show the window
   evas_object_resize(win, 800, 600);
   evas_object_show(win);

   inwin = evas_object_name_find(evas, "splash");
   elm_win_inwin_activate(inwin);

   elm_run(); // run main loop

   elm_shutdown(); // after mainloop finishes running, shutdown

   return 0; // exit 0 for exit code

}

ELM_MAIN()

#include <Elementary.h>
#include "pathgen_world.h"
#include "misc.h"
#include "main_callbacks.h"
#include "i_splash.h"
#include "i_display.h"
#include "i_sim.h"
#include "i_world.h"
#include "i_path.h"


#define HELP_TEXT "Path-Generator by Samuel Nicholas<br><br>" \
   "ESC  -  Toggle this window<br>" \
   "CTRL+Mouse Wheel  -  Zoom<br>" \
   "CTRL+ALT+Mouse Wheel  -  Fine Zoom"

EAPI_MAIN int

elm_main(int argc, char **argv)
{
   Evas *evas;
   Evas_Object *win, *bg, *vbox, *hbox, *fs_entry, *sep, *btn, *scroll,
      *label, *spinner, *chk, *inwin;


   win = elm_win_add(NULL, "path-generator", ELM_WIN_BASIC);
   elm_win_title_set(win, "path-generator");

   evas_object_smart_callback_add(win, "delete,request", _on_done, NULL);
   evas_object_event_callback_add(win, EVAS_CALLBACK_KEY_DOWN, _key_down, NULL);

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

/***************
* side buttons *
****************/

   i_display_setup(win, vbox);
   i_sim_setup(win, vbox);
   i_world_setup(win, vbox);
   i_path_setup(win, vbox);

/***********
* the rest *
************/

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

   inwin = evas_object_name_find(evas, "splash");
   elm_win_inwin_activate(inwin);

   elm_run(); // run main loop

   elm_shutdown(); // after mainloop finishes running, shutdown

   return 0; // exit 0 for exit code

}

ELM_MAIN()

#include <Elementary.h>
#include "pg_data.h"

/* global for my application data */
PG_Data pg_data;

static void 
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   // quit the mainloop (elm_run function will return)
   elm_exit();
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas_Object *win, *hbox, *vbox;
   Evas_Object *bg;

   win = elm_win_add(NULL, "path-generator", ELM_WIN_BASIC);
   elm_win_title_set(win, "path-generator");
   evas_object_resize(win, 800, 600);
   evas_object_show(win);

   evas_object_smart_callback_add(win, "delete,request", _on_done, NULL);

   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_color_set(bg, 255, 255, 255, 255);
   elm_win_resize_object_add(win, bg);
   evas_object_show(bg);

    /* create the interface */
   vbox = elm_box_add(win);
   evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_win_resize_object_add(win, vbox);
   evas_object_show(vbox);

   /* top menu */
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_max_set(bg, -1, 32);
   evas_object_size_hint_min_set(bg, 32, 32);
   evas_object_color_set(bg, 255, 0, 0, 255);
   elm_box_pack_end(vbox, bg);
   evas_object_show(bg);

   hbox = elm_box_add(win);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);
 
   /* map area */
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_min_set(bg, 32, 32);
   evas_object_color_set(bg, 255, 255, 0, 255);
   elm_box_pack_end(hbox, bg);
   evas_object_show(bg);
 
   /* right side buttons panel */
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, 0, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_min_set(bg, 192, 32);
   evas_object_size_hint_max_set(bg, 192, -1);
   evas_object_color_set(bg, 255, 0, 255, 255);
   elm_box_pack_end(hbox, bg);
   evas_object_show(bg);

   /* bottom Status Bar */
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_max_set(bg, -1, 32);
   evas_object_size_hint_min_set(bg, 32, 32);
   evas_object_color_set(bg, 0, 0, 255, 255);
   elm_box_pack_end(vbox, bg);
   evas_object_show(bg);
 

   elm_run(); // run main loop

   elm_shutdown(); // after mainloop finishes running, shutdown

   return 0; // exit 0 for exit code

}

ELM_MAIN()

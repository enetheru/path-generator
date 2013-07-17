#include <Elementary.h>
#include "pg_data.h"
#include "pg_world.h"
#include "pg_display.h"
#include "pg_event.h"


/* globals */
PG_Data pg_data;


static const char* ui_labels[] = {
   "ui,toggle_display", "ui,load_display", "Load Display Image",
   "ui,toggle_height", "ui,load_height", "Load Height Map",
   "ui,toggle_avoid", "ui,load_avoid", "Load Avoidance Map",
   "ui,toggle_map", "ui,load_map", "Load Road Map",
   "ui,toggle_spawn", "ui,load_spawn", "Load Spawn Map",
   "ui,toggle_teleport", "ui,load_teleport", "Load Teleport Map",
};

#include "main_cb.h"

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Evas *evas;
   Evas_Object *win, *hbox, *vbox, *vbox1;
   Evas_Object *scroller;
   Evas_Object *bg, *button, *chk;
   Evas_Object *map;
   int i;

   PG_World *pg_world = pg_world_new();
   pg_data.world = pg_world;

   _event_id_path_more = ecore_event_type_new();
   pg_data.path_more = ecore_event_handler_add(_event_id_path_more, _path_more, NULL);

   _event_id_sim_stop = ecore_event_type_new();
   pg_data.sim_stop = ecore_event_handler_add(_event_id_sim_stop, _sim_stop, NULL);

   win = elm_win_add(NULL, "path-generator", ELM_WIN_BASIC);
   elm_win_title_set(win, "path-generator");
   evas_object_resize(win, 800, 600);
   evas_object_show(win);

   evas = evas_object_evas_get(win);

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

   /* Centre Frame Divider */
   hbox = elm_box_add(win);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);
 
   /* map scroller */
   scroller = elm_scroller_add(win);
   evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(hbox, scroller);
   evas_object_show(scroller);

   /* map object */
   map = pg_display_add(evas);
   pg_data.display = map;
   evas_object_size_hint_min_set(map, 1024, 1024);
   elm_object_content_set(scroller, map);
   evas_object_show(map);
 
   /* buttons Scroller */
   scroller = elm_scroller_add(win);
   evas_object_size_hint_weight_set(scroller, 0.0, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_scroller_content_min_limit(scroller, EINA_TRUE, EINA_FALSE);
   elm_scroller_policy_set(scroller, EINA_FALSE, EINA_TRUE);
   elm_box_pack_end(hbox, scroller);
   evas_object_show(scroller);

#include "main_buttons.h"

   /* bottom Status Bar */
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_min_set(bg, 32, 32);
   evas_object_color_set(bg, 0, 0, 255, 255);
   elm_box_pack_end(vbox, bg);
   evas_object_show(bg);
 

   elm_run(); // run main loop

   elm_shutdown(); // after mainloop finishes running, shutdown

   return 0; // exit 0 for exit code

}

ELM_MAIN()

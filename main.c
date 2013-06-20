#include <Elementary.h>
#include "world.h"
#include "node.h"

static void 
_on_done(void *data, Evas_Object *obj, void *event_info)
{
   // quit the mainloop (elm_run function will return)
   elm_exit();
}

/* hook on the file,chosen smart callback */

static void
_wheel( void *data, Evas *evas, Evas_Object *o, void *event_info)
{
   Evas_Object *world = evas_object_name_find(evas, "world");
   evas_object_smart_callback_call( world, EVT_ZOOM, event_info);
}

static void 
_file_chosen(void *data, Evas_Object *obj, void *event_info)
{
   const char *file = event_info;

   if (file) printf("File chosen: %s\n", file);
   else
   {
      printf("File selection canceled.\n");
      return;
   }
   Evas *evas = evas_object_evas_get(obj);
   Evas_Object *world = evas_object_name_find(evas, "world");
   Evas_Object *image, *old;

   image = evas_object_image_filled_add(evas);
   evas_object_image_file_set(image, file, NULL);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);
   evas_object_show(image);

   old = pathgen_world_set_height(world, image);
   evas_object_del(old);
}

EAPI_MAIN int

elm_main(int argc, char **argv)
{
   Evas_Object *win, *bg, *vbox, *hbox, *fs_entry, *sep, *btn, *scroll;

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
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_homogeneous_set(vbox, EINA_TRUE);
   elm_box_pack_end(hbox, vbox);
   evas_object_show(vbox);

   /* file selector entry */
   fs_entry = elm_fileselector_entry_add(win);
   evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, 0.0);
   elm_fileselector_entry_path_set(fs_entry, "select height map");
   elm_fileselector_entry_expandable_set(fs_entry, EINA_FALSE);
   elm_object_text_set(fs_entry, "Select height map");
   elm_box_pack_end(vbox, fs_entry);
   evas_object_show(fs_entry);

   evas_object_smart_callback_add(fs_entry, "file,chosen", _file_chosen, NULL);

   ecore_timer_add(1.0, simulate, NULL);

   // now we are done, show the window
   evas_object_resize(win, 400, 400);
   evas_object_show(win);

   elm_run(); // run main loop

   elm_shutdown(); // after mainloop finishes running, shutdown

   return 0; // exit 0 for exit code

}

ELM_MAIN()

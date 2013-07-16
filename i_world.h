#ifndef I_WORLD_H
#define I_WORLD_H

#include "pathgen_node.h"
#include "r_image.h"
#include "r_noise.h"
#include "pg_data.h"

extern PG_Data pg_data;

static void 
_fs_world_height_load(void *data, Evas_Object *obj, void *event_info)
{
   const char *file = event_info;
   Evas *evas;
   Evas_Object *image;

   if(!file) return;

   evas = evas_object_evas_get(obj);

   image = evas_object_image_filled_add(evas);
   evas_object_image_file_set(image, file, NULL);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);

   pathgen_world_layer_set(data, image, 0);

   pg_world_layer_set(pg_data.world, image, 1);
}

static void
_btn_world_save_height(void *data, Evas_Object *o, void *event_info)
{
   Evas *evas;
   Evas_Object *world;
   if(!event_info)return;
   
   evas = evas_object_evas_get(o);
   world = evas_object_name_find(evas, "world");
   if(!world)return;
   PATHGEN_WORLD_DATA_GET(world, priv);
   if(!priv->l[0])return;

   evas_object_image_save(priv->l[0], (const char *)event_info, NULL, NULL);
}

static void
_spin_world_height_mult(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;

   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_world_height_mult = (float)elm_spinner_value_get(o);
}

static void 
_fs_world_pathmap_load(void *data, Evas_Object *obj, void *event_info)
{
   const char *file = event_info;
   Evas *evas;
   Evas_Object *image;

   if(!file) return;

   evas = evas_object_evas_get(obj);

   image = evas_object_image_filled_add(evas);
   evas_object_image_file_set(image, file, NULL);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);

   pathgen_world_layer_set(data, image, 3);
}

static void
_spin_world_pathmap_mult(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;

   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_world_pathmap_mult = (float)elm_spinner_value_get(o);
}

static void 
_fs_world_spawnmap_load(void *data, Evas_Object *obj, void *event_info)
{
   const char *file = event_info;
   Evas *evas;
   Evas_Object *image;

   if(!file) return;

   evas = evas_object_evas_get(obj);

   image = evas_object_image_filled_add(evas);
   evas_object_image_file_set(image, file, NULL);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);

   pathgen_world_layer_set(data, image, 4);
}

static void 
_fs_world_avoid_load(void *data, Evas_Object *obj, void *event_info)
{
   const char *file = event_info;
   Evas *evas;
   Evas_Object *image;

   if(!file) return;

   evas = evas_object_evas_get(obj);

   image = evas_object_image_filled_add(evas);
   evas_object_image_file_set(image, file, NULL);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);

   pathgen_world_layer_set(data, image, 2);
}

static void
_spin_world_avoid_mult(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;

   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_world_avoid_mult = (float)elm_spinner_value_get(o);
}

static void 
_fs_world_teleport_load(void *data, Evas_Object *obj, void *event_info)
{
   const char *file = event_info;
   Evas *evas;
   Evas_Object *image;

   if(!file) return;

   evas = evas_object_evas_get(obj);

   image = evas_object_image_filled_add(evas);
   evas_object_image_file_set(image, file, NULL);
   evas_object_image_smooth_scale_set(image, EINA_FALSE);

   pathgen_world_layer_set(data, image, 1);
}

static void
_hoversel_path_distance_goal_manhat(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->distance_to_goal = pathgen_node_dist_manhat;
}

static void
_hoversel_path_distance_goal_diagon(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->distance_to_goal = pathgen_node_dist_diagon;
}

static void
_hoversel_path_distance_goal_euclid(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->distance_to_goal = pathgen_node_dist_euclid;
}

static void
_spin_path_distance_goal_mult(void *data, Evas_Object *o, void *event_info)
{
   if(!data)return;
   PATHGEN_WORLD_DATA_GET(data, priv);
   priv->i_path_distance_goal_mult = (float)elm_spinner_value_get(o);
   fprintf(stderr, "i_path_distance_goal_mult = %f\n",
      priv->i_path_distance_goal_mult);
}


static void
i_world_setup(Evas_Object *win, Evas_Object *vbox)
{
   Evas_Object *world, *frm, *lab, *spin, *btn, *fs_entry, *hbox,
      *vbox1, *vbox2, *hov;

   world = evas_object_name_find(evas_object_evas_get(win),"world");

   frm = elm_frame_add(win);
   evas_object_size_hint_weight_set(frm, 0.0, 0.0);
   evas_object_size_hint_align_set(frm, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(frm, "World Options");
   elm_frame_autocollapse_set(frm, EINA_TRUE);
   elm_box_pack_end(vbox, frm);
   evas_object_show(frm);

   /* button divider */
   vbox = elm_box_add(win);
   evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(frm, vbox);
   evas_object_show(vbox);

   /***********
   * Spawnmap *
   ************/
   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* file selector entry */
   fs_entry = elm_fileselector_button_add(win);
   evas_object_name_set(fs_entry, "world,spawnmap,load");
   elm_fileselector_button_expandable_set(fs_entry, EINA_FALSE);
   elm_object_text_set(fs_entry, "Load Spawnmap");
   evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(fs_entry, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, fs_entry);
   evas_object_show(fs_entry);

   evas_object_smart_callback_add(
      fs_entry, "file,chosen", _fs_world_spawnmap_load, world);

   /***********************
   * Pathmap Image Options *
   ***********************/
   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* file selector entry */
   fs_entry = elm_fileselector_button_add(win);
   evas_object_name_set(fs_entry, "world,pathmap,load");
   elm_fileselector_button_expandable_set(fs_entry, EINA_FALSE);
   elm_object_text_set(fs_entry, "Load Path map");
   evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(fs_entry, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, fs_entry);
   evas_object_show(fs_entry);

   evas_object_smart_callback_add(
      fs_entry, "file,chosen", _fs_world_pathmap_load, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "Value: %.3f");
   elm_spinner_min_max_set(spin, I_WORLD_PATH_MULT_MIN,
      I_WORLD_PATH_MULT_MAX);
   elm_spinner_value_set(spin, I_WORLD_PATH_MULT_DEFAULT);
   elm_spinner_step_set(spin, I_WORLD_PATH_MULT_STEP);

   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_world_height_mult, world);

   /***********************
   * Avoidance Image Options *
   ***********************/
   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* file selector entry */
   fs_entry = elm_fileselector_button_add(win);
   evas_object_name_set(fs_entry, "world,avoid,load");
   elm_fileselector_button_expandable_set(fs_entry, EINA_FALSE);
   elm_object_text_set(fs_entry, "Load Avoidance");
   evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(fs_entry, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, fs_entry);
   evas_object_show(fs_entry);

   evas_object_smart_callback_add(
      fs_entry, "file,chosen", _fs_world_avoid_load, world);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "Value: %.3f");
   elm_spinner_min_max_set(spin, I_WORLD_AVOID_MULT_MIN,
      I_WORLD_AVOID_MULT_MAX);
   elm_spinner_value_set(spin, I_WORLD_AVOID_MULT_DEFAULT);
   elm_spinner_step_set(spin, I_WORLD_AVOID_MULT_STEP);

   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_world_avoid_mult, world);

   /***********************
   * Teleport ID Image Options *
   ***********************/
   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* file selector entry */
   fs_entry = elm_fileselector_button_add(win);
   evas_object_name_set(fs_entry, "world,teleport,load");
   elm_fileselector_button_expandable_set(fs_entry, EINA_FALSE);
   elm_object_text_set(fs_entry, "Load Teleport");
   evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(fs_entry, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, fs_entry);
   evas_object_show(fs_entry);

   evas_object_smart_callback_add(
      fs_entry, "file,chosen", _fs_world_teleport_load, world);

   /***********************
   * Height Image Options *
   ***********************/
   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   /* file selector entry */
   fs_entry = elm_fileselector_button_add(win);
   evas_object_name_set(fs_entry, "world,height,load");
   elm_fileselector_button_expandable_set(fs_entry, EINA_FALSE);
   elm_object_text_set(fs_entry, "Load Height");
   evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(fs_entry, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(hbox, fs_entry);
   evas_object_show(fs_entry);

   evas_object_smart_callback_add(
      fs_entry, "file,chosen", _fs_world_height_load, world);

//   btn = elm_fileselector_button_add(win);
//   elm_fileselector_button_window_title_set(btn, "Save Heightmap");
//   elm_fileselector_button_is_save_set(btn, EINA_TRUE);
//   elm_object_text_set(btn, "Save Heightmap");
//   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
//   evas_object_size_hint_weight_set(fs_entry, EVAS_HINT_EXPAND, 0.0);
//   elm_box_pack_end(hbox, btn);
//   evas_object_show(btn);

//  evas_object_smart_callback_add(btn, "file,chosen",
//      _btn_world_save_height, NULL);

   spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "value: %.3f");
   elm_spinner_min_max_set(spin, I_WORLD_HEIGHT_MULT_MIN,
      I_WORLD_HEIGHT_MULT_MAX);
   elm_spinner_value_set(spin, I_WORLD_HEIGHT_MULT_DEFAULT);
   elm_spinner_step_set(spin, I_WORLD_HEIGHT_MULT_STEP);

   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_world_height_mult, world);

   /*********************
   * Distance Algorithm *
   *********************/

   /* sub divider */
   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_homogeneous_set(hbox, EINA_TRUE);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, 0.0);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, 0.0);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   hov = elm_hoversel_add(win);
   elm_hoversel_hover_parent_set(hov, win);
   evas_object_size_hint_align_set(hov, EVAS_HINT_FILL, 0);
   evas_object_size_hint_weight_set(hov, EVAS_HINT_EXPAND, 0);
   elm_object_text_set(hov, "Distance to Goal");
   elm_hoversel_item_add(hov, "Manhatten", NULL, ELM_ICON_NONE, _hoversel_path_distance_goal_manhat, world);
   elm_hoversel_item_add(hov, "Diagonal", NULL, ELM_ICON_NONE, _hoversel_path_distance_goal_diagon, world);
   elm_hoversel_item_add(hov, "Euclidean", NULL, ELM_ICON_NONE, _hoversel_path_distance_goal_euclid, world);
   elm_box_pack_end(hbox, hov);
   evas_object_show(hov);

  spin = elm_spinner_add(win);
   elm_spinner_label_format_set(spin, "%0.3f");
   elm_spinner_min_max_set(spin, 0.0, 10.0);
   elm_spinner_step_set(spin, 0.001);
   elm_spinner_value_set(spin, I_PATH_DISTANCE_GOAL_MULT_DEFAULT);

   evas_object_size_hint_weight_set(spin, 0.5, 0.0);
   evas_object_size_hint_align_set(spin, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(hbox, spin);
   evas_object_show(spin);
   
   evas_object_smart_callback_add(spin, "delay,changed",
      _spin_path_distance_goal_mult, world);


}

#endif /*I_WORLD_H*/

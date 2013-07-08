#ifndef I_DEFAULTS_H
#define I_DEFAULTS_H

#define I_DISPLAY_HEATMAP_DEFAULT EINA_TRUE
#define I_DISPLAY_SEARCH_DEFAULT EINA_TRUE
#define I_DISPLAY_PATH_DEFAULT EINA_TRUE
#define I_DISPLAY_HEIGHT_DEFAULT EINA_TRUE
#define I_DISPLAY_SPEED_DEFAULT 0.01

#define I_SIM_PATHS_MAX_DEFAULT 20
#define I_SIM_SEARCH_ITER_MAX_DEFAULT 20
#define I_SIM_PATH_FADE_STRENGTH_DEFAULT 20
#define I_SIM_PATH_FADE_INTERVAL_DEFAULT 20

#define I_WORLD_HEIGHT_MULT_MIN 0.001
#define I_WORLD_HEIGHT_MULT_MAX 10
#define I_WORLD_HEIGHT_MULT_STEP 0.001
#define I_WORLD_HEIGHT_MULT_DEFAULT 1

#define I_WORLDGEN_W_MIN 32
#define I_WORLDGEN_W_MAX 2048
#define I_WORLDGEN_W_STEP 32
#define I_WORLDGEN_W_DEFAULT 128

#define I_WORLDGEN_H_MIN 32
#define I_WORLDGEN_H_MAX 2048
#define I_WORLDGEN_H_STEP 32
#define I_WORLDGEN_H_DEFAULT 128

#define I_WORLDGEN_DENSITY_MIN 1
#define I_WORLDGEN_DENSITY_MAX 1024
#define I_WORLDGEN_DENSITY_STEP 1
#define I_WORLDGEN_DENSITY_DEFAULT 16

#define I_PATH_SEARCH_DIAGONAL_DEFAULT EINA_TRUE
#define I_PATH_TREAD_WEIGHT_DEFAULT 0.5

#define I_PATH_DISTANCE_START_MULT_DEFAULT 1.0
#define I_PATH_DISTANCE_GOAL_MULT_DEFAULT 1.0

#define I_PATH_CLIMB_UP_MIN_DEFAULT 0
#define I_PATH_CLIMB_UP_MAX_DEFAULT 255
#define I_PATH_CLIMB_DOWN_MIN_DEFAULT 0
#define I_PATH_CLIMB_DOWN_MAX_DEFAULT 255

#endif /*I_DEFAULTS_H*/

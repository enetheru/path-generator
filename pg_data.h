#ifndef PG_DATA_H
#define PG_DATA_H

#include <Eina.h>
#include <Evas.h>
#include <Ecore.h>

typedef struct _PG_Data PG_Data;
typedef struct _PG_World PG_World;
typedef struct _PG_Path PG_Path;
typedef struct _PG_Path_Finder PG_Path_Finder;
typedef struct _PG_Node PG_Node;
typedef struct _PG_Node_Rel PG_Node_Rel;

struct _PG_Data
{
   /* evas smart object to display the data*/
   Evas_Object *display;

   int path_count;
   int path_fade_counter;

   PG_World *world;

   Ecore_Event_Handler *path_more;
   Ecore_Event_Handler *sim_stop;
};

struct _PG_World
{
   /* there are 7 data layers */
   /* 0 background or pretty  */
   /* 1 height/displacement */
   /* 2 avoidance */
   /* 3 arbitrary path map */
   /* 4 spawn map */
   /* 5 teleportation */
   /* 6 resulting heat map */
   Evas_Object *l[7];
   int width, length ,height;
   /* two dimensional array of nodes */
   Eina_List *nodes;
};

struct _PG_Path
{
   Eina_List *nodes;
   PG_Node_Rel *start, *end, *current;
};

struct _PG_Finder
{
   PG_Path *path;
   PG_World *world;
   Eina_List *open, *closed;
   PG_Node_Rel *goal, *start;

   int iteration_count;
   int iteration_max;
   Eina_Bool diagonal_search;
   int heat_minl, heat_min, heat_max, heat_maxl;
   int map_minl, map_min, map_max, map_maxl;
   int avoid_minl, avoid_min, avoid_max, avoid_maxl;
   int climb_minl, climb_min, climb_max, climb_maxl;
   
  // distance algorithm
};

struct _PG_Node
{
   /* position */
   int x, y, z;
   /* the neighbours */
   PG_Node *n[26];
};

struct _PG_Node_Rel
{
   PG_Node *node;
   PG_Node_Rel *prev, *next;
   /* g = total cost to get here */
   /* h = the perceived cost to the goal */
   /* f = g+h, used in selecting the best node */
   double f,g,h;
   /* state is either 0 none, 1 in open list, -1 in closed list*/
   int state;
};

#endif /* PG_DATA_H */

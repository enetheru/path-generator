#ifndef PG_PATH_FINDER_H
#define PG_PATH_FINDER_H

#include "pg_data.h"

PG_Path_Finder *
pg_path_finder_new(PG_Path *path, int x, int y);

void
pg_path_finder_del(PG_Path_Finder *path_finder);

void
pg_path_finder_goal_set(PG_Path_Finder *pf, int x, int y);

void
pg_path_finder_th_do(void *data, Ecore_Thread *th);

void // thread job finished - collect results and put in img obj
pg_path_finder_th_end(void *data, Ecore_Thread *th);

void // if the thread is cancelled - free pix, keep obj tho
pg_path_finder_th_cancel(void *data, Ecore_Thread *th);

#endif /* PG_PATH_FINDER_H */


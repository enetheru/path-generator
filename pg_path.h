#ifndef PG_PATH_H
#define PG_PATH_H

#include "pg_data.h"

extern PG_Data pg_data;

PG_Path *
pg_path_new(int x, int y);

void
pg_path_del(PG_Path *path);

#endif /* PG_PATH_H */


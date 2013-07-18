#include "pg_data.h"
#include "pg_path_finder.h"

PG_Path_Finder *
pg_path_finder_new()
{
   PG_Path_Finder *data = malloc(sizeof(PG_Path_Finder));

   return data;
}



#ifndef PG_EVENT_H
#define PG_EVENT_H

int _event_id_path_more = 0;
int _event_id_sim_stop = 0;

typedef struct _event_data_path_more Event_Data_Path_More;
struct _event_data_path_more{
   Evas *evas;
};

#endif /* PG_EVENT_H */


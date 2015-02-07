#ifndef PROCESS_H
#define PROCESS_H
#include <minix/endpoint.h>
#include "list.h"

void process_init();
bool processes_equal(const void* a, const void* b);
bool is_process_observed(endpoint_t process);
void start_observation(endpoint_t process);
void stop_observation(endpoint_t process);

#endif

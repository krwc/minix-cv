#ifndef UTILITY_H
#define UTILITY_H
#include <minix/ipc.h>
#include <minix/endpoint.h>

void tell_process(endpoint_t endpoint, int message);

#endif

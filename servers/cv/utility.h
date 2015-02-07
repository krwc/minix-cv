#ifndef UTILITY_H
#define UTILITY_H
#include <minix/ipc.h>
#include <minix/endpoint.h>
#include <stdio.h>

void tell_process(endpoint_t endpoint, int message);

#if defined(DEBUG)
#define debug(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define debug(fmt, ...) 
#endif

#endif

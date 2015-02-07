#ifndef UTILITY_H
#define UTILITY_H
#include <minix/ipc.h>
#include <minix/endpoint.h>
#include <stdio.h>

/* if defined, cv is quite verbose */
#define DEBUG

void tell_process(endpoint_t endpoint, int message);

#if defined(DEBUG)
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...) 
#endif

#endif

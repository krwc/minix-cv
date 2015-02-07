#ifndef CS_H
#define CS_H
#include <minix/endpoint.h>

#define MSG_DONE     0
#define MSG_ERROR   -1
#define MSG_SIGNAL  -2
#define MSG_KILLED  -3

endpoint_t get_cv_id();
int cs_lock(int);
int cs_unlock(int);
int cs_wait(int, int);
int cs_broadcast(int);

#endif

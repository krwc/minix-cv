#ifndef MUTEX_H
#define MUTEX_H
#include <minix/endpoint.h>
#include "list.h"

typedef struct Mutex {
    endpoint_t owner;
    int id;
} Mutex;

void mutex_init();
bool mutex_id_equal(const void* a, const void* b);
bool mutex_equal(const void* a, const void* b);
bool mutex_owner_equal(const void* a, const void* b);
bool is_mutex_owned(int mutex_id);
bool is_owning_mutexes(endpoint_t);
bool is_requesting_mutexes(endpoint_t);
void register_mutex(endpoint_t owner, int mutex_id);
bool unregister_mutex(endpoint_t owner, int mutex_id);
void enqueue_request(endpoint_t owner, int mutex_id);
endpoint_t dequeue_request(int mutex_id);
void remove_all_requests_by_owner(endpoint_t process);
void remove_all_mutexes_of_killed_process(endpoint_t process);

#endif

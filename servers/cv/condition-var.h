#ifndef CONDITION_VAR_H
#define CONDITION_VAR_H
#include "list.h"
#include <minix/endpoint.h>
#include <stdbool.h>

typedef struct ConditionVar {
    /* cvar identifier */
    int id;
    /* process waiting for broadcast */
    endpoint_t proc;
} ConditionVar;
void condition_vars_init();
void register_cond_var(endpoint_t proc, int cond_var_id);
void unregister_cond_var(endpoint_t proc);
void broadcast_cond_var(int cond_var_id);
bool is_waiting(endpoint_t process);

#endif

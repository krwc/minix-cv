#include "condition-var.h"
#include "process.h"
#include "utility.h"
#include <minix/cv.h>

/* list of registered condition variables */
static List condition_vars;

void condition_vars_init() {
    list_init(&condition_vars, sizeof(ConditionVar));
}

void register_cond_var(endpoint_t proc, int cond_var_id) {
    ConditionVar cvar;
    printf("register_cond_var(%d, %d)\n", proc, cond_var_id);
    cvar.id = cond_var_id;
    cvar.proc = proc;
    list_push_back(&condition_vars, &cvar);
}

void unregister_cond_var(endpoint_t proc) {
    ConditionVar* var;
    ListNode* node = condition_vars.head->next;
    ListNode* next = NULL;
    
    while (node != condition_vars.tail) {
        next = node->next;
        var = node->data;

        if (var->proc == proc) {
            printf("unregister_cond_var(): removed (%d, %d)\n", proc, var->id);
            list_remove(node);
        }
        node = next;
    }
}

void broadcast_cond_var(int cond_var_id) {
    ConditionVar* var;
    ListNode* node = condition_vars.head->next;
    ListNode* next = NULL;

    while (node != condition_vars.tail) {
        next = node->next;
        var = node->data;

        if (var->id == cond_var_id) {
            /* done waiting */
            tell_process(var->proc, MSG_DONE);
            list_remove(node);
        }
        node = next;
    }
}

bool is_waiting(endpoint_t process) {
    ConditionVar* var;
    ListNode* node = condition_vars.head->next;
    ListNode* next = NULL;
    
    while (node != condition_vars.tail) {
        next = node->next;
        var = node->data;

        if (var->proc == process) 
            return true;
        node = next;
    }
    return false;
}

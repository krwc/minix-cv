#include "process.h"
#include <stdio.h>

/* List of observed processes */
static List observed_processes;

void process_init() {
    list_init(&observed_processes, sizeof(endpoint_t));
}

bool processes_equal(const void* a, const void* b) {
    return *(endpoint_t*)a == *(endpoint_t*)b;
}

bool is_process_observed(endpoint_t process) {
    return list_find(&observed_processes, &process, processes_equal) != NULL;
}

void start_observation(endpoint_t process) {
    debug("start_observation(): proc=%d\n", process);
    vm_watch_exit(process);
    list_push_back(&observed_processes, &process);
}

void stop_observation(endpoint_t process) {
    ListNode* node = list_find(&observed_processes, &process, processes_equal);

    if (node) {
        debug("stop_observation(): proc=%d\n", process);
        list_remove(node);
    }
}


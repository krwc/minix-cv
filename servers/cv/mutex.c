#include "mutex.h"
#include "utility.h"
#include "process.h"
#include <minix/cv.h>

// List of owned mutexes
static List mutexes_owned;
// Queue of requested mutexes
static List mutexes_requested;

void mutex_init() {
    list_init(&mutexes_owned, sizeof(Mutex));
    list_init(&mutexes_requested, sizeof(Mutex));
}

bool mutex_id_equal(const void* a, const void* b) {
    return ((Mutex*)a)->id == ((Mutex*)b)->id;
}

bool mutex_owner_equal(const void* a, const void* b) {
    return ((Mutex*)a)->owner == ((Mutex*)b)->owner;
}

bool mutex_equal(const void* a, const void* b) {
    return ((Mutex*)a)->id    == ((Mutex*)b)->id &&
           ((Mutex*)a)->owner == ((Mutex*)b)->owner;
}

bool is_mutex_owned(int mutex_id) {
    Mutex mutex;
    mutex.id = mutex_id;
    return list_find(&mutexes_owned, &mutex, mutex_id_equal) != NULL;
}

bool is_owning_mutexes(endpoint_t owner) {
    Mutex mutex;
    mutex.owner = owner;
    return list_find(&mutexes_owned, &mutex, mutex_owner_equal) != NULL;
}

bool is_mutex_owner(endpoint_t owner, int mutex_id) {
    Mutex mutex;
    mutex.owner = owner;
    mutex.id = mutex_id;
    return list_find(&mutexes_owned, &mutex, mutex_equal) != NULL;
}

bool is_requesting_mutexes(endpoint_t owner) {
    Mutex mutex;
    mutex.owner = owner;
    return list_find(&mutexes_requested, &mutex, mutex_owner_equal) != NULL;
}

void register_mutex(endpoint_t owner, int mutex_id) {
    Mutex mutex;
    mutex.owner = owner;
    mutex.id = mutex_id;
   
    /* new process needs to be observed */
    if (!is_process_observed(owner))
        start_observation(owner);
    
    /* process wants owned lock again */
    if (is_mutex_owner(owner, mutex_id))
        tell_process(owner, MSG_ERROR);
    else if (is_mutex_owned(mutex_id))
        enqueue_request(owner, mutex_id);
    else {
        printf("register_mutex(): adding (%d, %d)\n", owner, mutex_id);
        list_push_back(&mutexes_owned, &mutex);

        /* tell process about mutex being acquired */
        tell_process(owner, MSG_DONE);
    }
}

bool unregister_mutex(endpoint_t owner, int mutex_id) {
    Mutex mutex;
    mutex.owner = owner;
    mutex.id = mutex_id;
    ListNode* node = list_find(&mutexes_owned, &mutex, mutex_equal);
    if (node) {
        printf("unregister_mutex(): removing (%d, %d)\n", owner, mutex_id);
        list_remove(node);
        
        /* tell process (if it is alive, otherwise info about deadlock will occur) */
        if (is_process_observed(owner))
            tell_process(owner, MSG_DONE);

        /* if proces does not own any more mutexes, stop observation */
        if (!is_owning_mutexes(owner))
            stop_observation(owner);

        /* if somebody wants this mutex, give it to him */
        endpoint_t new_owner = dequeue_request(mutex_id);
        
        if (new_owner)
            register_mutex(new_owner, mutex_id);

        return true;
    } else
        /* process did not own given mutex */
        tell_process(owner, MSG_ERROR);
    return false;
}

void enqueue_request(endpoint_t owner, int mutex_id) {
    Mutex mutex;
    mutex.owner = owner;
    mutex.id = mutex_id;
    printf("enqueue_request(): (%d, %d)\n", owner, mutex_id);    
    list_push_back(&mutexes_requested, &mutex);
}

/**
 * Dequeues request on given mutex_id, and if there is no such request
 * returns 0.
 */
endpoint_t dequeue_request(int mutex_id) {
    Mutex mutex;
    mutex.id = mutex_id;
    endpoint_t requesting_proc;
    ListNode* node = list_find(&mutexes_requested, &mutex, mutex_id_equal);

    if (node == NULL)
        return 0;
    requesting_proc = ((Mutex*)node->data)->owner;
    printf("dequeue_request(): %d\n", mutex_id);
    list_remove(node);
    return requesting_proc;
}

void remove_all_requests_by_owner(endpoint_t process) {
    ListNode* node = list_first(&mutexes_requested);
    ListNode* next = NULL;

    while (node && node != mutexes_requested.tail) {
        next = node->next;
        Mutex* mutex = node->data;

        if (mutex->owner == process)
            list_remove(node);

        node = next;
    }
    /* now we know process does not request any locks, and 
     * if it does not own any locks either then we can stop observation
     */
    if (!is_owning_mutexes(process))
        stop_observation(process);
}

void remove_all_mutexes_of_killed_process(endpoint_t process) {
    ListNode* node = list_first(&mutexes_owned);
    ListNode* next = NULL;
    
    /* all mutexes means all */
    remove_all_requests_by_owner(process);
    /* process is dead, it doesn't care about messages */
    stop_observation(process);

    while (node && node != mutexes_owned.tail) {
        next = node->next;
        Mutex* mutex = node->data;

        if (mutex->owner == process)
            unregister_mutex(mutex->owner, mutex->id);

        node = next;
    }
}

#include "inc.h"
#include <minix/cv.h>

void wait(endpoint_t owner, int mutex_id, int cond_var_id) {
    /* tries to unregister mutex */
    bool process_observed = is_process_observed(owner);
    
    /* stop observation for a moment to avoid sending message to the process */
    if (process_observed)
        stop_observation(owner);

    if (unregister_mutex(owner, mutex_id))
        register_cond_var(owner, cond_var_id);
    
    /* observed process shall stay observed */
    if (process_observed)
        start_observation(owner);
}

void broadcast(int cond_var_id) {
    broadcast_cond_var(cond_var_id);
}

void init() {
    sef_local_startup();
    mutex_init();
    process_init();
    condition_vars_init();
}

int main(int argc, char** argv)
{
    message m;

    /* initialize */
    init();    
    
    debug("%s", "CV: started\n");
    while (TRUE) {
        int result;
        int who;
        int type;

        if ((result = sef_receive(ANY, &m)) != OK) {
            debug("CV: sef_receive failed - %d\n", result);
            continue;
        }
        who  = m.m_source;
        type = m.m_type; 
        
        if (type & NOTIFY_MESSAGE) {
            if (who == PM_PROC_NR && is_process_observed(m.m1_i2)) {
                debug("CV: pm told us %d about process %d\n", m.m1_i1, m.m1_i2);
                endpoint_t process = m.m1_i2;

                switch (m.m1_i1) {
                case MSG_SIGNAL:
                    if (is_requesting_mutexes(process) || is_waiting(process)) {
                        /* remove signaled process request from the queue */
                        remove_all_requests_by_owner(process);
                        /* unregister cvar of this process */
                        unregister_cond_var(process);
                        /* tell him about signal IFF it is waiting */
                        tell_process(process, MSG_SIGNAL);
                    }
                    break;
                case MSG_KILLED:
                    /* free all mutexes and requests owned by this process */
                    remove_all_mutexes_of_killed_process(process);
                    /* it will not care about cvars either */
                    unregister_cond_var(process);
                    break;
                default:
                    debug("CV: received unexpected message.");
                    break;
                }
            }
            continue;
        } 
        
        switch (type) {
        case CV_LOCK:
            register_mutex(who, m.m1_i1);
            break;
        case CV_UNLOCK:
            unregister_mutex(who, m.m1_i1);
            break;
        case CV_WAIT:
            wait(who, m.m1_i1, m.m1_i2);
            break;
        case CV_BROADCAST:
            broadcast(m.m1_i1);
            tell_process(who, MSG_DONE);
            break;
        default:
            debug("CV: received unknown command - %d\n", type);
            break;
        }
    }

    return -1;
}

/*===========================================================================*
 *                             sef_local_startup                             *
 *===========================================================================*/
static void sef_local_startup()
{
  /* Register init callbacks. */
  sef_setcb_init_fresh(sef_cb_init_fresh);
  sef_setcb_init_restart(sef_cb_init_fresh);

  /* No live update support for now. */

  /* Register signal callbacks. */
  sef_setcb_signal_handler(sef_cb_signal_handler);

  /* Let SEF perform startup. */
  sef_startup();
}

/*===========================================================================*
 *                          sef_cb_init_fresh                                *
 *===========================================================================*/
static int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *UNUSED(info))
{
    return(OK);
}

/*===========================================================================*
 *                          sef_cb_signal_handler                            *
 *===========================================================================*/
static void sef_cb_signal_handler(int signo)
{
    return;
}


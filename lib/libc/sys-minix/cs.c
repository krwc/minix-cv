#include <lib.h>
#include <minix/rs.h>
#include <minix/cv.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <errno.h>

endpoint_t get_cv_id()
{
    endpoint_t cv_id;
    assert(minix_rs_lookup("cv", &cv_id) == 0 && "CV is not running");

    return cv_id;
}

int cs_lock(int mutex_id) 
{
    int ret;
    while (true) {
        message m;
        m.m1_i1 = mutex_id;

        ret = _syscall(get_cv_id(), CV_LOCK, &m);
        
        /* signal to be handled, we're trying again */
        if (m.m_type == MSG_SIGNAL)
            continue;
        else
            break;
    }
    return ret;
}

int cs_unlock(int mutex_id)
{
    message m;
    m.m1_i1  = mutex_id;
    
    if (_syscall(get_cv_id(), CV_UNLOCK, &m) == MSG_ERROR) {
        errno = EPERM;
        return -1;
    }
    return 0;   
}

int cs_wait(int cond_var_id, int mutex_id) 
{
    while (true) {
        message m;
        m.m1_i1 = mutex_id;
        m.m1_i2 = cond_var_id;

        _syscall(get_cv_id(), CV_WAIT, &m);
        
        if (m.m_type == MSG_ERROR) {
            errno = EINVAL;
            return -1;
        } else
            /* done waiting - either because of spurious wakeup or because we were 
             * broadcasted */
            return cs_lock(mutex_id);
    }
}

int cs_broadcast(int cond_var_id) 
{
    message m;
    m.m1_i1 = cond_var_id;
    return _syscall(get_cv_id(), CV_BROADCAST, &m);
}

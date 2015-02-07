#include "utility.h"

void tell_process(endpoint_t proc, int msg) {
    message m;
    m.m_type = msg;
    sendnb(proc, &m);
}


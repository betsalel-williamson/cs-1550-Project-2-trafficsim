//
// Created by School on 6/4/16.
//

#ifndef PROJECT1_SYSCALLS_H
#define PROJECT1_SYSCALLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "traffic_rules.h"
#include <printf.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

extern int my_clock;

void sleep_ms(long ms);

void *increment_clock_thread(void *ptr);

void init_simulator();

void destruct_simulator();

#ifdef __cplusplus
}
#endif

#endif //PROJECT1_SYSCALLS_H

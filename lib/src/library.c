//
// Created by School on 6/4/16.
//

#include "library.h"


void sleep_ms(long ms) {
    struct timespec request = {0};

    time_t sec = (int) (ms / 1000);
    ms = ms - (sec * 1000);

    request.tv_sec = sec;
    request.tv_nsec = ms * 1000000L;

    while (nanosleep(&request, &request) == -1 && errno == EINTR)
        continue;
}


void *increment_clock_thread(void *ptr) {

    while (TRUE) {
        my_clock += 1;
        sleep_ms(1000);
    }

//    return &my_clock;
}

void init_simulator() {

    /* initialize random seed: */
    srand((unsigned int) time(NULL));

    init_roads();
    init_queues();

    /* Create independent threads each of which will execute function */
    pthread_t produce_right_thread,
            produce_left_thread,
            consume_right_thread,
            consume_left_thread,
            consume_flag_person_thread,
            clock_thread;

    int iret1, iret2, iret3, iret4, iret5, iret6;

    iret1 = pthread_create(&produce_right_thread, NULL, produce_right_lane_traffic_thread, NULL);
    if (iret1) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
        exit(EXIT_FAILURE);
    }

    iret2 = pthread_create(&produce_left_thread, NULL, produce_left_lane_traffic_thread, NULL);
    if (iret2) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }

    iret3 = pthread_create(&consume_right_thread, NULL, move_right_lane_thread, NULL);
    if (iret3) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }
    iret4 = pthread_create(&consume_left_thread, NULL, move_left_lane_thread, NULL);
    if (iret4) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }

    iret5 = pthread_create(&consume_flag_person_thread, NULL, flag_person_thread, NULL);
    if (iret5) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }

    iret6 = pthread_create(&clock_thread, NULL, increment_clock_thread, NULL);
    if (iret6) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }
}


void destruct_simulator() {
    destruct_queues();
}

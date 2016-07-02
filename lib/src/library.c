//
// Created by School on 6/4/16.
//

#include "library.h"


// goes from 0 to int
int my_clock;

int foo() {
    printf("Hello World");

    return 1;
}

void sleep_ms(long ms) {
    struct timespec request = {0};

    time_t sec = (int) (ms / 1000);
    ms = ms - (sec * 1000);

    request.tv_sec = sec;
    request.tv_nsec = ms * 1000000L;

    while (nanosleep(&request, &request) == -1 && errno == EINTR)
        continue;
}


void increment_clock() {
    my_clock += 1;
//    move_cars();
}

//Treat the road as two queues, and have a producer for each direction putting cars into the queues at the appropriate times.


// thread to move right cars

// thread to move left cars


void init_simulator() {
    /* initialize random seed: */
    srand((unsigned int) time(NULL));

    if (pthread_mutex_init(&right_lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&left_lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&move_left_lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&move_right_lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        exit(EXIT_FAILURE);
    }

    init_roads();
    init_queues();

    pthread_t thread1, thread2;
    int iret1, iret2;

    /* Create independent threads each of which will execute function */

    iret1 = pthread_create(&thread1, NULL, add_car_to_right_road, NULL);
    if (iret1) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
        exit(EXIT_FAILURE);
    }

    iret2 = pthread_create(&thread2, NULL, add_car_to_left_road, NULL);
    if (iret2) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }

//    pthread_join(thread1, NULL);
//    pthread_join(thread2, NULL);




//    printf("pthread_create() for thread 1 returns: %d\n", iret1);
//    printf("pthread_create() for thread 2 returns: %d\n", iret2);

    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */



//    exit(EXIT_SUCCESS);
}


void destruct_simulator() {
    pthread_mutex_destroy(&right_lock);
    pthread_mutex_destroy(&left_lock);
    pthread_mutex_destroy(&move_left_lock);
    pthread_mutex_destroy(&move_left_lock);
}

//
// Created by School on 7/2/16.
//
#include <mach/boolean.h>
#include <lib/include/traffic_rules.h>


int flag_person = SLEEPING;
int direction_to_let_through = LEFT;

STAILQ_HEAD(left_lane_car_queue, Car) left_lane_car_queue_head =
        STAILQ_HEAD_INITIALIZER(left_lane_car_queue_head);
//struct left_lane_car_queue *left_car_headp;

STAILQ_HEAD(right_lane_car_queue, Car) right_lane_car_queue_head =
        STAILQ_HEAD_INITIALIZER(right_lane_car_queue_head);

//struct right_lane_car_queue *right_car_headp;

void init_queues() {
    STAILQ_INIT(&left_lane_car_queue_head);    /* Initialize the queue. */
    STAILQ_INIT(&right_lane_car_queue_head);   /* Initialize the queue. */
}

void destruct_queues() {

    /* Faster TailQ Deletion. */
    car n1 = STAILQ_FIRST(&left_lane_car_queue_head);
    car n2;
    while (n1 != NULL) {
        n2 = STAILQ_NEXT(n1, entries);
        free(n1);
        n1 = n2;
    }
    STAILQ_INIT(&left_lane_car_queue_head);

    n1 = STAILQ_FIRST(&right_lane_car_queue_head);
    while (n1 != NULL) {
        n2 = STAILQ_NEXT(n1, entries);
        free(n1);
        n1 = n2;
    }
    STAILQ_INIT(&right_lane_car_queue_head);
}

//int left_lane_has_car = 0;
//int right_lane_has_car = 0;

car right_road[ROAD_LENGTH]; // goes from road size to 0
car left_road[ROAD_LENGTH]; // goes from 0 to road size

int car_vin = 1;

//    When a car arrives, there is an 80% chance another car is following it, but once no car comes, there is a 20 second delay before any new car will come.

//    // fill right
//    produce_right();
//    move_right_cars();
//
//    // fill left
//    product_left();
//    move_left_(cars);



int calculate_median(car pCar, int direction) {
    int status = -1;

    if (direction_to_let_through != RIGHT && pCar != NULL) {
        status = PRINT_ACCIDENT;
    } else if (direction_to_let_through != LEFT && pCar != NULL) {
        status = PRINT_ACCIDENT;
    } else if (direction == RIGHT) {
        if (pCar != NULL) {
//            printf("%s", right_car);
            status = PRINT_RIGHT_CAR;
        } else {
            if (direction_to_let_through == RIGHT) {
//                printf("%s", road);
                status = PRINT_ROAD;
            } else {
                if (flag_person == SLEEPING) {
//                    printf("%s", traffic_cones_sleeping);
                    status = PRINT_TRAFFIC_CONE_SLEEPING;
                } else if (flag_person == !SLEEPING) {
//                    printf("%s", traffic_cones_active);
                    status = PRINT_TRAFFIC_CONE_AWAKE;
                }
            }
        }
    } else if (direction == LEFT) {
        if (pCar != NULL) {
//            printf("%s", left_car);
            status = PRINT_LEFT_CAR;
        } else {
            if (direction_to_let_through == LEFT) {
//                printf("%s", road);
                status = PRINT_ROAD;
            } else {
                if (flag_person == SLEEPING) {
//                    printf("%s", traffic_cones_sleeping);
                    status = PRINT_TRAFFIC_CONE_SLEEPING;
                } else if (flag_person == !SLEEPING) {
//                    printf("%s", traffic_cones_active);
                    status = PRINT_TRAFFIC_CONE_AWAKE;
                }
            }
        }
    } else {
        exit(EXIT_FAILURE);
    }

    return status;
}


bool right_lane_has_car() {
    return !STAILQ_EMPTY(&right_lane_car_queue_head);
}

bool left_lane_has_car() {
    return !STAILQ_EMPTY(&left_lane_car_queue_head);
}

pthread_mutex_t left_lock; // for use when moving cars

void *left_lane_traffic_thread(void *ptr) {
    while (TRUE) {
        pthread_mutex_lock(&left_lock);

        signed char add_car = FALSE;
//        signed char sleep = FALSE;

        int r = rand() % 10;

        if (left_lane_has_car() && r < 8) {
            // 80 of adding car
            add_car = TRUE;
        } else if (!left_lane_has_car()) {
            add_car = TRUE;
        }


        if (car_vin % 10 == 0
            || add_car == FALSE) {
            sleep_ms(5000);
        }
        if (!left_lane_has_car()) {
            sleep_ms(20000);
        }

        if (add_car) {
//            if (right_road[START_OF_RIGHT_ROAD] != NULL) {
//                move_left_lane();
//            }

            car my_car = malloc(sizeof(car));      /* Insert at the tail. */
            my_car->vin_number = car_vin++;
//        printf("Inserting %d\n", my_car->vin_number);
            STAILQ_INSERT_TAIL(&left_lane_car_queue_head, my_car, entries);
//            left_road[START_OF_LEFT_ROAD] = my_car;
        }
        pthread_mutex_unlock(&left_lock);
        sleep_ms(1000);
    }
    return NULL;
}

pthread_mutex_t right_lock; // for use when moving cars

void *right_lane_traffic_thread(void *ptr) {
    while (TRUE) {
        pthread_mutex_lock(&right_lock);

        signed char add_car = FALSE;

        int r = rand() % 10;
//    printf("Inserting %d; r=%d\n", car_vin, r);

        if (right_lane_has_car() && r < 8) {
            add_car = TRUE;
        } else if (!right_lane_has_car()) {
            // wait 20 seconds then add car
//            sleep_ms(20000);
            add_car = TRUE;
        }

        if (car_vin % 10 == 0
            || add_car == FALSE) {
            sleep_ms(5000);
        }
        if (!right_lane_has_car()) {
            sleep_ms(20000);
        }

        if (add_car) {
//            if (right_road[START_OF_RIGHT_ROAD] != NULL) {
//                move_right_lane();
//            }

            car my_car = malloc(sizeof(car));      /* Insert at the tail. */
            my_car->vin_number = car_vin++;
            STAILQ_INSERT_TAIL(&right_lane_car_queue_head, my_car, entries);
//            right_road[START_OF_RIGHT_ROAD] = my_car;
//        printf("Finished inserting %d\n", my_car->vin_number);
        }
//    else {
////        printf("Didn't insert %d\n", car_vin);
//    }


        pthread_mutex_unlock(&right_lock);
        sleep_ms(500);
    }
    return NULL;
}


//void move_cars() {
//    move_right_lane();
//    move_left_lane();
//}


void init_road(car *road) {
    for (int i = 0; i < ROAD_LENGTH; ++i) {
        road[i] = NULL;
    }
}

void init_roads() {
    init_road(right_road);
    init_road(left_road);
}


pthread_mutex_t move_right_lock; // for use when moving cars

void move_right_lane() {
    pthread_mutex_lock(&move_right_lock);

    int i;
    if (right_road[0] != NULL) {
        STAILQ_REMOVE_HEAD(&right_lane_car_queue_head, entries);
    }
    for (i = 0; i < ROAD_LENGTH; ++i) {
        right_road[i] = right_road[i + 1];
    }
    pthread_mutex_unlock(&move_right_lock);

}

pthread_mutex_t move_left_lock; // for use when moving cars

void move_left_lane() {
    pthread_mutex_lock(&move_left_lock);

    int i;
    if (left_road[0] != NULL) {
        STAILQ_REMOVE_HEAD(&left_lane_car_queue_head, entries);
    }
    for (i = ROAD_LENGTH - 1; i >= 0; --i) {
        left_road[i] = left_road[i - 1];
    }
    pthread_mutex_unlock(&move_left_lock);

}

// TODO: implement consumers for left and right road with way to alternate between the two. Make sure to use direction_to_let_through
void *consume_cars_thread(void *ptr) {

    // move the left cars down

    // move the right cars down

    // When a car arrives at either end, the flagperson will allow traffic from that side to continue to flow, until
    // there are no more cars, or until there are 10 cars lined up on the opposing side, at which time they will be
    // allowed to pass.

    return NULL;
}


int right_cars_out_of_queue() {
    return 0;
}


int left_cars_out_of_queue() {
    return 0;
}
// TODO: create producers to fill up left and right roads from the traffic threads
// from prompt: Treat the road as two queues, and have a producer for each direction putting cars into the queues at the
// appropriate times.


int right_cars_in_queue() {
    int i = 0;
    car np;
    STAILQ_FOREACH(np, &right_lane_car_queue_head, entries) {
        i++;
    }
    return i;
}


int left_cars_in_queue() {
    int i = 0;
    car np;
    STAILQ_FOREACH(np, &left_lane_car_queue_head, entries) {
        i++;
    }
    return i;
}


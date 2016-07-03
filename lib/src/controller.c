//
// Created by School on 7/2/16.
//
#include <lib/include/model.h>
#include "controller.h"

#define TRAFFIC_SPEED_MS 500
int flag_person = SLEEPING;
int direction_to_let_through = LEFT;

//int left_lane_has_car = 0;
//int right_lane_has_car = 0;

//    When a car arrives, there is an 80% chance another car is following it, but once no car comes, there is a 20 second delay before any new car will come.

//    // fill right
//    produce_right();
//    move_right_cars();
//
//    // fill left
//    product_left();
//    move_left_(cars);



int calculate_median(car pCar, direction d) {
    int status = -1;

    if (direction_to_let_through != RIGHT && pCar != NULL) {
        status = PRINT_ACCIDENT;
    } else if (direction_to_let_through != LEFT && pCar != NULL) {
        status = PRINT_ACCIDENT;
    } else if (d == RIGHT) {
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
    } else if (d == LEFT) {
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


pthread_mutex_t produce_left_lane_traffic_lock; // for use when moving cars

void *produce_left_lane_traffic_thread(void *ptr) {
    while (TRUE) {
        pthread_mutex_lock(&produce_left_lane_traffic_lock);

        signed char add_car = FALSE;

        int r = rand() % 10;

        if (lane_has_car(LEFT) && r < 8) {
            // 80% chance of adding car
            add_car = TRUE;
        } else if (!lane_has_car(LEFT)) {
            add_car = TRUE;
        }

        if (car_vin % 10 == 0
            || add_car == FALSE) {
            sleep_ms(5000);
        }
        if (!lane_has_car(LEFT)) {
//            sleep_ms(20000);
        }

        if (add_car) {
            add_car_to_queue(LEFT);
        }

        pthread_mutex_unlock(&produce_left_lane_traffic_lock);

        sleep_ms(TRAFFIC_SPEED_MS);
    }
    return NULL;
}

pthread_mutex_t produce_right_lane_traffic_lock; // for use when moving cars

void *produce_right_lane_traffic_thread(void *ptr) {
    while (TRUE) {
        pthread_mutex_lock(&produce_right_lane_traffic_lock);

        signed char add_car = FALSE;

        int r = rand() % 10;

        if (lane_has_car(RIGHT) && r < 8) {
            add_car = TRUE;
        } else if (!lane_has_car(RIGHT)) {
            add_car = TRUE;
        }

        if (car_vin % 10 == 0
            || add_car == FALSE) {
            sleep_ms(5000);
        }

        if (!lane_has_car(RIGHT)) {
//            sleep_ms(20000);
        }

        if (add_car) {
            add_car_to_queue(RIGHT);
        }

        pthread_mutex_unlock(&produce_right_lane_traffic_lock);
        sleep_ms(TRAFFIC_SPEED_MS);
    }
    return NULL;
}


void move_cars() {
    // spawn threads();

}


pthread_mutex_t move_right_traffic_lock; // for use when moving cars


//int next_position(direction i){
//    switch (i) {
//        case RIGHT:
//            pCar->position--;
//            break;
//        case LEFT:
//            pCar->position++;
//            break;
//        default:
//            exit(EXIT_FAILURE);
//    }
//}

void move_car(car pCar, direction i) {
    switch (i) {
        case RIGHT:
            pCar->position--;
            break;
        case LEFT:
            pCar->position++;
            break;
        default:
            exit(EXIT_FAILURE);
    }
}

int next_postion(int position, direction i) {
    int result;
    switch (i) {
        case RIGHT:
            result = position - 1;
            break;
        case LEFT:
            result = position + 1;
            break;
        default:
            exit(EXIT_FAILURE);
    }

    return result;
}

bool before(int position, int end_position, direction d) {
    bool result;
    switch (d) {
        case RIGHT:
            result = position > end_position;
            break;
        case LEFT:
            result = end_position > position;
            break;
        default:
            exit(EXIT_FAILURE);
    }

    return result;
}

void *move_right_lane_thread(void *ptr) {
    while (true) {
        pthread_mutex_lock(&move_right_traffic_lock);

        // go through each car in queue
//        int first_position = right_car_queue_head_ptr->stqh_first->c->position;
//        int previous_position = first_position;

        struct car_tail_queue *current = STAILQ_FIRST(right_car_queue_head_ptr);
        struct car_tail_queue *previous = NULL;

        if (current != NULL) {

            // handle the first car
            if (current->c->position == OFF_ROAD) {
                current->c->position = START_OF_RIGHT_LANE;
                right_lane[current->c->position] = current->c;
            } else if (next_postion(current->c->position, RIGHT)
                       == MIDDLE && direction_to_let_through != RIGHT) {
                // if next is median check to see that I can move
            } else if (before(current->c->position, END_OF_RIGHT_LANE, RIGHT)) {
                right_lane[current->c->position] = NULL;
                move_car(current->c, RIGHT);
                right_lane[current->c->position] = current->c;
            } else if (current->c->position == END_OF_RIGHT_LANE) {
                right_lane[current->c->position] = NULL;
                remove_car_from_queue(RIGHT);
            }

            while (current != NULL) {

                previous = current;

                current = current->entries.stqe_next;

                if (current == NULL) {
                    break;
                } else if (current->c->position == OFF_ROAD && previous->c->position == START_OF_RIGHT_LANE) {
                    break;
                }

                if (previous->c->position == next_postion(current->c->position, RIGHT)
                    // if next is median check to see that I can move
                    || (next_postion(current->c->position, RIGHT) == MIDDLE && direction_to_let_through != RIGHT)) {
                    // no room to move this car
                } else {
                    if (current->c->position == OFF_ROAD) {
                        current->c->position = START_OF_RIGHT_LANE;
                        right_lane[current->c->position] = current->c;
                    } else {

                        // if next is median check to see that I can move
                        // if I can't move then I stop
                        // if I can move then I move to the next
                        right_lane[current->c->position] = NULL;
                        move_car(current->c, RIGHT);
                        right_lane[current->c->position] = current->c;
                    }
                }
            }
        }

        pthread_mutex_unlock(&move_right_traffic_lock);
        sleep_ms(TRAFFIC_SPEED_MS);
    }
    return NULL;
}

pthread_mutex_t move_left_traffic_lock; // for use when moving cars

void *move_left_lane_thread(void *ptr) {
    while (true) {
        pthread_mutex_lock(&move_left_traffic_lock);
        // go through each car in queue
//        int first_position = right_car_queue_head_ptr->stqh_first->c->position;
//        int previous_position = first_position;

        struct car_tail_queue *current = STAILQ_FIRST(left_car_queue_head_ptr);
        struct car_tail_queue *previous = NULL;

        if (current != NULL) {

            // handle the first car
            if (current->c->position == OFF_ROAD) {
                current->c->position = START_OF_LEFT_LANE;
                left_lane[current->c->position] = current->c;
            } else if (next_postion(current->c->position, LEFT)
                       == MIDDLE && direction_to_let_through != LEFT) {
                // if next is median check to see that I can move
            } else if (before(current->c->position, END_OF_LEFT_LANE, LEFT)) {
                left_lane[current->c->position] = NULL;
                move_car(current->c, LEFT);
                left_lane[current->c->position] = current->c;
            } else if (current->c->position == END_OF_LEFT_LANE) {
                left_lane[current->c->position] = NULL;
                remove_car_from_queue(LEFT);
            }

            while (current != NULL) {

                previous = current;

                current = current->entries.stqe_next;

                if (current == NULL) {
                    break;
                } else if (current->c->position == OFF_ROAD && previous->c->position == START_OF_LEFT_LANE) {
                    break;
                }

                if (previous->c->position == next_postion(current->c->position, LEFT)
                    // if next is median check to see that I can move
                    || (next_postion(current->c->position, LEFT) == MIDDLE && direction_to_let_through != LEFT)) {
                    // no room to move this car
                } else {
                    if (current->c->position == OFF_ROAD) {
                        current->c->position = START_OF_LEFT_LANE;
                        left_lane[current->c->position] = current->c;
                    } else {
                        left_lane[current->c->position] = NULL;
                        move_car(current->c, LEFT);
                        left_lane[current->c->position] = current->c;
                    }
                }
            }
        }
        pthread_mutex_unlock(&move_left_traffic_lock);
        sleep_ms(TRAFFIC_SPEED_MS);

    }
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

// TODO: create producers to fill up left and right roads from the traffic threads
// from prompt: Treat the road as two queues, and have a producer for each direction putting cars into the queues at the
// appropriate times.



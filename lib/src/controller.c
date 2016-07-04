//
// Created by Betsalel Williamson on 7/2/16.
//

#include "controller.h"

int flag_person = SLEEPING;
direction direction_to_let_through = LEFT;

int calculate_median(car pCar, direction d) {
    int status = -1;
    // the lane can either be

    // blocked awake
    // blocked sleeping
    // open
    // accident - this isn't used because we assume that there won't be accidents

    if (d == RIGHT) {
        if (pCar != NULL) {
            status = PRINT_RIGHT_CAR;
        } else {
            if (direction_to_let_through == RIGHT) {
                status = PRINT_ROAD;
            } else {
                if (flag_person == SLEEPING) {
                    status = PRINT_TRAFFIC_CONE_SLEEPING;
                } else if (flag_person == AWAKE) {
                    status = PRINT_TRAFFIC_CONE_AWAKE;
                }
            }
        }
    } else if (d == LEFT) {
        if (pCar != NULL) {
            status = PRINT_LEFT_CAR;
        } else {
            if (direction_to_let_through == LEFT) {
                status = PRINT_ROAD;
            } else {
                if (flag_person == SLEEPING) {
                    status = PRINT_TRAFFIC_CONE_SLEEPING;
                } else if (flag_person == AWAKE) {
                    status = PRINT_TRAFFIC_CONE_AWAKE;
                }
            }
        }
    } else {
        exit(EXIT_FAILURE);
    }

    return status;
}

//void left_producer() {
//    while (true) {
//        item = produceItem();
//
//        down(&left_empty_count);
//        down(&left_buffer_mutex);
//
//        putItemIntoBuffer(item);
//
//        up(&left_buffer_mutex);
//        up(&left_fill_count);
//    }
//}

void *produce_left_lane_traffic_thread(void *ptr) {
    while (TRUE) {
        signed char add_car = FALSE;

        int r = rand() % 10;

        if (lane_has_car(LEFT) && r < 8) {
            // 80% chance of adding car
            add_car = TRUE;
        } else if (!lane_has_car(LEFT)) {
            add_car = TRUE;
        }

        if (add_car == FALSE || cars_in_queue(LEFT) > 10) {
            sleep_ms(TRAFFIC_SPEED_MS * 10);
        }

        if (add_car) {
            down(&left_empty_count);
            down(&left_buffer_mutex);

            add_car_to_queue(LEFT);

            up(&left_buffer_mutex);
            up(&left_fill_count);
        }
        sleep_ms(TRAFFIC_SPEED_MS);
    }
    return NULL;
}

//void right_producer() {
//    while (true) {
//        item = produceItem();
//
//        down(&right_empty_count);
//        down(&right_buffer_mutex);
//
//        putItemIntoBuffer(item);
//
//        up(&right_buffer_mutex);
//        up(&right_fill_count);
//    }
//}

void *produce_right_lane_traffic_thread(void *ptr) {
    while (TRUE) {
        signed char add_car = FALSE;

        int r = rand() % 10;

        if (lane_has_car(RIGHT) && r < 8) {
            add_car = TRUE;
        } else if (!lane_has_car(RIGHT)) {
            add_car = TRUE;
        }

        if (add_car == FALSE || cars_in_queue(RIGHT) > 10) {
            sleep_ms(TRAFFIC_SPEED_MS * 10);
        }

        if (add_car) {
            down(&right_empty_count);
            down(&right_buffer_mutex);

            add_car_to_queue(RIGHT);

            up(&right_buffer_mutex);
            up(&right_fill_count);
        }
        sleep_ms(TRAFFIC_SPEED_MS);
    }
    return NULL;
}

void wake_flag_person(direction d) {
    flag_person = AWAKE;
}

int wait_time_of_car_before_median(direction d) {
    int result = 0;
    int i;
    switch (d) {
        case RIGHT:
            if (right_lane[MIDDLE + 1] == NULL) {
                result = 0;
            } else {
                result = right_lane[MIDDLE + 1]->wait_time;
            }
            break;
        case LEFT:
            if (left_lane[MIDDLE - 1] == NULL) {
                result = 0;
            } else {
                result = left_lane[MIDDLE - 1]->wait_time;
            }
            break;
        default:
            exit(EXIT_FAILURE);
    }

    return result;
}

int cars_before_median(direction d) {
    int result = 0;
    int i;
    switch (d) {
        case RIGHT:
            for (i = START_OF_RIGHT_LANE; i >= MIDDLE; --i) {
                if (right_lane[i] != NULL) {
                    result++;
                }
            }
            break;
        case LEFT:
            for (i = START_OF_LEFT_LANE; i < MIDDLE; ++i) {
                if (left_lane[i] != NULL) {
                    result++;
                }
            }
            break;
        default:
            exit(EXIT_FAILURE);
    }

    return result;
}

// When a car arrives at either end, the flagperson will allow traffic from that side to continue to flow, until
// there are no more cars, or until there are 10 cars lined up on the opposing side, at which time they will be
// allowed to pass.
void *flag_person_thread(void *ptr) {
    while (TRUE) {
        if (flag_person == AWAKE) {

            if (!lane_has_car(RIGHT) || !lane_has_car(LEFT)) {
                flag_person = SLEEPING;
                // sleep here
                continue;
            }

            // until there are no more cars,
            // or until there are 10 cars lined up on the opposing side, at which time they will be allowed to pass.
            switch (direction_to_let_through) {
                case LEFT:
                    if (cars_in_queue(RIGHT) >= 10
                        //|| (wait_time_of_car_before_median(RIGHT) > 10)
                        || cars_before_median(LEFT) == 0) {
                        // switch directions
                        direction_to_let_through = NONE;
                        sleep_ms(TRAFFIC_SPEED_MS * 2);
                        direction_to_let_through = RIGHT;
                    }
                    break;
                case RIGHT:
                    if (cars_in_queue(LEFT) >= 10
                        //|| (wait_time_of_car_before_median(LEFT) > 10)
                        || cars_before_median(RIGHT) == 0) {
                        // switch directions
                        direction_to_let_through = NONE;
                        sleep_ms(TRAFFIC_SPEED_MS * 2);
                        direction_to_let_through = LEFT;
                    }
                    break;
                default:
                    // sleeping
                    break;
            }
        }
        sleep_ms(TRAFFIC_SPEED_MS * 2);
    }
    return NULL;
}

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

// from prompt: Treat the road as two queues, and have a producer for each direction putting cars into the queues at the
// appropriate times. this is the move right lane  and move left lane threads

//void left_consumer() {
//    while (true) {
//        down(&left_fill_count);
//        down(&left_buffer_mutex);
//
//        item = removeItemFromBuffer();
//
//        up(&left_buffer_mutex);
//        up(&left_empty_count);
//
//        consumeItem(item);
//    }
//}

void *consume_left_lane_traffic_thread(void *ptr) {
    while (true) {
        // go through each car in queue
        down(&left_fill_count);
        down(&left_buffer_mutex);

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
                wake_flag_person(LEFT);
                current->c->wait_time++;
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

                if (previous->c->position == next_postion(current->c->position, LEFT)) {
                    // no room to move this car
                    current->c->wait_time++;
                } else if (next_postion(current->c->position, LEFT) == MIDDLE && direction_to_let_through != LEFT) {
                    // if next is median check to see that I can move
                    wake_flag_person(LEFT);
                    current->c->wait_time++;
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

        up(&left_buffer_mutex);
        up(&left_empty_count);

        sleep_ms(TRAFFIC_SPEED_MS);
    }
}

//void right_consumer() {
//    while (true) {
//        down(&right_fill_count);
//        down(&right_buffer_mutex);
//
//        item = removeItemFromBuffer();
//
//        up(&right_buffer_mutex);
//        up(&right_empty_count);
//
//        consumeItem(item);
//    }
//}

void *consume_right_lane_traffic_thread(void *ptr) {
    while (true) {

        down(&right_fill_count);
        down(&right_buffer_mutex);

        // go through each car in queue
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
                wake_flag_person(RIGHT);
                current->c->wait_time++;
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

                if (previous->c->position == next_postion(current->c->position, RIGHT)) {
                    // no room to move this car
                    current->c->wait_time++;
                } else if (next_postion(current->c->position, RIGHT) == MIDDLE && direction_to_let_through != RIGHT) {
                    // if next is median
                    wake_flag_person(RIGHT);
                    current->c->wait_time++;
                } else {
                    if (current->c->position == OFF_ROAD) {
                        current->c->position = START_OF_RIGHT_LANE;
                        right_lane[current->c->position] = current->c;
                    } else {
                        // if I can move then I move to the next
                        right_lane[current->c->position] = NULL;
                        move_car(current->c, RIGHT);
                        right_lane[current->c->position] = current->c;
                    }
                }
            }
        }
        up(&right_buffer_mutex);
        up(&right_empty_count);

        sleep_ms(TRAFFIC_SPEED_MS);
    }
    return NULL;
}




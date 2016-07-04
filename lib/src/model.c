//
// Created by Betsalel Williamson on 7/3/16.
//
#include "model.h"

int car_vin = 1;

// goes from 0 to int
int my_clock;

struct left_lane_car_queue left_car_queue_head = STAILQ_HEAD_INITIALIZER(left_car_queue_head);
struct right_lane_car_queue right_car_queue_head = STAILQ_HEAD_INITIALIZER(right_car_queue_head);

struct left_lane_car_queue *left_car_queue_head_ptr = &left_car_queue_head;
struct right_lane_car_queue *right_car_queue_head_ptr = &right_car_queue_head;

void init_queues() {
    STAILQ_INIT(left_car_queue_head_ptr);    /* Initialize the queue. */
    STAILQ_INIT(right_car_queue_head_ptr);   /* Initialize the queue. */
}

void destruct_queues() {

    /* Faster TailQ Deletion. */
    n1 = STAILQ_FIRST(left_car_queue_head_ptr);
    while (n1 != NULL) {
        n2 = STAILQ_NEXT(n1, entries);
        free(n1->c);
        free(n1);
        n1 = n2;
    }
    STAILQ_INIT(left_car_queue_head_ptr);

    n1 = STAILQ_FIRST(right_car_queue_head_ptr);
    while (n1 != NULL) {
        n2 = STAILQ_NEXT(n1, entries);
        free(n1->c);
        free(n1);
        n1 = n2;
    }
    STAILQ_INIT(left_car_queue_head_ptr);
}

void add_car_to_queue(direction d) {
    struct car_tail_queue *my_car = malloc(sizeof(struct car_tail_queue));      /* Insert at the tail. */
    my_car->c = malloc(sizeof(car));
    my_car->c->vin_number = car_vin++;
    my_car->c->position = OFF_ROAD;
    my_car->c->lane = d;
    my_car->c->wait_time = 0;

    switch (d) {
        case LEFT:
            STAILQ_INSERT_TAIL(left_car_queue_head_ptr, my_car, entries);
            break;
        case RIGHT:
            STAILQ_INSERT_TAIL(right_car_queue_head_ptr, my_car, entries);
            break;
        default:
            exit(EXIT_FAILURE);
    }
}

int right_cars_processed = 0;
int left_cars_processed = 0;

int cars_out_of_queue(direction d) {
    int i = 0;
    switch (d) {
        case RIGHT:
            i = right_cars_processed;
            break;
        case LEFT:
            i = left_cars_processed;
            break;
        default:
            exit(EXIT_FAILURE);
    }
    return i;
}

// add to number of cars processed
void remove_car_from_queue(direction d) {
    switch (d) {
        case RIGHT:
            STAILQ_REMOVE_HEAD(right_car_queue_head_ptr, entries);
            right_cars_processed++;
            break;
        case LEFT:
            STAILQ_REMOVE_HEAD(left_car_queue_head_ptr, entries);
            left_cars_processed++;
            break;
        default:
            exit(EXIT_FAILURE);
    }
}

bool lane_has_car(direction d) {
    bool result = false;

    switch (d) {
        case RIGHT:
            result = !STAILQ_EMPTY(right_car_queue_head_ptr);
            break;
        case LEFT:
            result = !STAILQ_EMPTY(left_car_queue_head_ptr);
            break;
        default:
            exit(EXIT_FAILURE);
    }
    return result;
}

car right_lane[ROAD_LENGTH + 1]; // goes from road size to 0
car left_lane[ROAD_LENGTH + 1]; // goes from 0 to road size

void init_road(car *road) {
    for (int i = 0; i < ROAD_LENGTH; ++i) {
        road[i] = NULL;
    }
}

void init_roads() {
    init_road(right_lane);
    init_road(left_lane);
}

int cars_in_queue(direction d) {
    int i = 0;
    switch (d) {
        case RIGHT:
            STAILQ_FOREACH(np, right_car_queue_head_ptr, entries) {
                i++;
            }
            break;
        case LEFT:
            STAILQ_FOREACH(np, left_car_queue_head_ptr, entries) {
                i++;
            }
            break;
        default:
            exit(EXIT_FAILURE);
    }
    return i;
}






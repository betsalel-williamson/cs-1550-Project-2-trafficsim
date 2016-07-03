//
// Created by School on 7/3/16.
//

#ifndef MAIN_MODEL_H
#define MAIN_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/queue.h>

typedef struct Car {
    int position;
    int lane;
    int vin_number;
} *car;

struct car_tail_queue {
    car c;
    STAILQ_ENTRY(car_tail_queue) entries;    /* Tail queue. */
} *n1, *n2, *n3, *np;

STAILQ_HEAD(left_lane_car_queue, car_tail_queue);
extern struct left_lane_car_queue *left_car_queue_head_ptr;

STAILQ_HEAD(right_lane_car_queue, car_tail_queue);
extern struct right_lane_car_queue *right_car_queue_head_ptr;

typedef int direction;

int cars_in_queue(direction d);

int cars_out_of_queue(direction d);

void add_car_to_queue(direction d);

void remove_car_from_queue(direction d);

#define ROAD_LENGTH 21
#define MIDDLE 10

#define START_OF_LEFT_LANE 0
#define END_OF_LEFT_LANE 20

#define START_OF_RIGHT_LANE ROAD_LENGTH
#define END_OF_RIGHT_LANE 0

#define OFF_ROAD -1

extern car right_lane[]; // goes from 20 to 0 to, staging area of 21
extern car left_lane[]; // goes from 0 to 20, staging area of 21

bool lane_has_car(direction d);

extern int my_clock;

#define RIGHT 0
#define LEFT 1

void init_queues();

void destruct_queues();

void init_roads();

#ifdef __cplusplus
}
#endif

#endif //MAIN_MODEL_H

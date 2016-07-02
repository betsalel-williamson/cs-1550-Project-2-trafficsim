//
// Created by School on 7/2/16.
//

#ifndef MAIN_TRAFFIC_RULES_H
#define MAIN_TRAFFIC_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/file.h>
#include "library.h"
#include <sys/queue.h>
#include <stdbool.h>
#include <stdlib.h>
#include <printf.h>
#include "draw.h"

#define ROAD_LENGTH 21
#define MIDDLE 10

#define START_OF_LEFT_ROAD 0
#define START_OF_RIGHT_ROAD ROAD_LENGTH-1

#define RIGHT 0
#define LEFT 1

#define SLEEPING false

#define LET_RIGHT_TRAFFIC_THROUGH 0
#define LET_LEFT_TRAFFIC_THROUGH 1

void init_queues();

void destruct_queues();

void init_roads();

typedef struct Car {
//    int direction;
    int vin_number;
    STAILQ_ENTRY(Car) entries;    /* Tail queue. */

} *car, *np;

extern car right_road[ROAD_LENGTH]; // goes from road size to 0
extern car left_road[ROAD_LENGTH]; // goes from 0 to road size
extern int car_vin;
extern int flag_person;
extern int direction_to_let_through;

extern pthread_mutex_t right_lock;
extern pthread_mutex_t left_lock;
extern pthread_mutex_t move_left_lock;
extern pthread_mutex_t move_right_lock;

int calculate_median(car pCar, int direction);

void *right_lane_traffic_thread(void *ptr);

void *left_lane_traffic_thread(void *ptr);

void *consume_cars_thread(void *ptr);

int left_cars_in_queue();

int right_cars_in_queue();

int left_cars_out_of_queue();

int right_cars_out_of_queue();

#ifdef __cplusplus
}
#endif

#endif //MAIN_TRAFFIC_RULES_H

//
// Created by Betsalel Williamson on 7/2/16.
//

#ifndef MAIN_TRAFFIC_RULES_H
#define MAIN_TRAFFIC_RULES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/file.h>
#include <sys/queue.h>
#include <stdbool.h>
#include <stdlib.h>
#include <printf.h>
#include <mach/boolean.h>

#include "view.h"
#include "model.h"
#include "library.h"

#define TRAFFIC_SPEED_MS 2000

#define SLEEPING false
#define AWAKE true

extern int car_vin;
extern int flag_person;
extern int direction_to_let_through;

int calculate_median(car pCar, int direction);

int cars_before_median(direction d);

void *produce_right_lane_traffic_thread(void *ptr);

void *produce_left_lane_traffic_thread(void *ptr);

void *flag_person_thread(void *ptr);

void *consume_right_lane_traffic_thread(void *ptr);

void *consume_left_lane_traffic_thread(void *ptr);

int next_postion(int position, direction i);

#ifdef __cplusplus
}
#endif

#endif //MAIN_TRAFFIC_RULES_H

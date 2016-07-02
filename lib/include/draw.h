//
// Created by School on 7/2/16.
//

#ifndef MAIN_DRAW_H
#define MAIN_DRAW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "traffic_rules.h"
#include <mach/boolean.h>
#include <printf.h>
#include "library.h"

#define PRINT_ACCIDENT 0
#define PRINT_RIGHT_CAR 1
#define PRINT_LEFT_CAR 2
#define PRINT_ROAD 3
#define PRINT_TRAFFIC_CONE_SLEEPING 4
#define PRINT_TRAFFIC_CONE_AWAKE 5

const extern char *left_car_s;
const extern char *right_car_s;
const extern char *road_s;
const extern char *accident_s;
const extern char *empty_s;
const extern char *error_s;
const extern char *traffic_cones_active_s;
const extern char *traffic_cones_sleeping_s;

void draw();

void clear_screen();

#ifdef __cplusplus
}
#endif

#endif //MAIN_DRAW_H

//
// Created by School on 7/2/16.
//

#ifndef MAIN_DRAW_H
#define MAIN_DRAW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lib/include/traffic_rules.h>
#include <printf.h>
#include "library.h"

#define PRINT_ACCIDENT 0
#define PRINT_RIGHT_CAR 1
#define PRINT_LEFT_CAR 2
#define PRINT_ROAD 3
#define PRINT_TRAFFIC_CONE_SLEEPING 4
#define PRINT_TRAFFIC_CONE_AWAKE 5
extern char *left_car_s;
extern char *right_car_s;
extern char *road_s;
extern char *accident_s;
extern char *empty_s;
extern char *error_s;
extern char *traffic_cones_active_s;
extern char *traffic_cones_sleeping_s;
void draw();
void draw_traffic();
void clear_screen();
void print_instructions();

#ifdef __cplusplus
}
#endif

#endif //MAIN_DRAW_H

//
// Created by School on 7/2/16.
//

#ifndef MAIN_DRAW_H
#define MAIN_DRAW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "controller.h"
#include <mach/boolean.h>
#include <printf.h>
#include "library.h"

#define PRINT_ACCIDENT 0
#define PRINT_RIGHT_CAR 1
#define PRINT_LEFT_CAR 2
#define PRINT_ROAD 3
#define PRINT_TRAFFIC_CONE_SLEEPING 4
#define PRINT_TRAFFIC_CONE_AWAKE 5

void draw();

void clear_screen();

#ifdef __cplusplus
}
#endif

#endif //MAIN_DRAW_H

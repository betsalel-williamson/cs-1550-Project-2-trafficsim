//
// Created by School on 7/2/16.
//

#include <mach/boolean.h>
#include "draw.h"

char *left_car_s = "[*>";
char *right_car_s = "<*]";
char *road_s = "___";
char *accident_s = "^!^";
char *empty_s = "   ";
char *error_s = "ERR";
char *traffic_cones_active_s = "^A^";
char *traffic_cones_sleeping_s = "^S^";

void clear_screen() {
    const char msg[] = "\033[2J";
    syscall(4, STDOUT_FILENO, msg, sizeof(msg) - 1);
}


void print_instructions() {
    // print how to quit
    printf("\n");
    printf("Quit? Enter 'q'.\n");
    printf("\n");
    // print what's going on
    printf("Instructions:\n");
    printf("Hit enter to advance the clock by 2 seconds.\n");
    printf("This is a car coming from the right '%s'\n", right_car_s);
    printf("This is a car coming from the left '%s'\n", left_car_s);
    printf("\n");

}


void print_clock() {
    printf("Time elapsed is: %2.2d:%2.2d\n\n", my_clock / 60, my_clock % 60);
}

void draw() {
    while (TRUE) {
        clear_screen();

        print_instructions();

        print_clock();

        draw_traffic();

        increment_clock();

        sleep_ms(1000);
    }
}


void draw_car_numbers(car *road) {// draw left numbers
    int l;
    for (l = 0; l < ROAD_LENGTH; ++l) {

        if (road[l] != NULL) {
            printf("%2.2d ", road[l]->vin_number);
        } else if (road[l] == NULL) {
            printf("%s", empty_s);
        }
        else {
            printf("%s", error_s);
        }
    }
    printf("\n");
}

void draw_road(car * road, int direction){
    int j;
    for (j = 0; j < ROAD_LENGTH; ++j) {
        if (j == MIDDLE) {
            switch (calculate_median(road[j], RIGHT)) {
                case PRINT_ACCIDENT:
                    printf("%s", accident_s);
                    break;
                case PRINT_TRAFFIC_CONE_AWAKE:
                    printf("%s", traffic_cones_active_s);
                    break;
                case PRINT_TRAFFIC_CONE_SLEEPING:
                    printf("%s", traffic_cones_sleeping_s);
                    break;
                case PRINT_ROAD:
                    printf("%s", road_s);
                    break;
                case PRINT_LEFT_CAR:
                    printf("%s", left_car_s);
                    break;
                case PRINT_RIGHT_CAR:
                    printf("%s", right_car_s);
                    break;
                default:
                    break;
            };
        } else if (road[j] != NULL) {
            printf("%s", (direction == RIGHT ? right_car_s : (direction == LEFT ? left_car_s : "ERR")));
        } else if (road[j] == NULL) {
            printf("%s", road_s);
        }
        else {
            printf("%s", error_s);
        }
    }
    printf("\n");
}

void draw_traffic() {
    // draw road
    // 0-9 = left side
    // 10 = middle
    // 11 - 20 = right side

    // draw right numbers
    draw_car_numbers(right_road);

    // draw right road
    draw_road(right_road, RIGHT);

    // left road
    draw_road(left_road, LEFT);

    draw_car_numbers(left_road);
}
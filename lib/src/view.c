//
// Created by School on 7/2/16.
//

#include "view.h"

const char *left_car_s = "[*>";
const char *right_car_s = "<*]";
const char *road_s = "___";
const char *accident_s = "^!^";
const char *empty_s = "   ";
const char *error_s = "ERR";
const char *traffic_cones_active_s = "^A^";
const char *traffic_cones_sleeping_s = "^S^";

void clear_screen() {
    const char msg[] = "\033[2J";
    syscall(4, STDOUT_FILENO, msg, sizeof(msg) - 1);
}


void print_instructions() {
    // print how to quit
    printf("\n");
    printf("Quit? Enter '^C'.\n");
    printf("\n");
    // print what's going on
    printf("Instructions:\n");
    printf("This is a car coming from the right '%s'\n", right_car_s);
    printf("This is a car coming from the left '%s'\n", left_car_s);
    printf("This is a the flagperson sleeping '%s'\n", traffic_cones_sleeping_s);
    printf("This is a car flagperson letting people through '%s'\n", traffic_cones_active_s);
    printf("\n");

}

void print_clock() {
    printf("Time elapsed is: %2.2d:%2.2d\n\n", my_clock / 60, my_clock % 60);
}

void print_number_of_cars_in_queue() {
    printf("Total cars in queue:\nFrom right: %2.2d | From left: %2.2d\n\n", cars_in_queue(RIGHT),
           cars_in_queue(LEFT));
}

void print_number_of_cars_out_of_queue() {
    printf("Total cars processed:\nFrom right: %2.2d | From left: %2.2d\n\n", cars_out_of_queue(RIGHT),
           cars_out_of_queue(LEFT));
}

void print_number_of_cars_before_median() {
    printf("Total cars before median:\nFrom right: %2.2d | From left: %2.2d\n\n", cars_before_median(RIGHT),
           cars_before_median(LEFT));

}

void draw_car_numbers(car *road) {// draw left numbers
    int l;
    for (l = 0; l < ROAD_LENGTH; ++l) {
        if (road[l] != NULL) {
            printf("%2.2d ", road[l]->vin_number % 100);
        } else if (road[l] == NULL) {
            printf("%s", empty_s);
        }
        else {
            printf("%s", error_s);
        }
    }
    printf("\n");
}

void draw_road(car *road, int direction) {
    int j;
    for (j = 0; j < ROAD_LENGTH; ++j) {
        if (road[j] == NULL && j != MIDDLE) {
            printf("%s", road_s);
        } else if (road[j] != NULL && j != MIDDLE) {
            switch (direction) {
                case RIGHT:
                    printf("%s", right_car_s);
                    break;
                case LEFT:
                    printf("%s", left_car_s);
                    break;
                default:
                    printf("%s", error_s);
                    break;
            }
        } else if (j == MIDDLE) {
            switch (calculate_median(road[j], direction)) {
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
                    printf("%s", error_s);
                    break;
            };
        } else {
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
    draw_car_numbers(right_lane);

    // draw right road
    draw_road(right_lane, RIGHT);

    // left road
    draw_road(left_lane, LEFT);

    draw_car_numbers(left_lane);
}

void draw() {
    clear_screen();

    print_instructions();

    print_clock();

    print_number_of_cars_in_queue();

    draw_traffic();

    print_number_of_cars_out_of_queue();

    print_number_of_cars_before_median();
}



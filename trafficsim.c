//
// Created by Betsalel Williamson on 6/4/16.
//

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/queue.h>
#include <ctype.h>
#include <linux/unistd.h>
#include <unistd.h>

#define TRUE true
#define FALSE 0

void sleep_ms(long ms) {
    struct timespec request = {0};

    time_t sec = (int) (ms / 1000);
    ms = ms - (sec * 1000);

    request.tv_sec = sec;
    request.tv_nsec = ms * 1000000L;

    while (nanosleep(&request, &request) == -1 && errno == EINTR)
        continue;
}

int my_clock;


void *increment_clock_thread(void *ptr) {

    while (TRUE) {
        my_clock += 1;
        sleep_ms(1000);
    }
}

//******* begin model ******

#define __NR_cs1550_down    325
#define __NR_cs1550_up        326

#define down(sem) syscall(__NR_cs1550_down, sem);
#define up(sem) syscall(__NR_cs1550_up, sem);

typedef struct Car {
    int position;
    int wait_time;
    int lane;
    int vin_number;
} *car;

struct car_tail_queue {
    car c;
    STAILQ_ENTRY(car_tail_queue) entries;    /* Tail queue. */
} *n1, *n2, *n3, *np;

STAILQ_HEAD(left_lane_car_queue, car_tail_queue);
struct left_lane_car_queue *left_car_queue_head_ptr;

STAILQ_HEAD(right_lane_car_queue, car_tail_queue);
struct right_lane_car_queue *right_car_queue_head_ptr;

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

//extern car right_lane[]; // goes from 20 to 0 to, staging area of 21
//extern car left_lane[]; // goes from 0 to 20, staging area of 21

bool lane_has_car(direction d);

//extern int my_clock;

#define RIGHT 0
#define LEFT 1
#define NONE 2

void init_queues();

void destruct_queues();

void init_roads();

int car_vin = 1;
// goes from 0 to int

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
    int i;
    for (i = 0; i < ROAD_LENGTH; ++i) {
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

#define BUFFER_SIZE 10

// similar to "semaphore buffer_mutex = 1", but different (see notes below)
semaphore left_buffer_mutex = {1, 0, NULL};
semaphore left_fill_count = {0, 0, NULL};
semaphore left_empty_count = {BUFFER_SIZE, 0, NULL};

// similar to "semaphore buffer_mutex = 1", but different (see notes below)
semaphore right_buffer_mutex = {1, 0, NULL};
semaphore right_fill_count = {0, 0, NULL};
semaphore right_empty_count = {BUFFER_SIZE, 0, NULL};

//******* end model ******

//******* begin view ******
#define PRINT_ACCIDENT 0
#define PRINT_RIGHT_CAR 1
#define PRINT_LEFT_CAR 2
#define PRINT_ROAD 3
#define PRINT_TRAFFIC_CONE_SLEEPING 4
#define PRINT_TRAFFIC_CONE_AWAKE 5

void draw();

void clear_screen();


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



//******* end view ******

//******* begin controller ******

#define TRAFFIC_SPEED_MS 2000

#define SLEEPING false
#define AWAKE true

int car_vin;
int flag_person;
int direction_to_let_through;

int calculate_median(car pCar, int direction);

int cars_before_median(direction d);

void *produce_right_lane_traffic_thread(void *ptr);

void *produce_left_lane_traffic_thread(void *ptr);

void *flag_person_thread(void *ptr);

void *consume_right_lane_traffic_thread(void *ptr);

void *consume_left_lane_traffic_thread(void *ptr);

int next_postion(int position, direction i);

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




//******* end controller ******




/**
 * Handle quit to correctly exit and restore state on CTR-C.
 */


#define REFRESH_RATE 100

void *draw_thread(void *ptr) {
    while (true) {
        draw();
        sleep_ms(REFRESH_RATE);
    }
}

void init_simulator() {

    /* initialize random seed: */
    srand((unsigned int) time(NULL));

    init_roads();
    init_queues();

    /* Create independent threads each of which will execute function */
    pthread_t produce_right_thread,
            produce_left_thread,
            consume_right_thread,
            consume_left_thread,
            consume_flag_person_thread,
            clock_thread;

    int iret1, iret2, iret3, iret4, iret5, iret6;

    iret1 = pthread_create(&produce_right_thread, NULL, produce_right_lane_traffic_thread, NULL);
    if (iret1) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
        exit(EXIT_FAILURE);
    }

    iret2 = pthread_create(&produce_left_thread, NULL, produce_left_lane_traffic_thread, NULL);
    if (iret2) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }

    iret3 = pthread_create(&consume_right_thread, NULL, consume_right_lane_traffic_thread, NULL);
    if (iret3) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }
    iret4 = pthread_create(&consume_left_thread, NULL, consume_left_lane_traffic_thread, NULL);
    if (iret4) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }

    iret5 = pthread_create(&consume_flag_person_thread, NULL, flag_person_thread, NULL);
    if (iret5) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }

    iret6 = pthread_create(&clock_thread, NULL, increment_clock_thread, NULL);
    if (iret6) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }
}


void destruct_simulator() {
    destruct_queues();
}

void sig_handler(int signo) {

    destruct_simulator();

    if (signo == SIGINT) {
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

/***
 *
 * Create a program, trafficsim, which runs the simulation.
 *
 * Treat the road as two queues, and have a producer for each direction putting cars into the queues at the appropriate
 * times.
 *
 * Have a consumer (flagperson) that allows cars from one direction to pass through the work area as described above.
 *
 * To get an 80% chance of something, you can generate a random vin_number modulo 10, and see if its value is less than 8.
 * Its like flipping an unfair coin.
 *
 * Use the syscall nanosleep() or sleep() to pause your processes
 *
 * Make sure that your output shows all of the necessary events. You can sequentially vin_number each car and say the
 * direction it is heading. Display when the flagperson goes to sleep and is woken back up.
 *
 * */

    init_simulator();

    // spawn draw // capture user input to speed up or slow down traffic?

    pthread_t draw_pthread;

    int iret1;

    iret1 = pthread_create(&draw_pthread, NULL, draw_thread, NULL);

    if (iret1) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", iret1);
        exit(EXIT_FAILURE);
    }

    char mystring[100];
    char quit = FALSE;
    while (quit == FALSE) {

        fgets(mystring, 100, stdin);


        if (toupper(mystring[0]) != 'Q') {
            quit = FALSE;
        } else {
            quit = TRUE;
        }
    }

    destruct_simulator();

    return 0;
}
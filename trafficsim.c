//
// Created by Betsalel Williamson on 6/4/16.
//

#include <signal.h>
#include <mach/boolean.h>
#include "library.h"

/**
 * Handle quit to correctly exit and restore state on CTR-C.
 */
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

    while (TRUE) {
        draw();

        sleep_ms(500);
    }

//    destruct_simulator();

//    return 0;
}
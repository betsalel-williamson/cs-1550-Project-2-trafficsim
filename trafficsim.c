//
// Created by Betsalel Williamson on 6/4/16.
//

#include <signal.h>
#include <sys/param.h>
#include <mach/boolean.h>
#include <stdio.h>
#include <ctype.h>
#include "library.h"
#include "draw.h"

/**
 * Handle quit to correctly exit and restore state on CTR-C.
 */
void sig_handler(int signo) {

    foo();

    if (signo == SIGINT) {
        return;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

//    foo();

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

    char mystring[100];
    char quit = FALSE;
    while (quit == FALSE) {
        draw();
//        sleep_ms(2000);

//        fflush(stdin);
        fgets(mystring, 100, stdin);
//        scanf("%c", &quit);
//        scanf("%n",void);

        if (toupper(mystring[0]) != 'Q') {
            quit = FALSE;
        } else {
            quit = TRUE;
        }
    }

    destruct_simulator();

    return 0;
}
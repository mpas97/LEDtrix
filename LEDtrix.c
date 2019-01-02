//
// Created by salom on 02.01.2019.
//
#include "color.h"
#include "leds.h"
#include "tictactoe.h"
//#include "raindrops.h"

// compile: gcc LEDtrix.c -o LEDtrix -lpthread



int main(int argc, char **argv) {

    // Set up gpio pointer for direct register access
    setup_io();

    // must use INP_GPIO before we can use OUT_GPIO
    INP_GPIO(D1PIN);
    OUT_GPIO(D1PIN);
    INP_GPIO(D2PIN);
    OUT_GPIO(D2PIN);
    INP_GPIO(D3PIN);
    OUT_GPIO(D3PIN);
    INP_GPIO(CLPIN);
    OUT_GPIO(CLPIN);
    INP_GPIO(C3PIN);
    OUT_GPIO(C3PIN);

    // matrix for te leds
    struct color leds[STRIP][STRIP];

    // clear all leds, so the matrix is initialized
    clear(leds);

    if (argc == 2) {
        if (strcmp("clear", argv[1]) == 0){
            return 0;
        }
        if (strcmp("ttt", argv[1]) == 0) {
            startTictactoe(leds);
            return 0;
        }
    }

    test(leds);

    return 0;

}


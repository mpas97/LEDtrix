#include "color.h"
#include "leds.h"
#include "tictactoe.h"
#include "raindrops.h"
#include "buttons.h"

// compile: gcc LEDtrix.c -o LEDtrix -lpthread

int main(int argc, char **argv) {

    // Set up gpio pointer for direct register access
    setup_io();
    setupButtons();

    // clear all leds, so the matrix is initialized
    clear();

    //TODO: Spiel auswählen mit rechtem knopf (=blättern), mit linkem bestätigen.
    // wenn spiel verloren, zurück zu auswahl (auf diesem Spiel stehen bleiben)
    if (argc == 2) {
        if (strcmp("clear", argv[1]) == 0){
            return 0;
        }
        if (strcmp("ttt", argv[1]) == 0) {
            game = 1;
            startTictactoe();
            clear();
            return 0;
        }
        if (strcmp("rd", argv[1]) == 0) {
            startRaindrops();
            clear();
            return 0;
        }
    }

    test();

    return 0;
}


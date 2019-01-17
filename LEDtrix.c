#include "animations.h"
#include "raindrops.h"
#include "tictactoe.h"

// compile: gcc LEDtrix.c -o LEDtrix -pthread -lwiringPi

// suppress infinite loop warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#define HALF_S_IN_U 500000

#define TICTACTOE 0
#define RAINDROPS 1
#define NUM_GAMES 2

int game = 0;

void showGamePreview() {
    clear();
    switch (game) {
        case TICTACTOE:
            ttt_drawImage();
            break;
        case RAINDROPS:
            rd_drawImage();
            break;
        default:
            printf("ERROR: Game preview not found!\n");
            break;
    }
}

void startGame() {
    switch (game) {
        case TICTACTOE:
            startTictactoe();
            break;
        case RAINDROPS:
            startRaindrops();
            break;
        default:
            printf("ERROR: Game not found!\n");
            break;
    }
}

int main(int argc, char **argv) {

    // Set up gpio pointer for direct register access
    setup_io();
    setupButtons();

    // clear all leds, so the matrix is initialized
    clear();

    while (true) {
        usleep(HALF_S_IN_U);
        // left for demo mode
        if (getButtonClickLeft()) {
            demo();
        }
        // right for game mode
        if (getButtonClickRight()) {
            do {
                showGamePreview();
                do {
                    usleep(HALF_S_IN_U);
                    // right will preview next game
                    if (getButtonClickRight()) {
                        if (++game >= NUM_GAMES) game = 0;
                        showGamePreview();
                    }
                } while (!getButtonClickLeft()); // left will start game
                startGame();
                btn_l = false; // exit from game with left, choose game again
            } while (!getButtonClickRight()); // exit from game with right, choose between demo and game
        }
    }

    //TODO: write bye in the middle

    return 0;
}

#pragma clang diagnostic pop
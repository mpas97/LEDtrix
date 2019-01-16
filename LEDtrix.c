#include "color.h"
#include "leds.h"
#include "buttons.h"
#include "tictactoe.h"
#include "raindrops.h"

// compile: gcc LEDtrix.c -o LEDtrix -pthread -lwiringPi

int main(int argc, char **argv) {

    // Set up gpio pointer for direct register access
    setup_io();
    setupButtons();

    // clear all leds, so the matrix is initialized
    clear();

    int game = 0;
    bool gaming = true;

    while(gaming) {

        bool choosing = true;

        while (choosing) {
            if (btn_r) {
                if (++game > 1) game = 0;
                btn_r = false;
                switch (game) {
                    case 0:
                        ttt_drawImage();
                        break;
                    case 1:
                        rd_drawImage();
                        break;
                    default:
                        break;
                }
            }
            if (btn_l) {
                choosing = false;
                btn_l = false;
            }
        }

        switch (game) {
            case 0:
                startTictactoe();
                break;
            case 1:
                startRaindrops();
                break;
            default:
                break;
        }

        if(btn_r){
            btn_r = false;
            gaming = false;
        }
        if(btn_l) btn_l = false;
    }

    //TODO: write bye in the middle

    return 0;
}


#ifndef LEDTRIX_ANIMATIONS_H
#define LEDTRIX_ANIMATIONS_H

#include <unistd.h>
#include "buttons.h"
#include "leds.h"
#include "chars.h"

/**
 * Method for testing if x / y are displayed correct
 *
 * @param leds the matrix
 */
void demo() {
    /*clear();
    int character[5][5];
    for (int c = 1; c <= 18;) {
        for (int by = 0; by < 3; by++) {
            for (int bx = 0; bx < 3; bx++) {
                if (getChar(character, c)) {
                    for (int y = 0; y < 5; y++) {
                        for (int x = 0; x < 5; x++) {
                            if (character[y][x]) leds[x + bx * 5][y + by * 5].blue = 255;
                        }
                    }
                    updateMatrix();
                    sleep(1);
                    c++;
                }
            }
        }
        clear();
    }*/

    clear();
    for (int y = 0; y < STRIP; y++) {
        for (int x = 0; x < STRIP; x++) {
            leds[x][y].red = 15 * y;
            leds[x][y].green = 50;
            leds[x][y].blue = 255 - 15 * y - 1;
        }
    }
    updateMatrix();
    sleep(3);
    for (int y = 0; y < STRIP; y++) {
        for (int x = 0; x < STRIP; x++) {
            leds[x][y].green = 15 * x;
            leds[x][y].red = 50;
            leds[x][y].blue = 255 - 15 * x - 1;
        }
    }
    updateMatrix();
    sleep(3);

    while (1) {
        clear();
        if (getButtonClickLeft() | getButtonClickRight()) break;
        for (int y = 0; y < STRIP; y++) {
            for (int x = 0; x < STRIP; x++) {
                leds[x][y].red = 15 * y;
                leds[x][y].green = 50;
                leds[x][y].blue = 255 - 15 * y - 1;
                updateMatrix();
            }
        }
        clear();
        for (int y = 0; y < STRIP; y++) {
            for (int x = 0; x < STRIP; x++) {
                leds[x][y].green = 15 * x;
                leds[x][y].red = 50;
                leds[x][y].blue = 255 - 15 * x - 1;
                updateMatrix();
            }
        }
    }
}

#endif //LEDTRIX_ANIMATIONS_H

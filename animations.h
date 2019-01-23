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

    //fill Diagonal
    for (int i = 1; i < STRIP + 1; i++) {
        for (int j = 0; j < i; j++) {
            leds[j][i - j].red = 105 - j;
            leds[j][i - j].blue = 5 * j;
            leds[j][i - j].green = 105 - i;
            leds[STRIP - j][STRIP - i + j].red = 105 - j;
            leds[STRIP - j][STRIP - i + j].blue = 5 * j;
            leds[STRIP - j][STRIP - i + j].green = 105 - i;
        }
    }
    updateMatrix();
    sleep(10);


}

void drawSmile() {
    clear();
    color col = {28, 250, 73};


    //draw the circle
    for (int i = 0; i < 5; i++) {
        setLedsColor(i + 5, 0, col);
        setLedsColor(10 + i, i, col);
        setLedsColor(STRIP - 1, i + 5, col);
        setLedsColor(STRIP - 1 - i, 10 + i, col);
        setLedsColor(i + 5, STRIP - 1, col);
        setLedsColor(4 - i, STRIP - 1 - i, col);
        setLedsColor(0, i + 5, col);
        setLedsColor(i, 4 - i, col);
    }

    //draw Eyes
    int y = 3;
    int x1 = 5;
    int x2 = 8;
    for (int i = 0; i < 4; i++) {
        setLedsColor(x1, y + i, col);
        setLedsColor(x1 + 1, y + i, col);
        setLedsColor(x2, y + i, col);
        setLedsColor(x2 + 1, y + i, col);

    }

    for (int i = 0; i < 2; i++) {
        setLedsColor(4, 9 + i, col);
        setLedsColor(5, 9 + i, col);
        setLedsColor(6, 11 + i, col);
        setLedsColor(7, 11 + i, col);
        setLedsColor(8, 11 + i, col);
        setLedsColor(9, 9 + i, col);
        setLedsColor(10, 9 + i, col);
    }

    setLedsColor(6, 11, col);
    setLedsColor(9, 11, col);

    updateMatrix();
}

#endif //LEDTRIX_ANIMATIONS_H

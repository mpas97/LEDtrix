#ifndef LEDTRIX_ANIMATIONS_H
#define LEDTRIX_ANIMATIONS_H

#include <unistd.h>
#include "buttons.h"
#include "leds.h"
#include "chars.h"

void drawSmile();

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
        clear();
        //fill Diagonal
        for (int i = 1; i < STRIP + 1; i++) {
            for (int j = 0; j < i; j++) {
                leds[j][i - j-1].red = 105 - j;
                leds[j][i - j-1].blue = 5 * j;
                leds[j][i - j-1].green = 105 - i;
                leds[STRIP-1 - j][STRIP - i + j].red = 105 - j;
                leds[STRIP-1 - j][STRIP - i + j].blue = 5 * j;
                leds[STRIP-1 - j][STRIP - i + j].green = 105 - i;
                updateMatrix();

            }
        }
        usleep(HALF_S_IN_U);
        drawSmile();
        usleep(HALF_S_IN_U);

        for (int i = STRIP; i >= 0; i--) {
            for (int j = 0; j < i; j++) {
                leds[j][i - j-1].red = 60 - j;
                leds[j][i - j-1].blue = 10 * j;
                leds[j][i - j-1].green = 60 - i;
                leds[STRIP-1 - j][STRIP - i + j].red = 60 - j;
                leds[STRIP-1 - j][STRIP - i + j].blue = 10 * j;
                leds[STRIP-1 - j][STRIP - i + j].green = 60 - i;
                updateMatrix();
            }
        }
        usleep(HALF_S_IN_U);

        for (int i = STRIP; i >= 0; i--) {
            for (int j = 0; j < i; j++) {
                leds[STRIP - i + j][j].red = 150 - 3*j;
                leds[STRIP - i + j][j].blue = 4 * j;
                leds[STRIP - i + j][j].green = 70 - i;
                leds[i - j-1][STRIP-1 - j].red = 150 - 3*j;
                leds[i - j-1][STRIP-1 - j].blue = 4 * j;
                leds[i - j-1][STRIP-1 - j].green = 70 - i;
                updateMatrix();

            }
        }
        for (int i = 1; i < STRIP + 1; i++) {
            for (int j = 0; j < i; j++) {
                leds[i - j-1][j].red = 255 - 3*j;
                leds[i - j-1][j].blue = 3 * j;
                leds[i - j-1][j].green = 70 - i;
                leds[STRIP - i + j][STRIP-1 - j].red = 255 - 3*j;
                leds[STRIP - i + j][STRIP-1 - j].blue = 3 * j;
                leds[STRIP - i + j][STRIP-1 - j].green = 70 - i;
                updateMatrix();

            }
        }

        //clear square middle to out
        for(int j = STRIP/2; j >= 0; j--) {
            for (int i = j; i < STRIP-j; i++) {
                setLedsColor(j, i, WHITE);
                setLedsColor(i, j, WHITE);
                setLedsColor(STRIP-1-j, i, WHITE);
                setLedsColor(i, STRIP -1- j, WHITE);
            }
            updateMatrix();
            usleep(HALF_S_IN_U);
        }

        int red = 0;
        int blue = 0;
        int green = 255;

        //fill square out to middle
        for(int j = 0; j <= STRIP/2; j++) {
            blue += 10;
            green -= 2*j;
            for (int i = j; i < STRIP-j; i++) {
                leds[j][i].red = red;
                leds[j][i].blue = blue;
                leds[j][i].green = blue;
                leds[i][j].red = red;
                leds[i][j].blue = blue;
                leds[i][j].green = green;
                leds[STRIP - 1 - j][i].red = red;
                leds[STRIP - 1 - j][i].blue = blue;
                leds[STRIP - 1 - j][i].green = blue;
                leds[i][STRIP - 1 - j].red = red;
                leds[i][STRIP - 1 - j].blue = blue;
                leds[i][STRIP - 1 - j].green = green;
            }
            updateMatrix();
            usleep(HALF_S_IN_U);
        }

        blue = 0;
        green = 255;

        //fill square middle to out
        for(int j = STRIP/2; j >= 0; j--) {
            blue += 10;
            green -= 2*j;
            for (int i = j; i < STRIP-j; i++) {
                leds[j][i].red = red;
                leds[j][i].blue = blue;
                leds[j][i].green = green;
                leds[i][j].red = red;
                leds[i][j].blue = blue;
                leds[i][j].green = blue;
                leds[STRIP - 1 - j][i].red = red;
                leds[STRIP - 1 - j][i].blue = blue;
                leds[STRIP - 1 - j][i].green = green;
                leds[i][STRIP - 1 - j].red = red;
                leds[i][STRIP - 1 - j].blue = blue;
                leds[i][STRIP - 1 - j].green = blue;
            }
            updateMatrix();
            usleep(HALF_S_IN_U);
        }
        usleep(HALF_S_IN_U);

    }

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

    //draw mouth
    for (int i = 0; i < 2; i++) {
        setLedsColor(4, 9 + i, col);
        setLedsColor(5, 9 + i, col);
        setLedsColor(6, 11 + i, col);
        setLedsColor(7, 11 + i, col);
        setLedsColor(8, 11 + i, col);
        setLedsColor(9, 9 + i, col);
        setLedsColor(10, 9 + i, col);
    }

    setLedsColor(5, 11, col);
    setLedsColor(9, 11, col);

    updateMatrix();
}

#endif //LEDTRIX_ANIMATIONS_H

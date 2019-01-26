#ifndef LEDTRIX_ANIMATIONS_H
#define LEDTRIX_ANIMATIONS_H

#include <unistd.h>
#include "buttons.h"
#include "leds.h"
#include "chars.h"

#define ALLOWED_COLOR_CHANGE (255/STRIP)

void checkColorChange(color *col, int *changeR, int *changeG, int *changeB);

void drawRGGradientVertical(color col, int change);

void drawGBGradientHorizontal(color col, int change);

void drawGradientVerticalSingleStep(color col, int changeR, int changeG, int changeB);

void drawGradientHorizontalSingleStep(color col, int changeR, int changeG, int changeB);

void drawGradientDiagonal1Out(color col, int changeR, int changeG, int changeB);

void drawGradientDiagonal1In(color col, int changeR, int changeG, int changeB);

void drawGradientDiagonal2Out(color col, int changeR, int changeG, int changeB);

void drawGradientDiagonal2In(color col, int changeR, int changeG, int changeB);

void drawGradientSquareOut(color col1, color col2, int changeR, int changeG, int changeB);

void drawGradientSquareIn(color col1, color col2, int changeR, int changeG, int changeB);

void drawSmile(color col);

void drawChars(color col);

/**
 * Method for testing if x / y are displayed correct
 *
 * @param leds the matrix
 */
void demo() {
    clear();
    color col1;
    color col2;

    setColor(&col1, 0, 50, 255);
    drawRGGradientVertical(col1, 15);
    sleep(3);

    setColor(&col1, 50, 0, 255);
    drawGBGradientHorizontal(col1, 15);
    sleep(3);

    while (1) {
        clear();
        if (getButtonClickLeft() | getButtonClickRight()) break;

        setColor(&col1, 0, 50, 255);
        drawGradientVerticalSingleStep(col1, 15, 0, -15);
        clear();

        setColor(&col1, 50, 0, 255);
        drawGradientHorizontalSingleStep(col1, 0, 15, -15);
        clear();

        setColor(&col1, 105, 105, 0);
        drawGradientDiagonal1In(col1, -1, -1, 5);

        usleep(HALF_S_IN_U);
        setColor(&col1, 28, 250, 73);
        drawSmile(col1);
        usleep(HALF_S_IN_U);

        setColor(&col1, 60, 60, 0);
        drawGradientDiagonal1Out(col1, -1, -1, 10);
        usleep(HALF_S_IN_U);

        setColor(&col1, 150, 70, 0);
        drawGradientDiagonal2Out(col1, -3, -1, 4);

        setColor(&col1, 255, 70, 0);
        drawGradientDiagonal2In(col1, -3, -1, 3);

        col1 = WHITE;
        col2 = WHITE;
        drawGradientSquareOut(col1, col2, 0, 0, 0);

        setColor(&col1, 0, 255, 0);
        drawGradientSquareIn(col1, col2, 0, -2, 10);

        setColor(&col1, 0, 255, 0);
        drawGradientSquareOut(col1, col2, 0, 10, -2);
        usleep(HALF_S_IN_U);
    }
}

void checkColorChange(color *col, int *changeR, int *changeG, int *changeB) {
    int diff = col->red + STRIP * (*changeR);
    if (diff < 0 || diff > 255) {
        col->red = 0;
        *changeR = ALLOWED_COLOR_CHANGE;
    }

    diff = col->green + STRIP * (*changeG);
    if (diff < 0 || diff > 255) {
        col->green = 0;
        *changeG = ALLOWED_COLOR_CHANGE;
    }

    diff = col->blue + STRIP * (*changeB);
    if (diff < 0 || diff > 255) {
        col->blue = 0;
        *changeB = ALLOWED_COLOR_CHANGE;
    }
}

/**
 * draw a vertical color Gradient between Red and Green
 * @param col on top
 * @param change per row of red and green
 */
void drawRGGradientVertical(color col, int change) {
    if (change > ALLOWED_COLOR_CHANGE) change = ALLOWED_COLOR_CHANGE;
    if (col.red + STRIP * change > 255) col.red = 0;
    if (col.green - STRIP * change < 0) col.green = 255;
    for (int y = 0; y < STRIP; y++) {
        col.red += change;
        col.green -= change;
        for (int x = 0; x < STRIP; x++) {
            setLedsColor(x, y, col);
        }
    }
    updateMatrix();
}

/**
 * draw a horizontal color Gradient between Green and Blue
 * @param col on the left
 * @param change per column of red and green
 */
void drawGBGradientHorizontal(color col, int change) {
    if (change > ALLOWED_COLOR_CHANGE) change = ALLOWED_COLOR_CHANGE;
    if (col.green + STRIP * change > 255) col.green = 0;
    if (col.blue + STRIP * change < 0) col.blue = 255;
    for (int x = 0; x < STRIP; x++) {
        col.green += change;
        col.blue -= change;
        for (int y = 0; y < STRIP; y++) {
            setLedsColor(x, y, col);
        }
    }
    updateMatrix();
}

/**
 * Draw a top down gradient lighting up one LED at a time
 * @param col the color on the top
 * @param changeR
 * @param changeG
 * @param changeB
 */
void drawGradientVerticalSingleStep(color col, int changeR, int changeG, int changeB) {
    checkColorChange(&col, &changeR, &changeG, &changeB);
    for (int y = 0; y < STRIP; y++) {
        col.red += changeR;
        col.green += changeG;
        col.blue += changeB;
        for (int x = 0; x < STRIP; x++) {
            setLedsColor(x, y, col);
            updateMatrix();
        }
    }
}

/**
 * Draw a left to right gradient lighting up one LED at a time
 * @param col the color on the top
 * @param changeR
 * @param changeG
 * @param changeB
 */
void drawGradientHorizontalSingleStep(color col, int changeR, int changeG, int changeB) {
    checkColorChange(&col, &changeR, &changeG, &changeB);
    for (int x = 0; x < STRIP; x++) {
        col.red += changeR;
        col.green += changeG;
        col.blue += changeB;
        for (int y = 0; y < STRIP; y++) {
            setLedsColor(x, y, col);
            updateMatrix();
        }
    }
}

/**
 * Draw Diagonal one from the middle
 * @param col the color in the middle
 * @param changeR
 * @param changeG
 * @param changeB
 */
void drawGradientDiagonal1Out(color col, int changeR, int changeG, int changeB) {
    checkColorChange(&col, &changeR, &changeG, &changeB);
    int red = col.red; //TODO: remove
    int blue = col.blue;
    for (int i = STRIP; i >= 0; i--) {
        //TODO: col.red += changeR
        col.green += changeG;
        col.red = red; //TODO: remove
        col.blue = blue;
        for (int j = 0; j < i; j++) {
            col.red += changeR; //TODO: remove
            col.blue += changeB;
            setLedsColor(j, i - j - 1, col);
            setLedsColor(STRIP - 1 - j, STRIP - i + j, col);
            updateMatrix();
        }
    }
}

/**
 * Draw Diagonal one from the corner
 * @param col the color in upper left and bottom right corners
 * @param changeR
 * @param changeG
 * @param changeB
 */
void drawGradientDiagonal1In(color col, int changeR, int changeG, int changeB) {
    checkColorChange(&col, &changeR, &changeG, &changeB);
    int red = col.red; //TODO: remove
    int blue = col.blue;
    for (int i = 1; i < STRIP + 1; i++) {
        col.green += changeG;
        col.red = red; //TODO: remove
        col.blue = blue;
        for (int j = 0; j < i; j++) {
            col.red += changeR; //TODO: remove
            col.blue += changeB;
            setLedsColor(j, i - j - 1, col);
            setLedsColor(STRIP - 1 - j, STRIP - i + j, col);
            updateMatrix();
        }
    }
}

/**
 * Draw Diagonal two from the middle
 * @param col the color in the middle
 * @param changeR
 * @param changeG
 * @param changeB
 */
void drawGradientDiagonal2Out(color col, int changeR, int changeG, int changeB) {
    checkColorChange(&col, &changeR, &changeG, &changeB);
    int red = col.red; //TODO: remove
    int blue = col.blue;
    for (int i = STRIP; i >= 0; i--) {
        //TODO: col.red += changeR
        col.green += changeG;
        col.red = red; //TODO: remove
        col.blue = blue;
        for (int j = 0; j < i; j++) {
            col.red += changeR; //TODO: remove
            col.blue += changeB;
            setLedsColor(STRIP - i + j, j, col);
            setLedsColor(i - j - 1, STRIP - 1 - j, col);
            updateMatrix();
        }
    }
}

/**
 * Draw Diagonal one from the corner
 * @param col the upper right and bottom left corners
 * @param changeR
 * @param changeG
 * @param changeB
 */
void drawGradientDiagonal2In(color col, int changeR, int changeG, int changeB) {
    checkColorChange(&col, &changeR, &changeG, &changeB);
    int red = col.red; //TODO: remove
    int blue = col.blue;
    for (int i = 1; i < STRIP + 1; i++) {
        //TODO: col.red += changeR
        col.green += changeG;
        col.red = red; //TODO: remove
        col.blue = blue;
        for (int j = 0; j < i; j++) {
            col.red += changeR; //TODO: remove
            col.blue += changeB;
            setLedsColor(i - j - 1, j, col);
            setLedsColor(STRIP - i + j, STRIP - 1 - j, col);
            updateMatrix();
        }
    }
}

void drawGradientSquareOut(color col1, color col2, int changeR, int changeG, int changeB) {
    int cr = changeR / 2;
    int cg = 0; //TODO: int cg =changeG / 2;
    int cb = changeB / 2;
    checkColorChange(&col1, &cr, &cg, &cb);
    checkColorChange(&col2, &cr, &cg, &cb);

    for (int j = STRIP / 2; j >= 0; j--) {
        col1.red += changeR;
        col1.green -= 2 * j; //TODO: remove *j -> col.green += changeG
        col1.blue += changeB;
        col2.red += changeR;
        col2.green = col1.blue;
        col2.blue += changeB; //TODO: better solution for col2?
        for (int i = j; i < STRIP - j; i++) {
            setLedsColor(j, i, col2);
            setLedsColor(i, j, col1);
            setLedsColor(STRIP - 1 - j, i, col2);
            setLedsColor(i, STRIP - 1 - j, col1);
        }
        updateMatrix();
        usleep(HALF_S_IN_U);
    }
}

/**
 * Draw square from the middle
 * @param col1 in the middle
 * @param changeR
 * @param changeG
 * @param changeB
 */
void drawGradientSquareIn(color col1, color col2, int changeR, int changeG, int changeB) {
    int cr = changeR / 2;
    int cg = 0; //TODO: int cg =changeG / 2;
    int cb = changeB / 2;
    checkColorChange(&col1, &cr, &cg, &cb);
    checkColorChange(&col2, &cr, &cg, &cb);

    for (int j = 0; j <= STRIP / 2; j++) {
        col1.red += changeR;
        col1.green -= 2 * j; //TODO: remove *j -> col.green += changeG
        col1.blue += changeB;
        col2.red += changeR;
        col2.green = col1.blue;
        col2.blue += changeB; //TODO: better solution for col2?

        for (int i = j; i < STRIP - j; i++) {
            setLedsColor(j, i, col1);
            setLedsColor(i, j, col2);
            setLedsColor(STRIP - 1 - j, i, col1);
            setLedsColor(i, STRIP - 1 - j, col2);
        }
        updateMatrix();
        usleep(HALF_S_IN_U);
    }
}

/**
 * draw a smiley
 * @param col the color of the smiley
 */
void drawSmile(color col) {
    clear();

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

void drawChars(color col) {
    clear();
    int character[5][5];
    for (int c = 1; c <= 18;) {
        for (int by = 0; by < 3; by++) {
            for (int bx = 0; bx < 3; bx++) {
                if (getChar(character, c)) {
                    for (int y = 0; y < 5; y++) {
                        for (int x = 0; x < 5; x++) {
                            if (character[y][x]) setLedsColor(x + bx * 5, y + by * 5, col);
                        }
                    }
                    updateMatrix();
                    sleep(1);
                    c++;
                }
            }
        }
        clear();
    }
}

#endif //LEDTRIX_ANIMATIONS_H

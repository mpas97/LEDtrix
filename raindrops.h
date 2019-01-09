#ifndef LEDTRIX_RAINDROPS_H
#define LEDTRIX_RAINDROPS_H

#include <stdbool.h>
#include "color.h" //TODO: remove
#include "leds.h" //TODO: remove

bool raindrops[STRIP][STRIP] = {false};
int player;
struct color colorFail;
struct color colPlayer;
struct color colorDrop;

/**
 * Draw and print the field with the player.
 *
 * @param color the color of the player
 */
void drawPlayer(struct color col) {
    setLedsColor(player, STRIP - 1, col);
    setLedsColor(player + 1, STRIP - 1, col);
    setLedsColor(player, STRIP - 2, col);
    setLedsColor(player + 1, STRIP - 2, col);
    updateMatrix();
}

/**
 * What should happen if the player loses (gets hit by a raindrop)
 */
void endRaindrops() {
    //player = STRIP / 2 - 1;
    clear();

    int x = 0;
    int y = 0;

    struct color col;
    setColor(&col, 225, 30, 30);

    for (int i = STRIP -1; i > 0; i--) {
        setLedsColor(x, y + i, col);
        setLedsColor(x + STRIP-1, y + i, col);
        setLedsColor(x + i, y, col);
        setLedsColor(x + i, y + STRIP-1, col);

    }
    updateMatrix();
}

/**
 * Control the position of the player using his upper left position.
 */
bool control() {
    if (raindrops[player][STRIP - 1] || raindrops[player + 1][STRIP - 1] ||
        raindrops[player][STRIP - 2] || raindrops[player + 1][STRIP - 2]) {
        drawPlayer(colorFail);
        return false;
    }
    drawPlayer(colPlayer);
    return true;
}

/**
 * Move each row one row down and create a new one.
 * The chance of having a raindrop is 1/3 per field, there are always two free fields next to eachother.
 */
void rain() {
    for (int y = STRIP - 1; y > 0; y--) {
        for (int x = 0; x < STRIP; x++) {
            raindrops[x][y] = raindrops[x][y - 1];
            if (raindrops[x][y])
                setLedsColor(x, y, colorDrop);
            else
                setLedsRGB(x, y, 0, 0, 0);
        }
    }
    for (int i = 0; i < STRIP; i++) {
        int color = rand() % 3;
        if (color == 2) {
            raindrops[i][0] = true;
            setLedsColor(i, 0, colorDrop);

        } else {
            raindrops[i][0] = false;
            setLedsRGB(i, 0, 0, 0, 0);
            raindrops[++i][0] = false;
            setLedsRGB(i, 0, 0, 0, 0);

        }
    }
    //sleep(1);
}

/**
 * Move the player one to the left using his uppper left position.
 */
void moveLeft() {
    if (player > 0) {
        setLedsRGB(player + 1, STRIP - 1, 0, 0, 0);
        setLedsRGB(player + 1, STRIP - 2, 0, 0, 0);
        player -= 1;
    }
}

/**
 * Move the player one to the right using his uppper left position.
 */
void moveRight() {
    if (player < STRIP - 2) {
        setLedsRGB(player, STRIP - 1, 0, 0, 0);
        setLedsRGB(player, STRIP - 2, 0, 0, 0);
        player += 1;
    }
}

void startRaindrops() {
    setColor(&colorFail, 205, 51, 51);
    setColor(&colPlayer, 34, 139, 34);
    setColor(&colorDrop, 47, 79, 79);

    player = STRIP / 2 - 1;
    drawPlayer(colPlayer);

    bool running = true;

    while (running) {
        //TODO: if left button: move left, if right button: move right, rain zeit?
        rain();
        control();
        moveLeft();
        control();
        moveRight();
        control();
        running = control();
    }

    endRaindrops();
    sleep(10);
}

#endif //LEDTRIX_RAINDROPS_H

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
void drawPlayer(struct color col, struct color leds[STRIP][STRIP]) {
    setLedsColor(player, STRIP - 1, col, leds);
    setLedsColor(player + 1, STRIP - 1, col, leds);
    setLedsColor(player, STRIP - 2, col, leds);
    setLedsColor(player + 1, STRIP - 2, col, leds);
    updateMatrix(leds);
}

/**
 * What should happen if the player loses (gets hit by a raindrop)
 */
void endGame() {
    player = STRIP / 2 - 1;

}

/**
 * Control the position of the player using his upper left position.
 */
bool control(struct color leds[STRIP][STRIP]) {
    if (raindrops[player][STRIP - 1] || raindrops[player + 1][STRIP - 1] ||
        raindrops[player][STRIP - 2] || raindrops[player + 1][STRIP - 2]) {
        endGame();
        drawPlayer(colorFail, leds);
        return false;
    }
    drawPlayer(colPlayer, leds);
    return true;
}

/**
 * Move each row one row down and create a new one.
 * The chance of having a raindrop is 1/3 per field, there are always two free fields next to eachother.
 */
void rain(struct color leds[STRIP][STRIP]) {
    for (int y = STRIP - 1; y > 0; y--) {
        for (int x = 0; x < STRIP; x++) {
            raindrops[x][y] = raindrops[x][y - 1];
            if (raindrops[x][y])
                setLedsColor(x, y, colorDrop, leds);
            else
                setLedsRGB(x, y, 0, 0, 0, leds);
        }
    }
    for (int i = 0; i < STRIP; i++) {
        int color = rand() % 3;
        if (color == 2) {
            raindrops[i][0] = true;
            setLedsColor(i, 0, colorDrop, leds);

        } else {
            raindrops[i][0] = false;
            setLedsRGB(i, 0, 0, 0, 0, leds);
            raindrops[++i][0] = false;
            setLedsRGB(i, 0, 0, 0, 0, leds);

        }
    }
}

/**
 * Move the player one to the left using his uppper left position.
 */
void moveLeft(struct color leds[STRIP][STRIP]) {
    if (player > 0) {
        setLedsRGB(player + 1, STRIP - 1, 0, 0, 0, leds);
        setLedsRGB(player + 1, STRIP - 2, 0, 0, 0, leds);
        player -= 1;
    }
}

/**
 * Move the player one to the right using his uppper left position.
 */
void moveRight(struct color leds[STRIP][STRIP]) {
    if (player < STRIP - 2) {
        setLedsRGB(player, STRIP - 1, 0, 0, 0, leds);
        setLedsRGB(player, STRIP - 2, 0, 0, 0, leds);
        player += 1;
    }
}

int startRaindrops(struct color leds[STRIP][STRIP]) {
    setColor(colorFail, 205, 51, 51);
    setColor(colPlayer, 34, 139, 34);
    setColor(colorDrop, 47, 79, 79);

    player = STRIP / 2 - 1;
    drawPlayer(colPlayer, leds);

    bool running = true;

    while (running) {
        //TODO: if left button: move left, if right button: move right, rain zeit?
        rain(leds);
        control(leds);
        moveLeft(leds);
        control(leds);
        moveRight(leds);
        control(leds);
        running = control(leds);
    }

    return 0;
}

#endif //LEDTRIX_RAINDROPS_H

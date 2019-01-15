#ifndef LEDTRIX_RAINDROPS_H
#define LEDTRIX_RAINDROPS_H

#include <stdbool.h>
#include "color.h" //TODO: remove
#include "leds.h" //TODO: remove
#include "buttons.h" //TODO: remove

void drawPlayer();

void rain();

void moveLeft();

void moveRight();

void drawSadFace();

void rd_checkStatus();

void endRaindrops();

void startRaindrops();

bool rd_running = true;
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
void drawPlayer() {
    struct color col;
    col = rd_running? colPlayer : colorFail;

    setLedsColor(player, STRIP - 1, col);
    setLedsColor(player + 1, STRIP - 1, col);
    setLedsColor(player, STRIP - 2, col);
    setLedsColor(player + 1, STRIP - 2, col);
    updateMatrix();
}

/**
 * Move each row two rows down and create a new one.
 * The chance of having a raindrop is 1/3 per field, there are always two free fields next to eachother.
 */
void rain() {
    for (int y = STRIP - 1; y > 0; y--) {
        for (int x = 0; x < STRIP; x++) {
            raindrops[x][y] = raindrops[x][y - 2];
            if (raindrops[x][y])
                setLedsColor(x, y, colorDrop);
            else
                setLedsRGB(x, y, 0, 0, 0);
        }
    }
    for (int i = 0; i < STRIP; i++) {
        int rain = rand() % 3;
        if (rain == 2) {
            raindrops[i][0] = true;
            setLedsColor(i, 0, colorDrop);
        } else {
            raindrops[i][0] = false;
            setLedsRGB(i, 0, 0, 0, 0);
            raindrops[++i][0] = false;
            setLedsRGB(i, 0, 0, 0, 0);

        }
    }
    rd_checkStatus();
}

/**
 * Move the player one to the left using his uppper left position.
 */
void moveLeft() {
    if (player > 0) {
        setLedsRGB(player + 1, STRIP - 1, 0, 0, 0);
        setLedsRGB(player + 1, STRIP - 2, 0, 0, 0);
        player -= 1;
        rd_checkStatus();
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
        rd_checkStatus();
    }
}

void drawSadFace(){
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
void rd_checkStatus() {
    if(rd_running) {
        if (raindrops[player][STRIP - 1] || raindrops[player + 1][STRIP - 1] ||
            raindrops[player][STRIP - 2] || raindrops[player + 1][STRIP - 2]) {
            rd_running = false;
            drawPlayer();
            sleep(1);
        }
        else {
            rd_running = true;
            drawPlayer();
        }
    }
}

/**
 * What should happen if the player loses (gets hit by a raindrop)
 */
void endRaindrops() {
    drawSadFace();
    while (!btn_l && !btn_r) {}
}

/**
 * start Raindrops
 */
void startRaindrops() {
    setColor(&colorFail, 205, 51, 51);
    setColor(&colPlayer, 34, 139, 34);
    setColor(&colorDrop, 47, 79, 79);

    player = STRIP / 2 - 1;
    drawPlayer();

    struct timeval now;
    unsigned long diff;
    struct timeval beginning;
    gettimeofday(&now, NULL);

    while (rd_running) {

        diff = 0;
        beginning = now;

        while(diff < 3000000) {
            if(btn_r){
                moveRight();
                btn_r = false;
            }
            if(btn_l){
                moveLeft();
                btn_l = false;
            }

            gettimeofday(&now, NULL);
            // Time difference in usec
            diff = (now.tv_sec * 1000000 + now.tv_usec) - (beginning.tv_sec * 1000000 + beginning.tv_usec);
        }

        rain();
    }

    endRaindrops();
}

#endif //LEDTRIX_RAINDROPS_H

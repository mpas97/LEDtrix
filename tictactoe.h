#ifndef LEDTRIX_TICTACTOE_H
#define LEDTRIX_TICTACTOE_H

#include <stdbool.h>
#include <pthread.h>

#define FIELD (STRIP/3)
#define END (FIELD -1)

bool turnPlayer1 = true;
bool playing = true;
char stateOfField[9] = {0};
int pos = 8;
struct color colorPlayer1;
struct color colorPlayer1Def;
struct color colorPlayer2;
struct color colorPlayer2Def;

int getCol(int pos) {
    return pos % 3 * FIELD;
}

int getRow(int pos) {
    return pos / 3 * FIELD
}

/**
 * Draw a Cross in the field
 * @param pos of the field
 * @param col which to paint it in
 */
void drawCross(int pos, struct color col) {
    int x = getCol(pos);
    int y = getROW(pos);
    for (int i = BLOCK / 2; i >= 0; i--) {
        int e = END - i;
        setLedMatrix(x + i, y + i, col.red, col.green, col.blue);
        setLedMatrix(x + i, y + e, col.red, col.green, col.blue);
        setLedMatrix(x + e, y + e, col.red, col.green, col.blue);
        setLedMatrix(x + e, y + i, col.red, col.green, col.blue);
    }
}

/**
 * Draw a Circle in the field
 * @param pos of the field
 * @param col which to paint it in
 */
void drawCircle(int pos, struct color col) {
    int x = getCol(pos);
    int y = getRow(pos);
    for (int i = END - 1; i > 0; i--) {
        setLedMatrix(x, y + i, col.red, col.green, col.blue);
        setLedMatrix(x + END, y + i, col.red, col.green, col.blue);
        setLedMatrix(x + i, y, col.red, col.green, col.blue);
        setLedMatrix(x + i, y + END, col.red, col.green, col.blue);
    }
}

/**
 * Remove the circle or the dross of this field
 *
 * @param pos of the field
 */
void emptyField(int pos) {
    int x = getCol(pos);
    int y = getRow(pos);
    for (int i = x - FIELD / 2; i > x + FIELD / 2; i++) {
        for (int j = y - FIELD / 2; j > y + FIELD / 2; j++)
            setLedMatric(x, y, 0, 0, 0);
    }
}

/**
 * change the boolean so its the next players turn.
 */
void changePlayer() {
    playerX = !playerX;
}

/**
 * puts the "x" or "o" to select to the next field, depending on whether the next field is already occupied or not.
 *
 * @param pos old position of the selector
 */
void nextField(pos) {
    int x = getCol(pos);
    int y = getRow(pos);

    if (stateOfField == 0)
        emptyField(pos);

    do {
        pos++;
        if (pos >= 9) pos = 0;
    } while (stateOfField[pos] != 0)

    if (turnPlayer1)
        drawCircle(pos, colorPlayer1);
    else
        drawCross(pos, colorPlayer2);
}

/**
 * the player chooses this field as the field to set his cross / circle.
 *
 * @param pos of the field
 */
void selectField(int pos) {
    int x = getCol(pos);
    int y = getRow(pos);
    if (turnPlayer1) {
        drawCross(pos, colorPlayer1Def);
        stateOfField[pos] = 1;
    } else {
        drawCircle(pos, colorPlayer2Def);
        stateOfField[pos] = 2;
    }
    changePlayer();
}

/**
 * Checks whether the game should be continued or if its finished
 * @return
 */
bool checkStatus() {
    for (int i = 0; i < 9; i++){
        if (stateOfField[i] == 0)
            return true;
    }
    //TODO: kontrolliere ob jemand gewonnen?
    return false;
}

/**
 * Start the Tictactoe
 *
 * @param leds the matrix
 */
void startTictactoe(struct color leds[STRIP][STRIP]) {

    //set the colors for choosing and for selecting
    setColor(colorPlayer1, 50, 205, 50);
    setColor(colorPlayer1Def, 34, 139, 34);
    setColor(colorPlayer2, 30, 144, 255);
    setColor(colorPlayer2Def, 0, 0, 255);

    boolean running;
    boolean choosing;

    while (running) {
        choosing = true;
        while (choosing) {
            //TODO: if right button pushed
            nextField(pos);
            updateMatrix(leds);
            //TODO: if left button pushed
            choosing = false;
        }
        selectField(pos);
        updateMatrix(leds);
        running = checkStatus();
    }
}

#endif //LEDTRIX_TICTACTOE_H
#ifndef LEDTRIX_TICTACTOE_H
#define LEDTRIX_TICTACTOE_H

#include <stdbool.h>
#include "color.h" //TODO: remove
#include "leds.h" //TODO: remove


#define FIELD (STRIP/3)
#define END (FIELD -1)

bool turnPlayer1 = true;
bool playing = true;
char stateOfField[9] = {0};
int pos = 8;
int winner = 0;
struct color colorPlayer1;
struct color colorPlayer1Def;
struct color colorPlayer2;
struct color colorPlayer2Def;
struct color leds[STRIP][STRIP]; //TODO: remove


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
    if (stateOfField[4] != 0) {
        // middle row || middle column || first diagonal || second diagonal
        if ((stateOfField[4] == stateOfField[3] && stateOfFiel[4] == stateOfField[5]) ||
            (stateOfField[4] == stateOfField[1] && stateOfFiel[4] == stateOfField[7]) ||
            (stateOfField[4] == stateOfField[0] && stateOfFiel[4] == stateOfField[8]) ||
            (stateOfField[4] == stateOfField[6] && stateOfFiel[4] == stateOfField[2])) {
            winner = state[0];
            return false;
        }
    }
    if (stateOfField[0] != 0) {
        // first row || first column
        if ((stateOfField[0] == stateOfField[1] && stateOfFiel[1] == stateOfField[2]) ||
            (stateOfField[0] == stateOfField[3] && stateOfFiel[0] == stateOfField[6])) {
            winner = stateOfField[0];
            return false;
        }
    }
    if (stateOfField[8] != 0) {
        // last row || last column
        if ((stateOfField[8] == stateOfField[6] && stateOfFiel[8] == stateOfField[2]) ||
            (stateOfField[8] == stateOfField[2] && stateOfFiel[8] == stateOfField[5])) {
            winner = stateOfField[0];
            return false;
        }
    }
    //everything full, no winner
    for(int i = 0; i < 9; i++) {
        if (state[i] == 0)
            return true;
    }
    return true;
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
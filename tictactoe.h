#ifndef LEDTRIX_TICTACTOE_H
#define LEDTRIX_TICTACTOE_H

#include <stdbool.h>
#include "color.h" //TODO: remove
#include "leds.h" //TODO: remove


#define FIELD (STRIP/3)
#define END (FIELD -1)

bool turnPlayer1 = true;
bool playing = true;
int pos = 8;
int winner = 0;

//-1 for empty, 0 for preview, 1 or 2 for selected by player 1 or 2
int stateOfField[9] = {-1};

bool checkStatus();

int getCol(int pos) {
    return pos % 3 * FIELD;
}

int getRow(int pos) {
    return pos / 3 * FIELD;
}

/**
 * Draw a Cross or a Circle in the field
 *
 * @param pos of the field
 */
void drawItem(int pos) {
    int x = getCol(pos);
    int y = getRow(pos);

    //choose the color to draw the item in
    struct color col;

    //set the colors for preview, player, oder clearing the field
    switch (stateOfField[pos]) {
        case 0:
            setColor(&col, 238, 118, 99);
            break;
        case 1:
            setColor(&col, 40, 255, 40);
            break;
        case 2:
            setColor(&col, 0, 0, 255);
            break;
        default:
            setColor(&col, 0, 0, 0);
    }

    //draws
    if (turnPlayer1) {
        //draw cross
        for (int i = FIELD / 2; i >= 0; i--) {
            int e = END - i;
            setLedsColor(x + i, y + i, col);
            setLedsColor(x + i, y + e, col);
            setLedsColor(x + e, y + e, col);
            setLedsColor(x + e, y + i, col);
        }

    } else {
        //draw circle
        for (int i = END - 1; i > 0; i--) {
            setLedsColor(x, y + i, col);
            setLedsColor(x + END, y + i, col);
            setLedsColor(x + i, y, col);
            setLedsColor(x + i, y + END, col);
        }
    }
    updateMatrix();
}

void drawWon() {
    turnPlayer1 = winner == 1;

    clear();

    for (int i = 0; i < 3; i++) {
        stateOfField[i] = winner;
        drawItem(i);
    }
    for (int i = 6; i < 9; i++) {
        stateOfField[i] = winner;
        drawItem(i);
    }

    struct color col;
    setColor(&col, 225, 30, 30);

    //draw W
    int x = getCol(3);
    int y = getRow(3);
    for (int i = 0; i < FIELD; i++) {
        setLedsColor(x, y + i, col);
        setLedsColor(x + END, y + i, col);
    }
    setLedsColor(x + END / 2, y + END / 2, col);
    setLedsColor(x + 1, y + END - 1, col);
    setLedsColor(x + END - 1, y + END - 1, col);
    sleep(1);
    updateMatrix();

    //draw O
    x = getCol(4);
    for (int i = END - 1; i > 0; i--) {
        setLedsColor(x, y + i, col);
        setLedsColor(x + END, y + i, col);
        setLedsColor(x + i, y, col);
        setLedsColor(x + i, y + END, col);
    }
    sleep(1);
    updateMatrix();

    //draw N
    x = getCol(5);
    for (int i = 0; i < FIELD; i++) {
        setLedsColor(x, y + i, col);
        setLedsColor(x + END, y + i, col);
        setLedsColor(x + i, y + i, col);
    }
    sleep(1);
    updateMatrix();
}

/**
 * Remove the circle or the dross of this field
 *
 * @param pos of the field
 */
void clearField(int pos) {
    stateOfField[pos] = -1;
    drawItem(pos);
}

/**
 * change the boolean so its the next players turn.
 */
void changePlayer() {
    turnPlayer1 = !turnPlayer1;
}

/**
 * puts the "x" or "o" to select to the next field, depending on whether the next field is already occupied or not.
 *
 * @param pos old position of the selector
 */
void nextField() {
    if (stateOfField[pos] == 0)
        clearField(pos);
    if (checkStatus()) {
        do {
            pos++;
            if (pos >= 9) pos = 0;
        } while (stateOfField[pos] != -1);
        stateOfField[pos] = 0;
        drawItem(pos);
    }
}

/**
 * the player chooses this field as the field to set his cross / circle.
 *
 * @param pos of the field
 */
void selectField() {
    stateOfField[pos] = turnPlayer1 ? 1 : 2;
    drawItem(pos);
    changePlayer();
}

/**
 * Checks whether the game should be continued or if its finished
 * @return
 */
bool checkStatus() {
    if (stateOfField[4] != -1) {
        // middle row || middle column || first diagonal || second diagonal
        if ((stateOfField[4] == stateOfField[3] && stateOfField[4] == stateOfField[5]) ||
            (stateOfField[4] == stateOfField[1] && stateOfField[4] == stateOfField[7]) ||
            (stateOfField[4] == stateOfField[0] && stateOfField[4] == stateOfField[8]) ||
            (stateOfField[4] == stateOfField[6] && stateOfField[4] == stateOfField[2])) {
            winner = stateOfField[4];
            return false;
        }
    }
    if (stateOfField[0] != -1) {
        // first row || first column
        if ((stateOfField[0] == stateOfField[1] && stateOfField[1] == stateOfField[2]) ||
            (stateOfField[0] == stateOfField[3] && stateOfField[0] == stateOfField[6])) {
            winner = stateOfField[0];
            return false;
        }
    }
    if (stateOfField[8] != -1) {
        // last row || last column
        if ((stateOfField[8] == stateOfField[6] && stateOfField[8] == stateOfField[2]) ||
            (stateOfField[8] == stateOfField[2] && stateOfField[8] == stateOfField[5])) {
            winner = stateOfField[8];
            return false;
        }
    }
    //everything full, no winner
    for (int i = 0; i < 9; i++) {
        if (stateOfField[i] == -1)
            return true;
    }
    return false;
}

void endTictactoe() {
    drawWon();
    sleep(10);
}

/**
 * Start the Tictactoe
 *
 * @param leds the matrix
 */
void startTictactoe() {

    for (int i = 0; i < 9; i++) {
        stateOfField[i] = -1;
    }

    turnPlayer1 = false;
    bool running = true;
    bool choosing;

    while (running) {
        choosing = true;

        while (choosing) {
            //TODO: if right button pushed
            nextField();
            //sleep(1);
            //TODO: if left button pushed
            choosing = false;
        }
        selectField();
        //sleep(1);
        running = checkStatus();
    }

    endTictactoe();
}

#endif //LEDTRIX_TICTACTOE_H
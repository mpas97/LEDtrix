#ifndef LEDTRIX_TICTACTOE_H
#define LEDTRIX_TICTACTOE_H

#include <stdbool.h>
#include "color.h" //TODO: remove
#include "leds.h" //TODO: remove
#include "buttons.h" //TODO: remove


#define FIELD (STRIP/3)
#define END (FIELD -1)

int getCol(int pos);

int getRow(int pos);

void drawItem(int pos);

void drawTie();

void drawWon();

void changePlayer();

void nextField();

void clearField(int pos);

void selectField();

bool ttt_checkStatus();

void endTictactoe();

void startTictactoe();

void ttt_drawImage();


bool turnPlayer1 = true;
bool ttt_running = true;
bool choosing = true;
int pos = 8;
int winner = 0;

//-1 for empty, 0 for preview, 1 or 2 for selected by player 1 or 2
int stateOfField[9] = {-1};


/**
 * get the column of the matrix
 *
 * @param pos in the matrix
 * @return row
 */
int getCol(int pos) {
    return pos % 3 * FIELD;
}

/**
 * get the row of the matrix
 *
 * @param pos in the matrix
 * @return row
 */
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

/**
 * draw the item of the winner in his color, and write "WON" in the middle row
 */
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
    sleep(1);

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
    updateMatrix();
    sleep(1);

    //draw O
    x = getCol(4);
    for (int i = END - 1; i > 0; i--) {
        setLedsColor(x, y + i, col);
        setLedsColor(x + END, y + i, col);
        setLedsColor(x + i, y, col);
        setLedsColor(x + i, y + END, col);
    }
    updateMatrix();
    sleep(1);

    //draw N
    x = getCol(5);
    for (int i = 0; i < FIELD; i++) {
        setLedsColor(x, y + i, col);
        setLedsColor(x + END, y + i, col);
        setLedsColor(x + i, y + i, col);
    }
    updateMatrix();
}

/**
 * draw both items in the correct color, and write "TIE" in the middle row
 */
void drawTie() {
    clear();
    for (int i = 0; i < 3; i++) {
        pos = i;
        stateOfField[pos] = 0;
        selectField();
    }
    for (int i = 6; i < 9; i++) {
        pos = i;
        stateOfField[pos] = 0;
        selectField();
    }
    sleep(1);

    struct color col;
    setColor(&col, 225, 30, 30);

    //draw T
    int x = getCol(3);
    int y = getRow(3);
    for (int i = 0; i <= END; i++) {
        setLedsColor(x + i, y, col);
        setLedsColor(x + (END / 2), y + i, col);
    }
    updateMatrix();
    sleep(1);

    //draw I
    x = getCol(4);
    for (int i = 0; i <= END; i++) {
        setLedsColor(x + (END / 2), y + i, col);
    }
    updateMatrix();
    sleep(1);

    //draw E
    x = getCol(5);
    for (int i = 0; i <= END; i++) {
        setLedsColor(x, y + i, col);
        setLedsColor(x + i, y, col);
        setLedsColor(x + i, y + (END / 2), col);
        setLedsColor(x + i, y + END, col);
    }
    updateMatrix();
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
    if (ttt_checkStatus()) {
        do {
            pos++;
            if (pos >= 9) pos = 0;
        } while (stateOfField[pos] != -1);
        stateOfField[pos] = 0;
        drawItem(pos);
    }
}

/**
 * Remove the circle or the cross of this field
 *
 * @param pos of the field
 */
void clearField(int pos) {
    stateOfField[pos] = -1;
    drawItem(pos);
}

/**
 * the player chooses this field as the field to set his cross / circle.
 *
 * @param pos of the field
 */
void selectField() {
    if (stateOfField[pos] == 0) {
        stateOfField[pos] = turnPlayer1 ? 1 : 2;
        drawItem(pos);
        changePlayer();
        pos = -1;
        nextField();
    }
}

/**
 * Checks whether the game should be continued or if its finished
 * @return
 */
bool ttt_checkStatus() {
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
        if ((stateOfField[0] == stateOfField[1] && stateOfField[0] == stateOfField[2]) ||
            (stateOfField[0] == stateOfField[3] && stateOfField[0] == stateOfField[6])) {
            winner = stateOfField[0];
            return false;
        }
    }
    if (stateOfField[8] != -1) {
        // last row || last column
        if ((stateOfField[8] == stateOfField[6] && stateOfField[8] == stateOfField[7]) ||
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

    winner = 0;
    return false;
}

void endTictactoe() {
    if (winner == 0) drawTie();
    else drawWon();
    while (!btn_l && !btn_r) {}
    clear();
}

/**
 * method to finish the game without a winner
 * TODO: remove
 */
void getTie() {
    selectField();
    pos = 1;
    selectField();
    pos = 2;
    selectField();
    pos = 3;
    selectField();
    pos = 5;
    selectField();
    pos = 4;
    selectField();
    pos = 6;
    selectField();
    pos = 8;
    selectField();
    pos = 7;
    selectField();
    ttt_checkStatus();
    endTictactoe();
}

/**
 * Start the Tictactoe
 */
void startTictactoe() {

    clear();

    for (int i = 0; i < 9; i++) {
        stateOfField[i] = -1;
    }

    //getTie();
    nextField();
    while (ttt_running) {
        choosing = true;
        while (choosing) {
            if (btn_r) {
                btn_r = false;
                nextField();
            }
            if (btn_l) {
                choosing = false;
                btn_l = false;
            }
            //sleep(1);
        }
        selectField();
        ttt_running = ttt_checkStatus();
    }

    endTictactoe();
}

void ttt_drawImage(){
    stateOfField[4] = 1;
    drawItem(4);
}


#endif //LEDTRIX_TICTACTOE_H
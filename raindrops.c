#include "engine.h"
#include <stdio.h>
#include <stdlib.h>

#define COLDP B
#define COLPL G
#define COLFS R

void drawField(const int* pl, char color){
    setLED(*pl, SIZE - 1, color);
    setLED(*pl + 1, SIZE - 1, color);
    setLED(*pl, SIZE - 2, color);
    setLED(*pl + 1, SIZE - 2, color);
    print();
}

void endGame(int* pl) {
    *pl = SIZE/2 - 1;
    fill(OFF);
}

/*
 * Controle the position of the player using his upper left position.
 * Return COLFS, if the player gets hit by a raindrop, or COLDP if not.
 */
void control(int* pl){
    char color;
    if (matrix[SIZE-1][*pl] == COLDP || matrix[SIZE-1][*pl+1] == COLDP ||
    matrix[SIZE-2][*pl] == COLDP || matrix[SIZE-2][*pl+1] == COLDP) {
       color = COLFS;
       endGame(pl);
    } else {
        color= COLPL;
    }
    drawField(pl, color);
}

/*
 * Move each row one row down and create a new one.
 * The chance of having a raindrop is 1/3 per field.
 */
void rain(int* pl){
    for (int y = SIZE-1; y > 0; y--){
        for(int x = 0; x < SIZE; x++) {
            matrix[y][x] = matrix[y-1][x];
        }
    }
    for (int i = 0; i < SIZE; i++){
        int color = rand() %3;
        if(color == 2){
            matrix[0][i] = COLDP;
        } else {
            matrix[0][i] = OFF;
        }
        // matrix[0][i] = ((color < 2) ? COLDP : OFF); wieso git das e warnig??
    }
    control(pl);
}

/*
 * Move the player one to the left using his uppper left position.
 */
void moveLeft(int* pl){
    if(*pl > 0){
        matrix[SIZE-1][*pl+1] = OFF;
        matrix[SIZE-2][*pl+1] = OFF;
        //setLED(pl+1, SIZE-1, OFF);
        //setLED(pl+1, SIZE-2, OFF);
        *pl -= 1;
        control(pl);
    }
}

/*
 * Move the player one to the right using his uppper left position.
 */
void moveRight(int* pl){
    if(*pl < SIZE-2){
        matrix[SIZE-1][*pl] = OFF;
        matrix[SIZE-2][*pl] = OFF;
        //setLED(pl, SIZE-1, OFF);
        //setLED(pl, SIZE-2, OFF);
        *pl += 1;
        control(pl);
    }
}

int main() {

    int player = SIZE / 2 -1;
    int* pl = &player;
    drawField(pl, COLPL);

    for (int i = 0; i < 10; i++) {
        rain(pl);
        rain(pl);
        moveLeft(pl);
        rain(pl);
        moveRight(pl);
    }

    return 0;
}
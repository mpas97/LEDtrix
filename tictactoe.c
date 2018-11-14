#include "engine.h"
#include <stdbool.h>

#define BLOCK (SIZE/3)
#define   END (BLOCK-1)
#define ALIGN (SIZE%3/2)
#define COLP1 R
#define COLP2 G
#define COLBG B

bool playerX = true;
char state[3][3] = {0};

int getCol(int pos) {
    return pos%3;
}

int getRow(int pos) {
    return pos/3;
}

int getX(int pos) {
    return getCol(pos) * BLOCK + ALIGN;
}

int getY(int pos) {
    return getRow(pos) * BLOCK + ALIGN;
}

void drawCross(int pos, char color) {
    int x = getX(pos);
    int y = getY(pos);
    for (int i = BLOCK/2; i>=0; i--) {
        int e = END-i;
        setLED(x+i, y+i, color);
        setLED(x+i, y+e, color);
        setLED(x+e, y+e, color);
        setLED(x+e, y+i, color);
    }
}

void drawCircle(int pos, char color) {
    int x = getX(pos);
    int y = getY(pos);
    for (int i = END-1; i>0; i--) {
        setLED(x, y+i, color);
        setLED(x+END, y+i, color);
        setLED(x+i, y, color);
        setLED(x+i, y+END, color);
    }
}

void changePlayer() {
    playerX = !playerX;
}

void swap(int pos) {
    int x = getCol(pos);
    int y = getRow(pos);
    if (playerX) {
        switch (state[y][x]) {
            case 0:
                drawCross(pos, COLP1);
                state[y][x] = 1;
                break;
            case 1:
                drawCross(pos, COLBG);
                state[y][x] = 0;
                break;
            default:
                break;
        }
    } else {
        switch (state[y][x]) {
            case 0:
                drawCircle(pos, COLP2);
                state[y][x] = 2;
                break;
            case 2:
                drawCircle(pos, COLBG);
                state[y][x] = 0;
                break;
            default:
                break;
        }
    }
}


int main() {
    fill(B);
    bool empty = true;
    for (int i = 0; i <= 8; i++) {
        swap(i);
        changePlayer();
        print();
        if (i == 8) {
            i = -1;
            empty = !empty;
            if (!empty) {
                changePlayer();
            }
        }
    }
    /*fill(G);
    print();
    fill(B);
    print();*/
}
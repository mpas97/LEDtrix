#ifndef LEDTRIX_SNAKE_H
#define LEDTRIX_SNAKE_H

#include "buttons.h"
#include "leds.h"

#define TOP     1
#define RIGHT   2
#define BOTTOM  3
#define LEFT    4

int size;
int grow;
int direction;

typedef struct body {
    struct body *parent;
    int x, y;
} body;

body *head;
body *tail;

void checkDirection() {
    if (getButtonClickLeft()) {
        direction--;
    }
    if (getButtonClickRight()) {
        direction++;
    }
    if (direction <= 0) direction = LEFT;
    if (direction >= 5) direction = TOP;
}

void goodie() {
    int posx, posy;
    do {
         posx = rand() % STRIP;
         posy = rand() % STRIP;
    } while (leds[posx][posy].green);
    leds[posx][posy].red = 255;
}

int move() {
    checkDirection();
    body *part = malloc(sizeof(body));
    part->x = head->x;
    part->y = head->y;
    head->parent = part;
    if (head == tail) tail = head;
    head = part;
    switch (direction) {
        case TOP:
            if (head->y==0 || leds[head->x][--head->y].green) return 1; else break;
        case RIGHT:
            if (head->x==STRIP-1 || leds[++head->x][head->y].green) return 1; else break;
        case BOTTOM:
            if (head->y==STRIP-1 || leds[head->x][++head->y].green) return 1; else break;
        case LEFT:
            if (head->x==0 || leds[--head->x][head->y].green) return 1; else break;
        default:
            printf("ERROR: unknown direction!\n");
            break;
    }
    leds[head->x][head->y].green = 255;
    if (size <= 5 || leds[head->x][head->y].red) {
        size++;
    } else {
        leds[tail->x][tail->y].green = leds[tail->x][tail->y].red = 0;
        body *oldTail = tail;
        if (tail->parent != NULL) tail = tail->parent;
        free(oldTail);

    }
    updateMatrix();
    return 0;
}

void startSnake() {
    clear();
    size = 1;
    direction = RIGHT;
    head = malloc(sizeof(body));
    head->parent = NULL;
    head->x = head->y = STRIP/2;
    tail = head;
    for (int i = 1; i <= 10; i++) {
        if (move()) break;
        if (i == 10) {
            goodie();
            i = 0;
        }
        sleep(1);
    }
    clear();
    for (int i = 0; i < 5; i++) {
        body *tmp = tail;
        while (tmp != head) {
            leds[tmp->x][tmp->y].green = i%2==0 ? 0 : 255;
            tmp = tmp->parent;
        }
        updateMatrix();
        usleep(HALF_S_IN_U);
    }

    while (tail != head) {
        body *oldTail = tail;
        tail = tail->parent;
        free(oldTail);
    }
    free(head);

    int character[5][5];
    int bx = 2;
    int by = 1;
    do {
        //draw last digit
        if (getChar(character, size % 10)) {
            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                    if (character[y][x]) leds[x + bx * 5][y + by * 5].blue = 255;
                }
            }
        }
        //remove last digit
        size /= 10;
        bx--;
    } while (bx >= 0);
    updateMatrix();

    while (!btn_l && !btn_r) {}
    clear();
}

void snake_drawImage() {
    clear();
    leds[4][8].green = 255;
    leds[5][8].green = 255;
    leds[6][8].green = 255;
    leds[7][8].green = 255;
    leds[7][7].green = 255;
    leds[7][6].green = 255;
    leds[8][6].green = 255;
    leds[9][6].green = 255;
    leds[10][6].green = 255;
    leds[11][6].green = 255;
    leds[11][5].green = 255;

    for (int i = 0; i < STRIP; i++) {
        setLedsColor(0, i, BLUE);
        setLedsColor(i, 0, BLUE);
        setLedsColor(STRIP-1, i, BLUE);
        setLedsColor(i, STRIP-1, BLUE);
    }
    updateMatrix();
}

#endif //LEDTRIX_SNAKE_H

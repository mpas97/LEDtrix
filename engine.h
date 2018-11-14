#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <unistd.h>

#define CLEAR "\33[H\33[2J"
#define RESET "\33[0m"
#define   RED "\33[31m"
#define GREEN "\33[32m"
#define  BLUE "\33[34m"

#define   OFF 0
#define     R 1
#define     G 2
#define     B 3

#define STARL "| "
#define DIVID "- "
#define  ENDL "|\n"

#define  SIZE 15
#define TIMER 1000

void fill(char color);
void setLED(int x, int y, char c);
void printLED(int x, int y);
void printBorderLine();
void flushAndWait();

char matrix[SIZE][SIZE] = {OFF};

void fill(char color) {
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            matrix[y][x] = color;
        }
    }
}

void setLED(int x, int y, char c) {
    matrix[y][x] = c;
}

void printLED(int x, int y) {
    switch (matrix[y][x]) {
        case R:
            printf(RED);
            break;
        case G:
            printf(GREEN);
            break;
        case B:
            printf(BLUE);
            break;
        case OFF:
        default:
            printf(RESET);
            break;
    }
    printf("0 ");
}

void printBorderLine() {
    printf(STARL);
    for (int b = 0; b < SIZE; b++) printf(DIVID);
    printf(ENDL);
}

void print() {
    printf(CLEAR);
    printBorderLine();
    for (int y = 0; y < SIZE; y++) {
        printf(STARL);
        for (int x = 0; x < SIZE; x++) {
            printLED(x, y);
        }
        printf(RESET);
        printf(ENDL);
    }
    printBorderLine();
    flushAndWait();
}

void flushAndWait() {
    fflush(stdout);
    usleep(TIMER * 1000);
}
#endif //ENGINE_H
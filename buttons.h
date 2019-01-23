#ifndef LEDTRIX_BUTTONS_H
#define LEDTRIX_BUTTONS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <wiringPi.h>

#define HALF_S_IN_U 500000
#define S_IN_U      1000000
#define BTNL 25
#define BTNR 27


volatile bool btn_l = false;
volatile bool btn_r = false;

struct timeval last_change_l;
struct timeval last_change_r;

bool getButtonClickLeft() {
    bool state = btn_l;
    btn_l = false;
    return state;
}

bool getButtonClickRight() {
    bool state = btn_r;
    btn_r = false;
    return state;
}

void handleLeft() {
    struct timeval now;
    unsigned long diff;

    gettimeofday(&now, NULL);

    // Time difference in usec
    diff = (now.tv_sec * S_IN_U + now.tv_usec) - (last_change_l.tv_sec * S_IN_U + last_change_l.tv_usec);

    // Filter jitter
    if (diff > S_IN_U) {
        btn_l = true;
        last_change_l = now;
    }
}

void handleRight() {
    struct timeval now;
    unsigned long diff;

    gettimeofday(&now, NULL);

    // Time difference in usec
    diff = (now.tv_sec * S_IN_U + now.tv_usec) - (last_change_r.tv_sec * S_IN_U + last_change_r.tv_usec);

    // Filter jitter
    if (diff > S_IN_U) {
        btn_r = true;
        last_change_r = now;
    }
}

void setupButtons() {
    wiringPiSetup();
    pinMode(BTNL, INPUT);
    pinMode(BTNR, INPUT);
    pullUpDnControl(BTNL, PUD_UP);
    pullUpDnControl(BTNR, PUD_UP);
    if (wiringPiISR(BTNL, INT_EDGE_FALLING, &handleLeft) < 0) {
        printf("ERROR: Unable to setup ISR button left!\n");
        exit(-1);
    }
    if (wiringPiISR(BTNR, INT_EDGE_FALLING, &handleRight) < 0) {
        printf("ERROR: Unable to setup ISR button right!\n");
        exit(-1);
    }
}

#endif //LEDTRIX_BUTTONS_H

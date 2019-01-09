#ifndef LEDTRIX_BUTTONS_H
#define LEDTRIX_BUTTONS_H

#include <stdio.h>
#include <sys/time.h>
#include <wiringPi.h>

#define IGNORE_CHANGE_BELOW_USEC 500000
#define BTNL 25
#define BTNR 27


void ttt_btn_left();
void ttt_btn_right();
//void rd_btn_left();
//void rd_btn_left();

struct timeval last_change_l;
int game;

void handleLeft() {
    struct timeval now;
    unsigned long diff;

    gettimeofday(&now, NULL);

    // Time difference in usec
    diff = (now.tv_sec * 1000000 + now.tv_usec) - (last_change_l.tv_sec * 1000000 + last_change_l.tv_usec);

    // Filter jitter
    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        printf("button left pressed!\n");
        switch(game) {
            case 0:
                //spiel auswählen.h
                break;
            case 1:
                ttt_btn_left();
                break;
        }
        last_change_l = now;
    }
}

struct timeval last_change_r;

void handleRight() {
    struct timeval now;
    unsigned long diff;

    gettimeofday(&now, NULL);

    // Time difference in usec
    diff = (now.tv_sec * 1000000 + now.tv_usec) - (last_change_r.tv_sec * 1000000 + last_change_r.tv_usec);

    // Filter jitter
    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        printf("button right pressed!\n");
        switch(game) {
            case 0:
                //spiel auswählen.h
                break;
            case 1:
                ttt_btn_right();
                break;
        }
        last_change_r = now;
    }
}

int setupButtons() {
    wiringPiSetup();
    pinMode(BTNL, INPUT);
    pinMode(BTNR, INPUT);
    pullUpDnControl(BTNL, PUD_UP);
    pullUpDnControl(BTNR, PUD_UP);
    if (wiringPiISR(BTNL, INT_EDGE_FALLING, &handleLeft) < 0) {
        printf("Unable to setup ISR button left!\n");
        return 1;
    }
    if (wiringPiISR(BTNR, INT_EDGE_FALLING, &handleRight) < 0) {
        printf("Unable to setup ISR button right!\n");
        return 1;
    }
}

#endif //LEDTRIX_BUTTONS_H

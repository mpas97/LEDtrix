#include "wiringPi.h"
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>

#define DIPIN 12
#define CLPIN 14
#define  SIZE 75
#define LIGHT 15

typedef struct LED {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} LED;

void sendStartFrame();
void sendEndFrame();
void sendLedFrame(int red, int green, int blue);
void update();
void sendBrightness(int light);
void sendOnes(int n);
void sendZeros(int n);
void sendByte(int val);
void sendBit(int val);
void fill(int red, int green, int blue);
void setLed(int pos, int red, int green, int blue);
void *outputThread(LED matrix[]);

LED matrix[SIZE] = {0};

int main(int argc, char *argv[]) {
    wiringPiSetup();
    pinMode(DIPIN, OUTPUT);
    pinMode(CLPIN, OUTPUT);
    pinMode(6, INPUT);
    pullUpDnControl(6, PUD_DOWN);

    time_t t;
    srand((unsigned) time(&t));

    if (argc == 2) {
        if (strcmp("clear", argv[1]) == 0) {
            sendStartFrame();
            for (int led = 0; led < SIZE; led++) {
                sendOnes(3);
                sendZeros(29);
            }
            sendEndFrame();
            return 0;
        }
    }

    /*int on = 0;
    fill(255,105,180);
    while(1) {
        update();
        (on > SIZE/3 && on < SIZE*3/2) ? delay(500) : delay(1000);
        on++;
        if (on == SIZE) on = 0;
    }*/

    while(1) {
        for (int pos = 0; pos < SIZE; pos++) {
            setLed(pos, rand() % 256, rand() % 256, rand() % 256);
        }
        update();
        delay(5000);
    }
    return 0 ;
}

pthread_t current;

void update() {
    pthread_t next;
    // terminate the actual output thread
    pthread_cancel(current);
    // start a new output thread
    pthread_create(&next, NULL, (void *) outputThread, (void *) matrix);
    // detach it, so that memory is released after cancel
    pthread_detach(next);
    current = next;
}

void *outputThread(LED data[]) {
    int pos = 0;
    while(1) {
        if (digitalRead(6)) printf("button pressed");
        sendStartFrame();
        for (int led = 0; led < SIZE; led++) {
            if (pos == led) {
                sendLedFrame(data[led].red, data[led].green, data[led].blue);
            } else {
                sendLedFrame(0, 0, 0);
            }
        }
        sendEndFrame();
        pos++;
        if (pos==SIZE) pos = 0;
        delay(500);
    }
}

void setLed(int pos, int red, int green, int blue) {
    matrix[pos].red = (unsigned char) red;
    matrix[pos].green = (unsigned char) green;
    matrix[pos].blue = (unsigned char) blue;
}

void fill(int red, int green, int blue) {
    for (int pos = 0; pos < SIZE; pos++) {
        setLed(pos, red, green, blue);
    }
}

void sendStartFrame() {
    sendZeros(32);
}

void sendEndFrame() {
    sendOnes(SIZE);
}

void sendLedFrame(int red, int green, int blue) {
    if (red == 0 && green == 0 && blue == 0) {
        sendBrightness(0);
        sendZeros(24);
    } else {
        sendBrightness(LIGHT);
        sendByte(blue);
        sendByte(green);
        sendByte(red);
    }
}

void sendBrightness(int light) {
    sendByte(0xE0 + light);
}

void sendOnes(int n) {
    for (int i = 0; i < n; i++) {
        sendBit(1);
    }
}

void sendZeros(int n) {
    for (int i = 0; i < n; i++) {
        sendBit(0);
    }
}

void sendByte(int val) {
    for (int shift = 7; shift >= 0; shift--) {
        sendBit(val>>shift);
    }
}

void sendBit(int val) {
    digitalWrite(CLPIN, LOW);
    digitalWrite(DIPIN, val&1);
    digitalWrite(CLPIN, HIGH);
}
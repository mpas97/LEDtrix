#ifndef LEDTRIX_LEDS_H
#define LEDTRIX_LEDS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>

#define BLOCK_SIZE 4096
#define GPIO_BASE 0x200000
#define CLPIN 17
#define C3PIN 4
#define D1PIN 27
#define D2PIN 22
#define D3PIN 25
#define SIZE 225
#define BLOCK SIZE/3
#define STRIP BLOCK/5

int mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g, a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

struct color leds[STRIP][STRIP];
uint32_t matrix[SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void setLed(int pos, uint8_t red, uint8_t green, uint8_t blue);

void setLedsRGB(int xpos, int ypos, uint8_t red, uint8_t green, uint8_t blue);

void setLedsColor(int xpos, int ypos, struct color col);

void setup_io();

void sendStartFrame();

void send_32_bits(uint32_t val1, uint32_t val2, uint32_t val3);

void fill(uint8_t red, uint8_t green, uint8_t blue);

void clear();

void demo();

void update();

void updateMatrix();

void *outputThread(uint32_t data[]);

pthread_t current;
pthread_mutex_t run = PTHREAD_MUTEX_INITIALIZER;

/**
 * Replaces the continuous led frame sending thread.
 * So that the current colors will be visible.
 */
void update() {
    pthread_t next;
    // terminate the actual output thread
    pthread_cancel(current);
    // block until the previous thread got cancelled
    pthread_mutex_lock(&run);
    // start a new output thread
    pthread_create(&next, NULL, (void *) outputThread, (void *) matrix);
    // detach it, so that memory is released after cancel
    pthread_detach(next);
    current = next;
}

/**
 * Sends a start frame, SIZE-times a LED frame followed by an end frame.
 * This is repeated until the thread got cancelled.
 *
 * @param data all the colors for the leds
 */
void *outputThread(uint32_t data[]) {
    pthread_cleanup_push((void *) pthread_mutex_unlock, (void *) &run);
        //int pos = 0;
        int led = 0;
        while (1) {
            pthread_mutex_lock(&mutex);
            for (int pos = 0; pos < BLOCK - 2 ; pos += 4) {
                sendStartFrame();
                for (led = 0; led < pos; led++) {
                    send_32_bits(0x80000000, 0x80000000, 0x80000000);
                }
                send_32_bits(data[led], data[led + BLOCK], data[led + 2 * BLOCK]);
                send_32_bits(data[++led], data[led + BLOCK], data[led + 2 * BLOCK]);
                send_32_bits(data[++led], data[led + BLOCK], data[led + 2 * BLOCK]);
                send_32_bits(data[++led], data[led + BLOCK], data[led + 2 * BLOCK]);
                for (led++; led < BLOCK; led++) {
                    send_32_bits(0x80000000, 0x80000000, 0x80000000);
                }
                //pos += 4;
                //if (pos > BLOCK - 3) pos = 0;
            }
            pthread_mutex_unlock(&mutex);
            pthread_testcancel();
        }
    pthread_cleanup_pop(1);
}

/**
 * set all leds so the Matrix is displayed correctly.
 *
 * @param leds the matrix
 */
void updateMatrix() {
    for (int i = 0; i < STRIP; i++) {
        //(x,y) = (0,0) is upper left corner
        setLed(i, leds[4][STRIP - i - 1].red, leds[4][STRIP - i - 1].green, leds[4][STRIP - i - 1].blue); //5th strip
        setLed(i + STRIP, leds[3][i].red, leds[3][i].green, leds[3][i].blue); //4th Strip
        setLed(i + 2 * STRIP, leds[2][STRIP - i - 1].red, leds[2][STRIP - i - 1].green,
               leds[2][STRIP - i - 1].blue); //3rd Strip
        setLed(i + 3 * STRIP, leds[1][i].red, leds[1][i].green, leds[1][i].blue); //2nd Strip
        setLed(i + 4 * STRIP, leds[0][STRIP - i - 1].red, leds[0][STRIP - i - 1].green,
               leds[0][STRIP - i - 1].blue); //1st Strip
        setLed(i + 5 * STRIP, leds[5][STRIP - i - 1].red, leds[5][STRIP - i - 1].green,
               leds[5][STRIP - i - 1].blue); //6th Strip
        setLed(i + 6 * STRIP, leds[6][i].red, leds[6][i].green, leds[6][i].blue); //7th Strip
        setLed(i + 7 * STRIP, leds[7][STRIP - i - 1].red, leds[7][STRIP - i - 1].green,
               leds[7][STRIP - i - 1].blue); //8th Strip
        setLed(i + 8 * STRIP, leds[8][i].red, leds[8][i].green, leds[8][i].blue); //9th Strip
        setLed(i + 9 * STRIP, leds[9][STRIP - i - 1].red, leds[9][STRIP - i - 1].green,
               leds[9][STRIP - i - 1].blue); //10th Strip
        setLed(i + 10 * STRIP, leds[10][STRIP - i - 1].red, leds[10][STRIP - i - 1].green,
               leds[10][STRIP - i - 1].blue); //11th Strip
        setLed(i + 11 * STRIP, leds[11][i].red, leds[11][i].green, leds[11][i].blue); //12th Strip
        setLed(i + 12 * STRIP, leds[12][STRIP - i - 1].red, leds[12][STRIP - i - 1].green,
               leds[12][STRIP - i - 1].blue); //13th Strip
        setLed(i + 13 * STRIP, leds[13][i].red, leds[13][i].green, leds[13][i].blue); //14th Strip
        setLed(i + 14 * STRIP, leds[14][STRIP - i - 1].red, leds[14][STRIP - i - 1].green,
               leds[14][STRIP - i - 1].blue); //15th Strip
    }
    update();
}

/**
 * Changes the color of a specific LED.
 *
 * @param pos of the led
 * @param red
 * @param green
 * @param blue
 */
void setLed(int pos, uint8_t red, uint8_t green, uint8_t blue) {
    matrix[pos] = 0x8F000000;
    matrix[pos] |= red;
    matrix[pos] |= green << 8;
    matrix[pos] |= blue << 16;
}

/**
* Set the Color of the led on this position (x/y).
* @param xpos
* @param ypos
* @param red value
* @param green value
* @param blue value
*/
void setLedsRGB(int xpos, int ypos, uint8_t red, uint8_t green, uint8_t blue) {
    leds[xpos][ypos].red = red;
    leds[xpos][ypos].green = green;
    leds[xpos][ypos].blue = blue;
}

/**
 * Set the Color of the led on this position (x/y).
 * @param xpos
 * @param ypos
 * @param col of this led
 */
void setLedsColor(int xpos, int ypos, struct color col) {
    leds[xpos][ypos] = col;
}



/**
 * Sets all LED matrix entries to the same color.
 *
 * @param red
 * @param green
 * @param blue
 * @param leds the matrix
 */
void fill(uint8_t red, uint8_t green, uint8_t blue) {
    sendStartFrame();
    for (int y = 0; y < STRIP; y++) {
        for (int x = 0; x < STRIP; x++) {
            setLedsRGB(x, y, red, green, blue);
        }
    }
    updateMatrix();
}

/**
 * Turns all LEDs off.
 * Sends a start frame, SIZE-times an all zero LED frame followed by an end frame.
 *
 * @param leds
*/
void clear() {
    sendStartFrame();
    for (int y = 0; y < STRIP; y++) {
        for (int x = 0; x < STRIP; x++) {
            setLedsRGB(x, y, 0, 0, 0);
        }
    }
    updateMatrix();
    sleep(1);
}

/**
 * Sends all 32 bits of an unit32 param over GPIO.
 * The order is from highest to lowest bit.
 *
 * @param val1 color led in first block
 * @param val2 color led in second block
 * @param val3 color led in third block
 */
void send_32_bits(uint32_t val1, uint32_t val2, uint32_t val3) {
    for (int shift = 0; shift < 32; shift++) {
        GPIO_CLR = 1 << C3PIN;
        // block 1
        if (val1 & 0x80000000)
            GPIO_SET = 1 << D1PIN;
        else
            GPIO_CLR = 1 << D1PIN;
        GPIO_SET = 1 << C3PIN;

        // clock
        GPIO_CLR = 1 << CLPIN;

        // block 2
        if (val2 & 0x80000000)
            GPIO_SET = 1 << D2PIN;
        else
            GPIO_CLR = 1 << D2PIN;
        // block 3
        if (val3 & 0x80000000)
            GPIO_SET = 1 << D3PIN;
        else
            GPIO_CLR = 1 << D3PIN;
        // clock
        GPIO_SET = 1 << CLPIN;

        val1 <<= 1;
        val2 <<= 1;
        val3 <<= 1;
    }
}

/**
 * Sends a start frame.
 * This is at least a 32 bit long sequence of zeros.
 */
void sendStartFrame() {
    send_32_bits(0, 0, 0);
}

/**
 * Sets up memory regions to access GPIO.
 */
void setup_io() {

    // only access gpio memory space
    if ((mem_fd = open("/dev/gpiomem", O_RDWR | O_SYNC)) < 0) {
        // if failed, try to access whole memory space (requires sudo)
        if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
            printf("Can't open /dev/mem nor /dev/gpiomem. Try again with sudo!\n");
            exit(-1);
        }
    }

    // mmap GPIO
    gpio_map = mmap(
            NULL,                // Any address in our space will do
            BLOCK_SIZE,          // Map length
            PROT_READ | PROT_WRITE,// Enable reading & writing to mapped memory
            MAP_SHARED,          // Shared with other processes
            mem_fd,              // File to map
            GPIO_BASE            // Offset to GPIO peripheral
    );

    close(mem_fd); // No need to keep mem_fd open after mmap

    if (gpio_map == MAP_FAILED) {
        printf("mmap error %d\n", (int) gpio_map); // errno also set!
        exit(-1);
    }

    // Always use volatile pointer!
    gpio = (volatile unsigned *) gpio_map;

    // must use INP_GPIO before we can use OUT_GPIO
    INP_GPIO(D1PIN);
    OUT_GPIO(D1PIN);
    INP_GPIO(D2PIN);
    OUT_GPIO(D2PIN);
    INP_GPIO(D3PIN);
    OUT_GPIO(D3PIN);
    INP_GPIO(CLPIN);
    OUT_GPIO(CLPIN);
    INP_GPIO(C3PIN);
    OUT_GPIO(C3PIN);

}

#endif //LEDTRIX_LEDS_H
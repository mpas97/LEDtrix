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
#define DOPIN 27
#define  SIZE 60

// compile: gcc leds.c -o leds -lpthread

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

uint32_t matrix[SIZE];
void fill(uint8_t red, uint8_t green, uint8_t blue);
void setLed(int pos, uint8_t red, uint8_t green, uint8_t blue);

void setup_io();
void sendStartFrame();
void sendEndFrame();
void send_32_bits(uint32_t val);
void clear();
void update();
void *outputThread(uint32_t data[]);

void printButton(int g) {
    if (GET_GPIO(g)) // !=0 <-> bit is 1 <- port is HIGH=3.3V
        printf("Button pressed!\n");
    else // port is LOW=0V
        printf("Button released!\n");
}

int main(int argc, char **argv) {

    // Set up gpio pointer for direct register access
    setup_io();

    // must use INP_GPIO before we can use OUT_GPIO
    INP_GPIO(DOPIN);
    OUT_GPIO(DOPIN);
    INP_GPIO(CLPIN);
    OUT_GPIO(CLPIN);

    if (argc == 2) {
        if (strcmp("clear", argv[1]) == 0) {
            clear();
            return 0;
        }
    }

    while(1) {
        fill(255, 0, 0);
        update();
        sleep(3);
        fill(0, 255, 0);
        update();
        sleep(3);
        fill(0, 0, 255);
        update();
        sleep(3);
    }

    return 0;

}

pthread_t current;
pthread_mutex_t run = PTHREAD_MUTEX_INITIALIZER;

/*
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

/*
 * Sends a start frame, SIZE-times a LED frame followed by an end frame.
 * This is repeated until the thread got cancelled.
 */
void *outputThread(uint32_t data[]) {
    pthread_cleanup_push((void *) pthread_mutex_unlock, (void *) &run);
        int pos = 0;
        while (1) {
            sendStartFrame();
            for (int led = 0; led < SIZE; led++) {
                if (led == pos) {
                    send_32_bits(data[led]);
                    send_32_bits(data[++led]);
                    send_32_bits(data[++led]);
                    send_32_bits(data[++led]);
                    send_32_bits(data[++led]);
                } else {
                    send_32_bits(0x80000000);
                }
            }
            pos += 5;
            if (pos >= SIZE-4) pos = 0;
            pthread_testcancel();
        }
    pthread_cleanup_pop(1);
}

/*
 * Changes the color of a specific LED.
 */
void setLed(int pos, uint8_t red, uint8_t green, uint8_t blue) {
    matrix[pos]  = 0x8F000000;
    matrix[pos] |= red;
    matrix[pos] |= green<<8;
    matrix[pos] |= blue<<16;
}

/*
 * Sets all LED matrix entries to the same color.
 */
void fill(uint8_t red, uint8_t green, uint8_t blue) {
    for (int pos = 0; pos < SIZE; pos++) {
        setLed(pos, red, green, blue);
    }
}

/*
 * Sends all 32 bits of an unit32 param over GPIO.
 * The order is from highest to lowest bit.
 */
void send_32_bits(uint32_t val) {
    for (int shift = 0; shift < 32; shift++) {
        GPIO_CLR = 1<<CLPIN;
        if (val&0x80000000)
            GPIO_SET = 1<<DOPIN;
        else
            GPIO_CLR = 1<<DOPIN;
        GPIO_SET = 1<<CLPIN;
        val<<=1;
    }
}

/*
 * Sends a start frame.
 * This is at least a 32 bit long sequence of zeros.
 */
void sendStartFrame() {
    send_32_bits(0);
}

/*
 * Sends an end frame.
 * This is at least a (SIZE/2+1) bit long sequence of ones.
 */
void sendEndFrame() {
    for (int i = 0; i <= (SIZE/2 + 1)>>5; i++) {
        send_32_bits(0xFFFFFFFF);
    }
}

/*
 * Turns all LEDs off.
 * Sends a start frame, SIZE-times an all zero LED frame followed by an end frame.
 */
void clear() {
    sendStartFrame();
    for (int led = 0; led < SIZE; led++) {
        send_32_bits(0x80000000);
    }
    sendEndFrame();
}

/*
 * Sets up memory regions to access GPIO.
 */
void setup_io() {

    // only access gpio memory space
    if ((mem_fd = open("/dev/gpiomem", O_RDWR|O_SYNC)) < 0) {
        // if failed, try to access whole memory space (requires sudo)
        if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0) {
            printf("Can't open /dev/mem nor /dev/gpiomem. Try again with sudo!\n");
            exit(-1);
        }
    }

    // mmap GPIO
    gpio_map = mmap(
            NULL,                // Any address in our space will do
            BLOCK_SIZE,          // Map length
            PROT_READ|PROT_WRITE,// Enable reading & writing to mapped memory
            MAP_SHARED,          // Shared with other processes
            mem_fd,              // File to map
            GPIO_BASE            // Offset to GPIO peripheral
    );

    close(mem_fd); // No need to keep mem_fd open after mmap

    if (gpio_map == MAP_FAILED) {
        printf("mmap error %d\n", (int)gpio_map); // errno also set!
        exit(-1);
    }

    // Always use volatile pointer!
    gpio = (volatile unsigned *)gpio_map;

}
#ifndef LEDTRIX_COLOR_H
#define LEDTRIX_COLOR_H

#include <stdint.h>

typedef struct color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color;

const color WHITE   = {0, 0, 0};
const color RED     = {255,0,0};
const color GREEN   = {0,255,0};
const color BLUE    = {0,0,255};

void setColor(color *col, uint8_t red, uint8_t green, uint8_t blue) {
    col->red = red;
    col->green = green;
    col->blue = blue;
}

#endif //LEDTRIX_COLOR_H
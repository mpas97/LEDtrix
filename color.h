#ifndef LEDTRIX_COLOR_H
#define LEDTRIX_COLOR_H

#include <stdint.h>

typedef struct color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color;

void setColor(color *col, uint8_t red, uint8_t green, uint8_t blue) {
    col->red = red;
    col->green = green;
    col->blue = blue;
}

#endif //LEDTRIX_COLOR_H
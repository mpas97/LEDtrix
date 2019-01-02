#ifndef LEDTRIX_COLOR_H
#define LEDTRIX_COLOR_H

#endif //LEDTRIX_COLOR_H

struct color{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void setColor(struct color col, uint8_t red, uint8_t green, uint8_t blue) {
    col.red = red;
    col.green = green;
    col.blue = blue;
}

#include "utils.h"
#include <gb/gb.h>

uint8_t world2screen(int16_t world_coord) {
    return (uint8_t)(world_coord / UNITS_PER_PIXEL);
}
int16_t screen2world(uint8_t screen_cord) {
    return (int16_t)screen_cord * UNITS_PER_PIXEL;
}

int16_t clamp(int16_t val, int16_t lower, int16_t upper) {
    if (val < lower) {
        return lower;
    }
    if (val > upper) {
        return upper;
    }
    return val;
}
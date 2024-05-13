#include <gb/gb.h>
#include <stdio.h>

extern struct ObstacleAgent {

    int16_t x_pos;
    int16_t y_pos;
    uint8_t sprites_head[2];
    uint8_t sprites_base[2];
};
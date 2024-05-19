#ifndef UTILS_H
#define UTILS_H
#include <gb/gb.h>
#define UNITS_PER_PIXEL 8
#define TILE_SIZE 8
#define CEIL 2 * TILE_SIZE
#define FLOOR 16 * TILE_SIZE

extern uint8_t world2screen(int16_t world_coord);
extern int16_t screen2world(uint8_t screen_cord);
extern int16_t clamp(int16_t val, int16_t lower, int16_t upper);

#endif /*UTILS_H*/

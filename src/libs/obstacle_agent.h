#include <gb/gb.h>
#include <stdio.h>

extern struct ObstacleAgent {
    // Tile set data
    uint8_t sprite_bank_id;
    uint8_t tile_size;
    uint8_t tile_offset;
    uint8_t tile_set_size;

    // Position controllers
    int16_t x_pos;
    uint8_t x_speed;
    uint8_t height_in_tiles;
    BOOLEAN reverse;
    uint8_t width_in_tiles;
};

extern struct ObstacleAgent init_obstacle_agent(
    // Tile set data
    uint8_t sprite_bank_id,
    uint8_t tile_size,
    uint8_t tile_offset,
    uint8_t tile_set_size,
    unsigned char tiles[],

    // Position controllers
    int16_t x_pos,
    uint8_t x_speed,
    uint8_t height_in_tiles,
    BOOLEAN reverse,
    uint8_t width_in_tiles);

extern void draw_obstacle_agent(struct ObstacleAgent *agent);
extern void update_obstacle_agent(struct ObstacleAgent *agent);

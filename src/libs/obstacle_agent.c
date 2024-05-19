#include "obstacle_agent.h"
#include "../tiles/gate_tiles.h"
#include "utils.h"
#include <gb/gb.h>
#include <stdio.h>

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
    uint8_t width_in_tiles) {

    struct ObstacleAgent agent;
    set_sprite_data(tile_offset, tile_set_size, tiles);

    agent.sprite_bank_id = sprite_bank_id;
    agent.tile_size = tile_size;
    agent.tile_offset = tile_offset;
    agent.tile_set_size = tile_set_size;

    agent.x_pos = x_pos;
    agent.x_speed = x_speed;
    agent.height_in_tiles = height_in_tiles;
    agent.reverse = reverse;
    agent.width_in_tiles = width_in_tiles;

    agent.ceiling_bound_screen = CEIL;
    agent.floor_bound_screen = FLOOR;

    int16_t x_left = x_pos;
    int16_t y_top = reverse ? screen2world(agent.ceiling_bound_screen) : screen2world(agent.floor_bound_screen) - agent.height_in_tiles * screen2world(agent.tile_size);
    int16_t width = agent.width_in_tiles * screen2world(agent.tile_size);
    int16_t height = agent.height_in_tiles * screen2world(agent.tile_size);

    agent.hitbox = init_bbox(x_left, y_top, width, height);

    return agent;
}

extern void draw_obstacle_agent(struct ObstacleAgent *agent) {

    uint8_t nr_tiles = agent->height_in_tiles * agent->width_in_tiles;
    uint8_t x_screen = world2screen(agent->x_pos);
    uint8_t y_screen = agent->reverse ? agent->ceiling_bound_screen : agent->floor_bound_screen - (agent->height_in_tiles - 1) * agent->tile_size;

    for (uint8_t i = 0; i < nr_tiles; i++) {
        uint8_t tile_position = agent->reverse ? nr_tiles - i - 1 : i;
        uint8_t curr_tile_idx = 0;

        if (tile_position == 0) {
            curr_tile_idx = agent->reverse ? BASE_RIGHT_REVERSE : BASE_LEFT;
        } else if (tile_position == 1) {
            curr_tile_idx = agent->reverse ? BASE_LEFT_REVERSE : BASE_RIGHT;
        } else if (tile_position % 2 == 0) {
            curr_tile_idx = agent->reverse ? SHAFT_RIGHT_REVERSE : SHAFT_LEFT;

        } else {
            curr_tile_idx = agent->reverse ? SHAFT_LEFT_REVERSE : SHAFT_RIGHT;
        }
        set_sprite_tile(agent->sprite_bank_id + i, agent->tile_offset + curr_tile_idx);

        uint8_t x_offset = agent->tile_size * (i % 2);
        uint8_t y_offset = agent->tile_size * (i / 2);
        move_sprite(agent->sprite_bank_id + i, x_screen + x_offset, y_screen + y_offset);
    }
}
extern void update_obstacle_agent(struct ObstacleAgent *agent) {
    agent->x_pos -= agent->x_speed;
    int16_t new_x = agent->x_pos;
    int16_t new_y = agent->hitbox.y_top;
    update_bbox(&agent->hitbox, new_x, new_y);
}

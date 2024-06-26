#include "playable_agent.h"
#include "audio.h"
#include "utils.h"
#include <gb/gb.h>
#include <stdio.h>

struct PlayableAgent init_playable_agent(
    // Tile set data
    uint8_t sprite_bank_id,
    uint8_t first_animation_frame,
    uint8_t last_animation_frame,
    uint8_t tile_size,
    uint8_t tile_offset,
    uint8_t tile_set_size,
    unsigned char tiles[],
    // Animation controllers
    uint8_t nr_loops_per_frame,

    // Speed controllers
    int16_t max_x_speed,
    int16_t max_y_speed,

    // Acceleration controllers
    int16_t gravity,

    // Position controllers
    int16_t x_pos,
    int16_t y_pos,

    // State
    enum PlayableAgentState state) {
    struct PlayableAgent agent;

    set_sprite_data(tile_offset, tile_set_size, tiles);

    // Tile set data
    agent.sprite_bank_id = sprite_bank_id;
    agent.first_animation_frame = first_animation_frame;
    agent.last_animation_frame = last_animation_frame;
    agent.tile_size = tile_size;
    agent.tile_offset = tile_offset;
    agent.tile_set_size = tile_set_size;

    // Animation controllers
    agent.nr_loops_per_frame = nr_loops_per_frame;
    agent.frame_counter = first_animation_frame;
    agent.curr_loops = 0;

    // Speed controllers
    agent.curr_y_speed = 0;
    agent.max_x_speed = max_x_speed;
    agent.max_y_speed = max_y_speed;

    // Acceleration controllers
    agent.gravity = gravity;

    // Position controllers
    agent.x_pos = x_pos;
    agent.y_pos = y_pos;

    // State
    agent.state = state;

    int16_t x_left = x_pos;
    int16_t y_top = y_pos;
    int16_t width = screen2world(agent.tile_size);
    int16_t heigth = screen2world(agent.tile_size);

    agent.hitbox = init_bbox(x_left, y_top, width, heigth);

    move_sprite(sprite_bank_id, x_pos, y_pos);
    return agent;
}

void draw_playable_agent(struct PlayableAgent *agent) {
    // ANIMATION
    agent->curr_loops++;
    if (agent->curr_loops == agent->nr_loops_per_frame) {
        agent->curr_loops = 0;
        agent->frame_counter++;
        if (agent->frame_counter > agent->last_animation_frame) {
            agent->frame_counter = agent->first_animation_frame;
        }

        set_sprite_tile(agent->sprite_bank_id, agent->frame_counter + agent->tile_offset);
    }

    uint8_t x_screen = world2screen(agent->x_pos);
    uint8_t y_screen = world2screen(agent->y_pos);

    move_sprite(agent->sprite_bank_id, x_screen, y_screen);
}

//@TODO REFACTORIZAR, MEHJORAR CÓDIGO (SE REPITEN ASIGNACIONES)
// @TODO colisiones, pantalla de listo? puntaición
// Acabar de ver,
// refactorizar más.
// Definir colisión con entorno
// https://www.youtube.com/watch?v=T6vxF63JJaA&list=PLeEj4c2zF7PaFv5MPYhNAkBGrkx4iPGJo&index=8

// REFACTORIZAR
void update_playable_agent(struct PlayableAgent *agent, uint8_t key, uint8_t prevKey) {

    // PHYSICS
    if (agent->state != RISING) {

        int16_t nextSpeed = agent->curr_y_speed + agent->gravity;
        agent->curr_y_speed = clamp(nextSpeed, -agent->max_y_speed, agent->max_y_speed);
    }

    if (key && prevKey == 0) {
        agent->state = RISING;
        agent->curr_y_speed = -agent->max_y_speed;
        playFlap();
    }
    if (agent->state == RISING) {
        // Whenever a key is pressed
        // playCrash();

        // Jump
        agent->curr_y_speed = agent->curr_y_speed + agent->gravity;
        if (agent->curr_y_speed > 0) {
            agent->state = FALLING;
        }
    }
    agent->y_pos += agent->curr_y_speed;

    uint8_t ceiling_bound_screen = CEIL;
    uint8_t floor_bound_screen = FLOOR;
    int16_t ceiling_bound_world = screen2world(ceiling_bound_screen);
    int16_t floor_bound_world = screen2world(floor_bound_screen);

    // IF pos > techo
    if (agent->y_pos < ceiling_bound_world) {
        agent->y_pos = ceiling_bound_world;
    }
    // IF pos < suelo
    if (agent->y_pos >= floor_bound_world) {
        agent->y_pos = floor_bound_world;
        agent->state = DEAD;
    }
    int16_t new_x = agent->x_pos;
    int16_t new_y = agent->y_pos;
    update_bbox(&agent->hitbox, new_x, new_y);
}

extern void setState(struct PlayableAgent *agent, enum PlayableAgentState state) {
    agent->state = state;
}
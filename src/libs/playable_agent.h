#include <gb/gb.h>
#include <stdio.h>

extern enum PlayableAgentState {
    FALLING,
    RISING,
    DEAD,
};

extern struct PlayableAgent {
    // Tile set data
    uint8_t sprite_bank_id;
    uint8_t first_animation_frame;
    uint8_t last_animation_frame;
    uint8_t tile_size;
    uint8_t tile_offset;

    // Animation controllers
    uint8_t nr_loops_per_frame;
    uint8_t frame_counter;
    uint8_t curr_loops;

    // Speed controllers
    int16_t curr_y_speed;
    int16_t max_x_speed;
    int16_t max_y_speed;

    // Acceleration controllers
    int16_t gravity;

    // Position controllers
    int16_t x_pos;
    int16_t y_pos;

    // State
    enum PlayableAgentState state;
};
extern struct PlayableAgent init_agent(
    // Tile set data
    uint8_t sprite_bank_id,
    uint8_t first_animation_frame,
    uint8_t last_animation_frame,
    uint8_t tile_size,
    uint8_t tile_offset,
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
    enum PlayableAgentState state);
extern struct PlayableAgent update_agent(struct PlayableAgent el, uint8_t key, uint8_t prevKey);

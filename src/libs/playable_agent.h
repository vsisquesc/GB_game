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
    uint8_t tile_set_size;

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
extern struct PlayableAgent init_playable_agent(
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
    enum PlayableAgentState state);
extern void draw_playable_agent(struct PlayableAgent *agent);
extern void update_playable_agent(struct PlayableAgent *agent, uint8_t key, uint8_t prevKey);

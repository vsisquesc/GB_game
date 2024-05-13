#include <gb/gb.h>
#include <stdio.h>

extern enum PlayableAgentState {
    FALLING,
    RISING,
    DEAD,
};

extern struct PlayableAgent {
    // Animation controllers
    uint8_t nrLoopsPerFrame;
    uint8_t frameCounter;
    uint8_t currLoops;

    // Speed controllers
    int16_t curr_y_speed;
    int16_t max_y_speed;
    int16_t max_x_speed;

    // Acceleration controllers
    int16_t gravity;
    int16_t curr_boost;
    int16_t max_boost;

    // Position controllers
    int16_t x_pos;
    int16_t y_pos;

    // State
    enum PlayableAgentState state;
};
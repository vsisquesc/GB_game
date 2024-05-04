#include <gb/gb.h>     // If "gb/gb.h" file is not found, make sure the path to the gbdk include folder is set properly in ".vscode\c_cpp_properties.json"
#include <gbdk/font.h> // If "gb/gb.h" file is not found, make sure the path to the gbdk include folder is set properly in ".vscode\c_cpp_properties.json"
#include <stdio.h>

#include "libs/bg_map.h"
#include "libs/bg_tiles.h"
#include "libs/bird_tiles.h"
//  Direccion de los tiles en los que empieza cada tileset
//  El offset se debe a que sino se machacan los datos
//  de fuentes

// ============================================
// ===================WindowTiles==================
// ============================================

#define UNITS_PER_PIXEL 8

// @TODO hacer tipo de dato que represente floats
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

#define WINDOW_SIZE 11
unsigned char window_map[] =
    {
        0x10, // F
        0x16, // L
        0x0B, // A
        0x1A, // P
        0x1A, // P
        0x23, // Y
        0x00, //
        0x0C, // B
        0x13, // I
        0x1C, // R
        0x0E, // D
};

// ============================================
// ================= playCrash =================
// ============================================

void playCrash() {
    // bit 5-0 Sound length
    // 0001 1111 is 0x1F the maximum length
    NR41_REG = 0x1F;

    // volume envelope
    // bit 7-4 - Initial Volume of envelope (0-0Fh) (0=No Sound)
    // bit 3 - Envelope Direction (0=Decrease, 1=Increase)
    // bit 2-0 - Number of envelope sweep (n: 0-7) (If zero, stop envelope operation.)
    // 1111 0001 is 0xF1
    // 1111     -   start at full volume,
    // 0        -   fade down,
    // 001      -   1 envelope sweep  (decimal)

    NR42_REG = 0xF1;

    // bit 7-4 - Shift Clock Frequency (s)
    // bit 3   - Counter Step/Width (0=15 bits, 1=7 bits)
    // bit 2-0 - Dividing Ratio of Frequencies (r)
    // The amplitude is randomly switched between high and low at the given frequency.
    // A higher frequency will make the noise to appear 'softer'.
    // When Bit 3 is set, the output will become more regular, and some frequencies will sound more like Tone than Noise.
    // 0011 0000 is 0x30
    // 0011     -   shift clock frequency 3
    // 0        -   step 0
    // 000      -   dividing ratio 0

    NR43_REG = 0x30;

    // bit 7   - Initial (1=Restart Sound)
    // bit 6   - Controls if last forever or stops when NR41 ends
    // (1=Stop output when length in NR41 expires)
    // bit 5-0	Unused
    // 1100 0000 is 0xC0
    // 1        -       start sound
    // 1        -       not continuous
    // 00 0000  -       unused
    NR44_REG = 0xC0;
}

// ============================================
// ================= playFlap =================
// ============================================

void playFlap() {
    // see https://github.com/bwhitman/pushpin/blob/master/src/gbsound.txt
    // chanel 1 register 0, Frequency sweep settings
    // 7	Unused
    // 6-4	Sweep time(update rate) (if 0, sweeping is off)
    // 3	Sweep Direction (1: decrease, 0: increase)
    // 2-0	Sweep RtShift amount (if 0, sweeping is off)
    // 0001 0110 is 0x16
    // 0    -   Unused
    // 001  -   sweep time 1
    // 0    -   sweep direction increase
    // 110  -   shift ammount per step 110 (6 decimal)
    NR10_REG = 0x16;

    // chanel 1 register 1: Wave pattern duty and sound length
    // Channels 1 2 and 4
    // 7-6	Wave pattern duty cycle 0-3 (12.5%, 25%, 50%, 75%), duty cycle is how long a quadrangular  wave is "on" vs "off" so 50% (2) is both equal.
    // 5-0 sound length (higher the number shorter the sound)
    // 01000000 is 0x40,
    // 01       -   duty cycle 1 (25%),
    // 000000   -   wave length 0 (long)

    NR11_REG = 0x40;

    // chanel 1 register 2: Volume Envelope (Makes the volume get louder or quieter each "tick")
    // On Channels 1 2 and 4
    // 7-4	(Initial) Channel Volume
    // 3	Volume sweep direction (0: down; 1: up)
    // 2-0	Length of each step in sweep (if 0, sweeping is off)
    // NOTE: each step is n/64 seconds long, where n is 1-7
    // 0111 0011 is 0x73
    // 0111 -   volume 7
    // 0    -   sweep down
    // 011  -   step length 3

    NR12_REG = 0x73;

    // chanel 1 register 3: Frequency LSbs (Least Significant bits) and noise options
    // for Channels 1 2 and 3
    // 7-0	8 Least Significant bits of frequency (3 Most Significant Bits are set in register 4)
    NR13_REG = 0x00;

    // chanel 1 register 4: Playback and frequency MSbs
    // Channels 1 2 3 and 4
    // 7	Initialize (trigger channel start, AKA channel INIT) (Write only)
    // 6	Consecutive select/length counter enable (Read/Write). When "0", regardless of the length of data on the NR11 register, sound can be produced consecutively.  When "1", sound is generated during the time period set by the length data contained in register NR11.  After the sound is ouput, the Sound 1 ON flag, at bit 0 of register NR52 is reset.
    // 5-3	Unused
    // 2-0	3 Most Significant bits of frequency
    // 1100 0011 is 0xC3
    // 1    -    initialize
    // 1    -    no consecutive
    // 00 0 -    UNUSED
    // 011  -   frequency = MSB + LSB = 011 0000 0000 = 0x300

    NR14_REG = 0xC3;
}

// ============================================
// ================= menu =================
// ============================================

void menu() {
    // printf("menu");
}

// ============================================
// =================== Bird ===================
// ============================================

#define START_X 20
#define START_Y 70

enum AgentState {
    FALLING,
    RISING,
    DEAD,
};

struct Agent {
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
    enum AgentState state;
};

struct Agent bird;

//@TODO REFACTORIZAR, MEHJORAR CÓDIGO (SE REPITEN ASIGNACIONES)
// @TODO colisiones, pantalla de listo? puntaición
// Acabar de ver,
// refactorizar más.
// Definir velocidades máximas y míimmas en el controlador del pajarín
// Definir colisión con entorno
// Abstraer las velocidades del pajarín de las coordenadas de pantalla
// https://www.youtube.com/watch?v=T6vxF63JJaA&list=PLeEj4c2zF7PaFv5MPYhNAkBGrkx4iPGJo&index=8

// REFACTORIZAR
void game(uint8_t key, uint8_t prevKey) {
    // ANIMATION
    bird.currLoops++;
    if (bird.currLoops == bird.nrLoopsPerFrame) {
        bird.currLoops = 0;
        bird.frameCounter++;
        if (bird.frameCounter > BIRD_LAST_ANIMATION_FRAME) {
            bird.frameCounter = BIRD_FIRST_ANIMATION_FRAME;
        }

        set_sprite_tile(BIRD_ANIMATION_TILES_BANK, bird.frameCounter + BIRD_OFFSET);
    }
    // PHYSICS
    if (bird.state != RISING) {

        int16_t nextSpeed = bird.curr_y_speed + bird.gravity;
        bird.curr_y_speed = clamp(nextSpeed, -bird.max_y_speed, bird.max_y_speed);
    }

    if (key && prevKey == 0) {
        bird.state = RISING;
        bird.curr_y_speed = -bird.max_y_speed;
    }
    if (bird.state == RISING) {
        // Whenever a key is pressed
        // playCrash();
        // playFlap();

        // Jump
        bird.curr_y_speed = bird.curr_y_speed + bird.gravity;
        if (bird.curr_y_speed > 0) {
            bird.state = FALLING;
        }
    }
    bird.y_pos += bird.curr_y_speed;

    uint8_t ceiling_bound_screen = 10 + BIRD_TILE_SIZE;
    uint8_t floor_bound_screen = 120 + BIRD_TILE_SIZE;
    int16_t ceiling_bound_world = screen2world(ceiling_bound_screen);
    int16_t floor_bound_world = screen2world(floor_bound_screen);

    // IF pos > techo
    if (bird.y_pos < ceiling_bound_world) {
        bird.y_pos = ceiling_bound_world;
    }
    // IF pos < suelo
    if (bird.y_pos >= floor_bound_world) {
        bird.y_pos = floor_bound_world;
        bird.state = DEAD;
    }
    uint8_t x_screen = world2screen(bird.x_pos);
    uint8_t y_screen = clamp(world2screen(bird.y_pos), ceiling_bound_screen, floor_bound_screen);

    move_sprite(0, x_screen, y_screen);
    scroll_bkg(world2screen(bird.max_x_speed), 0);
}

// ============================================
// ================= pause =================
// ============================================

void pause() {
    // printf("pause");
}

// ============================================
// ================= GameLoop =================
// ============================================

enum GameState {
    MENU,
    GAME_SETUP,
    GAME,
    PAUSE
};

enum GameState state = GAME_SETUP;

void gameLoop(uint8_t key, uint8_t prevKey) {
    switch (state) {
    case MENU:
        /* code */
        menu();
        if (key & (J_START | J_SELECT | J_A | J_B)) {
            waitpadup();
            state = GAME_SETUP;
        }
        break;
    case GAME_SETUP:
        set_sprite_data(BIRD_OFFSET, BIRD_ANIMATION_SIZE, BirdAnimationTiles);

        set_bkg_data(MAP_OFFSET, BG_SIZE, bg_tiles);
        set_bkg_tiles(0, 0, BG_MAP_WIDTH, BG_MAP_HEIGHT, bg_map);

        set_win_tiles(0, 0, WINDOW_SIZE, 1, window_map);

        SHOW_SPRITES;
        SHOW_WIN;
        SHOW_BKG;

        int16_t start_x = screen2world(START_X);
        int16_t start_y = screen2world(START_Y);

        move_win(7, 120);
        move_sprite(0, START_X, START_Y);

        bird.nrLoopsPerFrame = 4;
        bird.frameCounter = BIRD_FIRST_ANIMATION_FRAME;
        bird.currLoops = 0;
        bird.curr_y_speed = 0;
        bird.max_y_speed = 30;
        bird.max_x_speed = 10;
        bird.gravity = 2;
        bird.x_pos = start_x;
        bird.y_pos = start_y;
        bird.state = FALLING;

        state = GAME;
        break;
    case GAME:
        game(key, prevKey);

        if (key & (J_START | J_SELECT)) {
            waitpadup();
            state = PAUSE;
        }
        break;
    case PAUSE:
        pause();
        if (key & J_A) {

            waitpadup();
            state = MENU;
        }
        break;

    default:
        break;
    }
}

// ============================================
// ================= main =================
// ============================================

void main(void) {

    //  Inicializacion del audio
    rAUDENA = AUDENA_ON; // 1000 0000 -> ENCIENDE EL SONIDO
    // Pone el volumen al máximo en ambos canales, izquierdo y derecho
    rAUDVOL = AUDVOL_VOL_LEFT(0x7) | AUDVOL_VOL_RIGHT(0x7);
    //  Canal 1: Quadrangular wave
    //  Canal 2: Quadrangular wave
    //  Canal 3: Programmable wave table
    //  Canal 4: Noise generator
    // 1111 1111 -> que canales queremos usar, un bit para cada canal. los primeros 4 bits son para el izquierdo y los segundos 4 para el canal derecho
    rAUDTERM = AUDTERM_4_LEFT | AUDTERM_3_LEFT | AUDTERM_2_LEFT | AUDTERM_1_LEFT |
               AUDTERM_4_RIGHT | AUDTERM_3_RIGHT | AUDTERM_2_RIGHT | AUDTERM_1_RIGHT;

    DISPLAY_ON;
    font_init();
    font_t min_font = font_load(font_min);
    font_set(min_font);

    setupMap();
    uint8_t key = joypad();
    uint8_t prevKey = 0x0;
    while (1) {

        gameLoop(key, prevKey);
        vsync();
        prevKey = key;
        key = joypad();
    }
}

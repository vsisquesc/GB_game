#include <gb/gb.h>     // If "gb/gb.h" file is not found, make sure the path to the gbdk include folder is set properly in ".vscode\c_cpp_properties.json"
#include <gbdk/font.h> // If "gb/gb.h" file is not found, make sure the path to the gbdk include folder is set properly in ".vscode\c_cpp_properties.json"
#include <stdio.h>

#include "libs/audio.h"
#include "libs/obstacle.h"
#include "libs/playable_agent.h"
#include "libs/utils.h"
#include "maps/bg_map.h"
#include "tiles/bg_tiles.h"
#include "tiles/bird_tiles.h"
//  Direccion de los tiles en los que empieza cada tileset
//  El offset se debe a que sino se machacan los datos
//  de fuentes

// ============================================
// ===================WindowTiles==================
// ============================================

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

struct ObstacleAgent obstacle;
struct PlayableAgent bird;

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
    bird = update_agent(bird, key, prevKey);
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
// Más factorizado
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

        move_win(7, 120);

        int16_t start_x = screen2world(START_X);
        int16_t start_y = screen2world(START_Y);

        bird = init_agent(
            BIRD_ANIMATION_TILES_BANK,
            BIRD_FIRST_ANIMATION_FRAME,
            BIRD_LAST_ANIMATION_FRAME,
            BIRD_TILE_SIZE,
            BIRD_OFFSET,
            4,  /* LOOPS PER FRAME*/
            10, /* MAX X SPEED */
            20, /* MAX Y SPEED */
            1,  /* GRAVITY */
            start_x,
            start_y,
            FALLING);

        obstacle.x_pos = 80;
        obstacle.y_pos = 100;

        set_sprite_tile(0, 0);
        obstacle.sprites_head[0] = 0;

        set_sprite_tile(1, 1);
        obstacle.sprites_head[1] = 1;

        set_sprite_tile(2, 2);
        obstacle.sprites_base[0] = 2;

        set_sprite_tile(3, 3);
        obstacle.sprites_base[1] = 3;

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

    setupAudio();
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

#include "raylib.h"

const int SPRITE_SIZE = 60; // in pixels

// black pieces
Rectangle black_queen = {.x = 0 * SPRITE_SIZE, .y = 0 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};
Rectangle black_king = {.x = 1 * SPRITE_SIZE, .y = 0 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};
Rectangle black_rook = {.x = 2 * SPRITE_SIZE, .y = 0 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};
Rectangle black_knight = {.x = 3 * SPRITE_SIZE, .y = 0 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};
Rectangle black_bishop = {.x = 4 * SPRITE_SIZE, .y = 0 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};
Rectangle black_pawn = {.x = 5 * SPRITE_SIZE, .y = 0 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};

// white pieces
Rectangle white_queen = {.x = 0 * SPRITE_SIZE, .y = 1 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};
Rectangle white_king = {.x = 1 * SPRITE_SIZE, .y = 1 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};
Rectangle white_rook = {.x = 2 * SPRITE_SIZE, .y = 1 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};
Rectangle white_knight = {.x = 3 * SPRITE_SIZE, .y = 1 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};
Rectangle white_bishop = {.x = 4 * SPRITE_SIZE, .y = 1 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};
Rectangle white_pawn = {.x = 5 * SPRITE_SIZE, .y = 1 * SPRITE_SIZE, .height = SPRITE_SIZE, .width = SPRITE_SIZE};

// chess board colors
Color CB_COLOR1 = (Color){.r = 118, .g = 150, .b = 86, .a = 255}; // #769656
Color CB_COLOR2 = (Color){.r = 238, .g = 238, .b = 210, .a = 255}; // #eeeed2
Color CB_COLOR3 = (Color){.r = 186, .g = 202, .b = 68, .a = 255}; // #baca44
Color CB_COLOR4 = (Color){.r = 186, .g = 152, .b = 68, .a = 255}; // #baca44 (modified for darker green)
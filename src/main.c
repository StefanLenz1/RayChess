#include "raylib.h"

void initializeChessBoard();

struct pieces {
	char piece;
	int player; // 0 is empty, 1 is player one, 2 is player two
};

#define BOARD_SIZE 8
struct pieces chess_board[BOARD_SIZE][BOARD_SIZE];
/*
e: empty
n: knight
b: bishop
q: queen
k: king
p: pawn
r: rook
*/

const int SQUARE_SIZE = 60; // in pixels

// chess board colors
Color CB_COLOR1 = (Color){.r = 118, .g = 150, .b = 86, .a = 255}; // #769656
Color CB_COLOR2 = (Color){.r = 238, .g = 238, .b = 210, .a = 255}; // #eeeed2

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

int main(void)
{
	const int screenWidth = SQUARE_SIZE * 8;
	const int screenHeight = SQUARE_SIZE * 8;

	InitWindow(screenWidth, screenHeight, "RayChess");
	SetTargetFPS(60);

	Texture2D cb_pieces = LoadTexture("../resources/chesspieces_spritesheet.png");
	initializeChessBoard();
	// Main game loop
	while (!WindowShouldClose()) {
		// Update

		// Draw
		BeginDrawing();

		ClearBackground(RAYWHITE);
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				// draw board
				Vector2 rectPos = (Vector2){.x = i * SQUARE_SIZE, .y = j * SQUARE_SIZE};
				if ((i % 2 == 0) && (j % 2 == 0) || (i % 2 == 1) && (j % 2 == 1)) // if both are even or both are odd
				{
					DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR1);
				} else {
					DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR2);
				}

				//draw pieces
				int player = chess_board[i][j].player;
				char piece = chess_board[i][j].piece;
				Rectangle source;
				if (player == 1) // white
				{
					switch(piece)
					{
						case 'p':
							source = white_pawn;
							break;
						case 'n':
							source = white_knight;
							break;
						case 'b':
							source = white_bishop;
							break;
						case 'r':
							source = white_rook;
							break;
						case 'q':
							source = white_queen;
							break;
						case 'k':
							source = white_king;
							break;
					}
				}

				if (player == 2) // black
				{
					switch(piece)
					{
						case 'p':
							source = black_pawn;
							break;
						case 'n':
							source = black_knight;
							break;
						case 'b':
							source = black_bishop;
							break;
						case 'r':
							source = black_rook;
							break;
						case 'q':
							source = black_queen;
							break;
						case 'k':
							source = black_king;
							break;
					}
				}

				if (player) // if not zero
					DrawTextureRec(cb_pieces, source, rectPos, WHITE);
			}
		}
		EndDrawing();
	}

	// De-Initialization
	CloseWindow();

	return 0;
}

void initializeChessBoard()
{
	// empty field
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			chess_board[i][j] = (struct pieces){.piece = 'e', .player = 0};

	// white pieces
	chess_board[0][0] = (struct pieces){.piece = 'r', .player = 1};
	chess_board[1][0] = (struct pieces){.piece = 'n', .player = 1};
	chess_board[2][0] = (struct pieces){.piece = 'b', .player = 1};
	chess_board[3][0] = (struct pieces){.piece = 'q', .player = 1};
	chess_board[4][0] = (struct pieces){.piece = 'k', .player = 1};
	chess_board[5][0] = (struct pieces){.piece = 'b', .player = 1};
	chess_board[6][0] = (struct pieces){.piece = 'n', .player = 1};
	chess_board[7][0] = (struct pieces){.piece = 'r', .player = 1};
	for (int i = 0; i < BOARD_SIZE; i++)
		chess_board[i][1] = (struct pieces){.piece = 'p', .player = 1};

	// black pieces
	chess_board[0][7] = (struct pieces){.piece = 'r', .player = 2};
	chess_board[1][7] = (struct pieces){.piece = 'n', .player = 2};
	chess_board[2][7] = (struct pieces){.piece = 'b', .player = 2};
	chess_board[3][7] = (struct pieces){.piece = 'q', .player = 2};
	chess_board[4][7] = (struct pieces){.piece = 'k', .player = 2};
	chess_board[5][7] = (struct pieces){.piece = 'b', .player = 2};
	chess_board[6][7] = (struct pieces){.piece = 'n', .player = 2};
	chess_board[7][7] = (struct pieces){.piece = 'r', .player = 2};
	for (int i = 0; i < BOARD_SIZE; i++)
		chess_board[i][6] = (struct pieces){.piece = 'p', .player = 2};
}
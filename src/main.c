#include "pieces.h"
#include "raylib.h"
#include "move_legal_checker.h"

struct pieces {
	char piece;
	int player; // 0 is empty, 1 is player one, 2 is player two
	bool isSelected;
	bool isMouseHovering;
};

void initializeChessBoard();
Rectangle getSourceSprite(struct pieces pieces);
void drawFrame();
void drawBoard();
void updateFrame(Vector2* mouse_position);
void getLegalMoves(struct pieces pieces, int collumn, int row);
void drawLegalMoves();

#define BOARD_SIZE 8
bool legal_moves[BOARD_SIZE][BOARD_SIZE];
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

Texture2D cb_pieces;
const int SQUARE_SIZE = 60; // in pixels

int main(void)
{
	const int screenWidth = SQUARE_SIZE * 8;
	const int screenHeight = SQUARE_SIZE * 8;

	InitWindow(screenWidth, screenHeight, "RayChess");
	SetTargetFPS(60);

	cb_pieces = LoadTexture("../resources/chesspieces_spritesheet.png");
	initializeChessBoard();
	Vector2 mouse_position = {.x = 0.0f, .y = 0.0f};

	// initialize legal move board
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			legal_moves[i][j] = false;

	// Main game loop
	while (!WindowShouldClose()) {
		updateFrame(&mouse_position);
		drawFrame();
	}

	// De-Initialization
	CloseWindow();

	return 0;
}

void initializeChessBoard()
{
	// empty field
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++) {
			chess_board[i][j] = (struct pieces){.piece = 'e', .player = 0};
			chess_board[i][j].isSelected = false;
			chess_board[i][j].isMouseHovering = false;
		}

	// white pieces
	chess_board[0][7] = (struct pieces){.piece = 'r', .player = 1};
	chess_board[1][7] = (struct pieces){.piece = 'n', .player = 1};
	chess_board[2][7] = (struct pieces){.piece = 'b', .player = 1};
	chess_board[3][7] = (struct pieces){.piece = 'k', .player = 1};
	chess_board[4][7] = (struct pieces){.piece = 'q', .player = 1};
	chess_board[5][7] = (struct pieces){.piece = 'b', .player = 1};
	chess_board[6][7] = (struct pieces){.piece = 'n', .player = 1};
	chess_board[7][7] = (struct pieces){.piece = 'r', .player = 1};
	for (int i = 0; i < BOARD_SIZE; i++)
		chess_board[i][6] = (struct pieces){.piece = 'p', .player = 1};

	// black pieces
	chess_board[0][0] = (struct pieces){.piece = 'r', .player = 2};
	chess_board[1][0] = (struct pieces){.piece = 'n', .player = 2};
	chess_board[2][0] = (struct pieces){.piece = 'b', .player = 2};
	chess_board[3][0] = (struct pieces){.piece = 'k', .player = 2};
	chess_board[4][0] = (struct pieces){.piece = 'q', .player = 2};
	chess_board[5][0] = (struct pieces){.piece = 'b', .player = 2};
	chess_board[6][0] = (struct pieces){.piece = 'n', .player = 2};
	chess_board[7][0] = (struct pieces){.piece = 'r', .player = 2};
	for (int i = 0; i < BOARD_SIZE; i++)
		chess_board[i][1] = (struct pieces){.piece = 'p', .player = 2};
}

void updateFrame(Vector2* mouse_position)
{
	*mouse_position = GetMousePosition();
	int collumn = mouse_position->x / SQUARE_SIZE;
	int row = mouse_position->y / SQUARE_SIZE;

	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			chess_board[i][j].isMouseHovering = false;
	chess_board[collumn][row].isMouseHovering = true;

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (chess_board[collumn][row].isSelected) {
			chess_board[collumn][row].isSelected = false;
		} else {
			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++)
					chess_board[i][j].isSelected = false;
			chess_board[collumn][row].isSelected = true;
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (chess_board[i][j].isSelected) {
				getLegalMoves(chess_board[i][j], i, j);
			}
		}
	}
}

void drawFrame()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);
	drawBoard();
	drawLegalMoves();

	EndDrawing();
}

void drawBoard()
{
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
			// draw hovered over field
			if (chess_board[i][j].isMouseHovering) {
				DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR4);
			}
			// draw clicked on field
			if (chess_board[i][j].isSelected) {
				DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR3);
			}
			// draw pieces
			Rectangle source = getSourceSprite(chess_board[i][j]);
			if (chess_board[i][j].player) // if not zero
				DrawTextureRec(cb_pieces, source, rectPos, WHITE);
		}
	}
}

void drawLegalMoves()
{
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Vector2 rectPos = (Vector2){.x = i * SQUARE_SIZE, .y = j * SQUARE_SIZE};
			if (legal_moves[i][j])
			{
				DrawCircle(rectPos.x, rectPos.y, 30.0f, BLACK);
			}
		}
	}
}

Rectangle getSourceSprite(struct pieces pieces)
{
	int player = pieces.player;
	char piece = pieces.piece;
	Rectangle source;
	if (player == 1) // white
	{
		switch (piece) {
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
		switch (piece) {
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
	return source;
}

void getLegalMoves(struct pieces pieces, int collumn, int row)
{
	int piece = pieces.piece;
	int player = pieces.player;
	switch (piece) {
	case 'p':
		setPawnMoves(piece, player, collumn, row);
		break;
	case 'n':
		//source = white_knight;
		break;
	case 'b':
		//source = white_bishop;
		break;
	case 'r':
		//source = white_rook;
		break;
	case 'q':
		//source = white_queen;
		break;
	case 'k':
		//source = white_king;
		break;
	}
}
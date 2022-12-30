#include "board_and_pieces.h"
#include "move_legal_checker.h"
#include "piece_sprites.h"
#include "raylib.h"

void initializeChessBoard();
Rectangle getSourceSprite(struct pieces pieces);
void drawFrame();
void drawBoard();
void updateFrame(Vector2* mouse_position, bool* pieceIsSelected);
void getLegalMoves(struct pieces pieces, int collumn, int row);
void drawLegalMoves();
void setLegalMoves();
void movePiece();
void resetBoardIsSelected();
void resetBoardIsHovering();
void resetLegalMoves();

Texture2D cb_pieces;
const int SQUARE_SIZE = 60; // in pixels

bool legal_moves[BOARD_SIZE][BOARD_SIZE];
struct pieces chess_board[BOARD_SIZE][BOARD_SIZE];

int main(void)
{
	// window initialization
	const int screenWidth = SQUARE_SIZE * 8;
	const int screenHeight = SQUARE_SIZE * 8;
	const int targetFps = 60;
	InitWindow(screenWidth, screenHeight, "RayChess");
	SetTargetFPS(targetFps);

	// resource loading
	cb_pieces = LoadTexture("../resources/chesspieces_spritesheet.png");

	// initialize cross function variables and arrays to 0 and false
	initializeChessBoard();
	Vector2 mouse_position = {.x = 0.0f, .y = 0.0f};
	bool pieceIsSelected = false;
	resetLegalMoves();

	// main game loop
	while (!WindowShouldClose()) {
		updateFrame(&mouse_position, &pieceIsSelected); // updates the game logic
		drawFrame(); // draws the frame
	}

	// de-initialization
	CloseWindow();
	return 0;
}

void initializeChessBoard()
{
	// empty board
	for (int collumn = 0; collumn < BOARD_SIZE; collumn++)
	{
		for (int row = 0; row < BOARD_SIZE; row++) {
			chess_board[collumn][row] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
			chess_board[collumn][row].isSelected = false;
			chess_board[collumn][row].isMouseHovering = false;
		}
	}

	// white pieces
	chess_board[0][7] = (struct pieces){.piece = ROOK, .player = WHITE_PLAYER};
	chess_board[1][7] = (struct pieces){.piece = KNIGHT, .player = WHITE_PLAYER};
	chess_board[2][7] = (struct pieces){.piece = BISHOP, .player = WHITE_PLAYER};
	chess_board[3][7] = (struct pieces){.piece = KING, .player = WHITE_PLAYER};
	chess_board[4][7] = (struct pieces){.piece = QUEEN, .player = WHITE_PLAYER};
	chess_board[5][7] = (struct pieces){.piece = BISHOP, .player = WHITE_PLAYER};
	chess_board[6][7] = (struct pieces){.piece = KNIGHT, .player = WHITE_PLAYER};
	chess_board[7][7] = (struct pieces){.piece = ROOK, .player = WHITE_PLAYER};
	for (int collumn = 0; collumn < BOARD_SIZE; collumn++)
		chess_board[collumn][6] = (struct pieces){.piece = PAWN, .player = WHITE_PLAYER};

	// black pieces
	chess_board[0][0] = (struct pieces){.piece = ROOK, .player = BLACK_PLAYER};
	chess_board[1][0] = (struct pieces){.piece = KNIGHT, .player = BLACK_PLAYER};
	chess_board[2][0] = (struct pieces){.piece = BISHOP, .player = BLACK_PLAYER};
	chess_board[3][0] = (struct pieces){.piece = KING, .player = BLACK_PLAYER};
	chess_board[4][0] = (struct pieces){.piece = QUEEN, .player = BLACK_PLAYER};
	chess_board[5][0] = (struct pieces){.piece = BISHOP, .player = BLACK_PLAYER};
	chess_board[6][0] = (struct pieces){.piece = KNIGHT, .player = BLACK_PLAYER};
	chess_board[7][0] = (struct pieces){.piece = ROOK, .player = BLACK_PLAYER};
	for (int collumn = 0; collumn < BOARD_SIZE; collumn++)
		chess_board[collumn][1] = (struct pieces){.piece = PAWN, .player = BLACK_PLAYER};
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
	for (int collumn = 0; collumn < BOARD_SIZE; collumn++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			Vector2 rectPos = (Vector2){.x = collumn * SQUARE_SIZE, .y = row * SQUARE_SIZE}; // target square

			// draw checkered pattern
			if ((collumn % 2 == 0) && (row % 2 == 0)
			  || (collumn % 2 == 1) && (row % 2 == 1)) // if both are even or both are odd
			{
				DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR1);
			} else {
				DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR2);
			}

			// draw hovered over square
			if (chess_board[collumn][row].isMouseHovering) {
				DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR4);
			}

			// draw clicked on square
			if (chess_board[collumn][row].isSelected) {
				DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR3);
			}
			// draw pieces
			Rectangle source = getSourceSprite(chess_board[collumn][row]);
			if (chess_board[collumn][row].player) // if not zero
				DrawTextureRec(cb_pieces, source, rectPos, WHITE);
		}
	}
}

void drawLegalMoves()
{
	for (int collumn = 0; collumn < BOARD_SIZE; collumn++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			Vector2 rectPos = (Vector2){.x = collumn * SQUARE_SIZE, .y = row * SQUARE_SIZE}; // target square

			if (legal_moves[collumn][row]) {
				DrawCircle(rectPos.x + SPRITE_SIZE / 2, rectPos.y + SPRITE_SIZE / 2, 10.0f,
				  (Color){.r = 0, .g = 121, .b = 255, .a = 100});
			}
		}
	}
}

void updateFrame(Vector2* mouse_position, bool* pieceIsSelected)
{
	/*
	1. click on an square with one of your pieces
	2. click on a legal square to move to
	-if clicked on the same square already clicked one it gets cancelled
	-if after clicking of piece one clicks on not legal square it gets cancelled
	-a square gets clicked when the mouse position is on it and the left mouse button gets pressed
	*/

	// update mouse position
	*mouse_position = GetMousePosition();
	int collumn = mouse_position->x / SQUARE_SIZE;
	int row = mouse_position->y / SQUARE_SIZE;

	// update mouse hovering
	resetBoardIsHovering();
	chess_board[collumn][row].isMouseHovering = true;

	// early return if mouse is not pressed
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		return;

	// cancel if piece is already selected
	if (chess_board[collumn][row].isSelected) {
		*pieceIsSelected = false;
		resetBoardIsSelected();
		resetLegalMoves();
		return;
	}

	if (*pieceIsSelected) {
		if (legal_moves[collumn][row]) {
			// legal move -> do it
			movePiece();
			*pieceIsSelected = false;
			resetBoardIsSelected();
			resetLegalMoves();
		} else {
			// illegal move -> cancel
			*pieceIsSelected = false;
			resetLegalMoves();
			resetBoardIsSelected();
		}
	} else {
		resetBoardIsSelected();
		resetLegalMoves();
		chess_board[collumn][row].isSelected = true;
		*pieceIsSelected = true;
		setLegalMoves();
	}
}

void resetBoardIsSelected()
{
	for (int collumn = 0; collumn < BOARD_SIZE; collumn++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			chess_board[collumn][row].isSelected = false;
		}
	}
}

void resetBoardIsHovering()
{
	for (int collumn = 0; collumn < BOARD_SIZE; collumn++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			chess_board[collumn][row].isMouseHovering = false;
		}
	}
}

void resetLegalMoves()
{
	for (int collumn = 0; collumn < BOARD_SIZE; collumn++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			legal_moves[collumn][row] = false;
		}
	}
}

Rectangle getSourceSprite(struct pieces pieces)
{
	int player = pieces.player;
	int piece = pieces.piece;
	Rectangle source;
	if (player == WHITE_PLAYER)
	{
		switch (piece) {
		case PAWN:
			source = white_pawn;
			break;
		case KNIGHT:
			source = white_knight;
			break;
		case BISHOP:
			source = white_bishop;
			break;
		case ROOK:
			source = white_rook;
			break;
		case QUEEN:
			source = white_queen;
			break;
		case KING:
			source = white_king;
			break;
		}
	}

	if (player == BLACK_PLAYER)
	{
		switch (piece) {
		case PAWN:
			source = black_pawn;
			break;
		case KNIGHT:
			source = black_knight;
			break;
		case BISHOP:
			source = black_bishop;
			break;
		case ROOK:
			source = black_rook;
			break;
		case QUEEN:
			source = black_queen;
			break;
		case KING:
			source = black_king;
			break;
		}
	}
	return source;
}

void setLegalMoves()
{
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (chess_board[i][j].isSelected) {
				getLegalMoves(chess_board[i][j], i, j);
			}
		}
	}
}

void getLegalMoves(struct pieces pieces, int collumn, int row)
{
	int piece = pieces.piece;
	int player = pieces.player;
	switch (piece) {
	case PAWN:
		setPawnMoves(player, collumn, row);
		break;
	case KNIGHT:
		// TODO
		break;
	case BISHOP:
		setBishopMoves(player, collumn, row);
		break;
	case ROOK:
		setRookMoves(player, collumn, row);
		break;
	case QUEEN:
		// TODO
		break;
	case KING:
		// TODO
		break;
	}
}

void movePiece()
{
	// initiating variables
	struct pieces piece;
	int piece_collumn, piece_row;
	struct pieces moveTo;
	int moveTo_collumn, moveTo_row;

	// assign value to variables
	for (int collumn = 0; collumn < BOARD_SIZE; collumn++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			// get row and collumn of selected piece
			if (chess_board[collumn][row].isSelected) {
				piece = chess_board[collumn][row];
				piece_collumn = collumn;
				piece_row = row;
			}
			// get row and collumn of clicked piece
			if (chess_board[collumn][row].isMouseHovering) {
				moveTo = chess_board[collumn][row];
				moveTo_collumn = collumn;
				moveTo_row = row;
			}
		}
	}

	// move pieces
	chess_board[piece_collumn][piece_row].isSelected = false;
	chess_board[moveTo_collumn][moveTo_row] = chess_board[piece_collumn][piece_row];
	chess_board[piece_collumn][piece_row] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
}
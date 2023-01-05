#include "main.h"
#include "chess_computer.h"
#include "move_legal_checker.h"
#include "piece_sprites.h"
#include "raylib.h"
#include <stdlib.h>

Texture2D cb_pieces;
Sound capture_piece;
Sound move_piece;
const int SQUARE_SIZE = 60; // in pixels

bool legal_moves[BOARD_SIZE][BOARD_SIZE];
struct pieces chess_board[BOARD_SIZE][BOARD_SIZE];
extern struct board* history_of_turns;

int main(void)
{
	// window initialization
	const int screenWidth = SQUARE_SIZE * 8;
	const int screenHeight = SQUARE_SIZE * 8;
	const int targetFps = 60;
	InitWindow(screenWidth, screenHeight, "RayChess");
	SetTargetFPS(targetFps);
	InitAudioDevice();

	// resource loading
	cb_pieces = LoadTexture("../resources/chesspieces_spritesheet.png");
	capture_piece = LoadSound("../resources/capture_piece.wav");
	move_piece = LoadSound("../resources/move_piece.wav");

	// initialize cross function variables and arrays to 0 and false
	initializeChessBoard();
	Vector2 mouse_position = {.x = 0.0f, .y = 0.0f};
	bool piece_is_selected = false;
	bool opponent_turn = false;
	bool game_is_over = false;
	struct temp_move previous_move;
	resetLegalMoves();
	initaliazeMoveHistory();

	// main game loop
	while (!WindowShouldClose()) {
		updateFrame(&mouse_position, &piece_is_selected, &opponent_turn, &game_is_over, &previous_move); // updates the game logic
		drawFrame(); // draws the frame
	}

	// de-initialization
	free(history_of_turns);
	UnloadTexture(cb_pieces);
	UnloadSound(capture_piece);
	UnloadSound(move_piece);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}

void initializeChessBoard()
{
	// empty board
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			chess_board[column][row] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
			chess_board[column][row].isSelected = false;
			chess_board[column][row].isMouseHovering = false;
		}
	}

	// white pieces
	chess_board[0][7] = (struct pieces){.piece = ROOK, .player = WHITE_PLAYER};
	chess_board[1][7] = (struct pieces){.piece = KNIGHT, .player = WHITE_PLAYER};
	chess_board[2][7] = (struct pieces){.piece = BISHOP, .player = WHITE_PLAYER};
	chess_board[3][7] = (struct pieces){.piece = QUEEN, .player = WHITE_PLAYER};
	chess_board[4][7] = (struct pieces){.piece = KING, .player = WHITE_PLAYER};
	chess_board[5][7] = (struct pieces){.piece = BISHOP, .player = WHITE_PLAYER};
	chess_board[6][7] = (struct pieces){.piece = KNIGHT, .player = WHITE_PLAYER};
	chess_board[7][7] = (struct pieces){.piece = ROOK, .player = WHITE_PLAYER};
	for (int column = 0; column < BOARD_SIZE; column++)
		chess_board[column][6] = (struct pieces){.piece = PAWN, .player = WHITE_PLAYER};

	// black pieces
	chess_board[0][0] = (struct pieces){.piece = ROOK, .player = BLACK_PLAYER};
	chess_board[1][0] = (struct pieces){.piece = KNIGHT, .player = BLACK_PLAYER};
	chess_board[2][0] = (struct pieces){.piece = BISHOP, .player = BLACK_PLAYER};
	chess_board[3][0] = (struct pieces){.piece = QUEEN, .player = BLACK_PLAYER};
	chess_board[4][0] = (struct pieces){.piece = KING, .player = BLACK_PLAYER};
	chess_board[5][0] = (struct pieces){.piece = BISHOP, .player = BLACK_PLAYER};
	chess_board[6][0] = (struct pieces){.piece = KNIGHT, .player = BLACK_PLAYER};
	chess_board[7][0] = (struct pieces){.piece = ROOK, .player = BLACK_PLAYER};
	for (int column = 0; column < BOARD_SIZE; column++)
		chess_board[column][1] = (struct pieces){.piece = PAWN, .player = BLACK_PLAYER};
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
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			Vector2 rectPos = (Vector2){.x = column * SQUARE_SIZE, .y = row * SQUARE_SIZE}; // target square

			// draw checkered pattern
			if ((column % 2 == 0) && (row % 2 == 0) || (column % 2 == 1) && (row % 2 == 1)) // if both are even or both are odd
			{
				DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR2);
			} else {
				DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR1);
			}

			// draw hovered over square
			if (chess_board[column][row].isMouseHovering) {
				DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR4);
			}

			// draw clicked on square
			if (chess_board[column][row].isSelected) {
				DrawRectangle(rectPos.x, rectPos.y, SQUARE_SIZE, SQUARE_SIZE, CB_COLOR3);
			}
			// draw pieces
			Rectangle source = getSourceSprite(chess_board[column][row]);
			if (chess_board[column][row].player) // if not zero
				DrawTextureRec(cb_pieces, source, rectPos, WHITE);
		}
	}
}

void drawLegalMoves()
{
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			Vector2 rectPos = (Vector2){.x = column * SQUARE_SIZE, .y = row * SQUARE_SIZE}; // target square

			if (legal_moves[column][row]) {
				DrawCircle(rectPos.x + SPRITE_SIZE / 2, rectPos.y + SPRITE_SIZE / 2, 10.0f,
				  (Color){.r = 0, .g = 121, .b = 255, .a = 100});
			}
		}
	}
}

void updateFrame(Vector2* mouse_position, bool* piece_is_selected, bool* opponent_turn, bool* game_is_over, struct temp_move* previous_move)
{
	if (*game_is_over) return; // game has ended

	if (checkWinner()) // no winner is zero
	{
		*game_is_over = true;
		return;
	}

	if (IsKeyPressed(KEY_R))
	{
		unMove();
		return;
	}

	// check if it is the opponents turn
	if (*opponent_turn) {
		opponentMove(previous_move);
		*opponent_turn = false;
		return;
	}

	/*
	1. click on an square with one of your pieces
	2. click on a legal square to move to
	-if clicked on the same square already clicked one it gets cancelled
	-if after clicking of piece one clicks on not legal square it gets cancelled
	-a square gets clicked when the mouse position is on it and the left mouse button gets pressed
	*/

	// update mouse position
	*mouse_position = GetMousePosition();
	int column = mouse_position->x / SQUARE_SIZE;
	int row = mouse_position->y / SQUARE_SIZE;

	// cancel if not white piece selected
	if ((chess_board[column][row].player != WHITE_PLAYER) && !(*piece_is_selected))
	{
		return;
	}

	// update mouse hovering
	resetBoardIsHovering();
	chess_board[column][row].isMouseHovering = true;

	// early return if mouse is not pressed
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		return;

	// cancel if piece is already selected
	if (chess_board[column][row].isSelected) {
		*piece_is_selected = false;
		resetBoardIsSelected();
		resetLegalMoves();
		return;
	}

	if (*piece_is_selected) {
		if (legal_moves[column][row]) {
			// legal move -> do it
			if (chess_board[column][row].player == NO_PLAYER)
				PlaySound(move_piece);
			else
				PlaySound(capture_piece);
			movePiece();

			// log moves
			previous_move->target_column = column;
			previous_move->target_row = row;

			// reset logs
			*piece_is_selected = false;
			resetBoardIsSelected();
			resetBoardIsHovering();
			resetLegalMoves();
			*opponent_turn = true;
		} else {
			// illegal move -> cancel
			*piece_is_selected = false;
			resetLegalMoves();
			resetBoardIsSelected();
		}
	} else {
		// select piece and get possible moves
		resetBoardIsSelected();
		resetLegalMoves();
		chess_board[column][row].isSelected = true;
		*piece_is_selected = true;
		setLegalMoves();

		//log moves
		previous_move->selected_column = column;
		previous_move->selected_row = row;
	}
}

void resetBoardIsSelected()
{
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			chess_board[column][row].isSelected = false;
		}
	}
}

void resetBoardIsHovering()
{
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			chess_board[column][row].isMouseHovering = false;
		}
	}
}

void resetLegalMoves()
{
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			legal_moves[column][row] = false;
		}
	}
}

Rectangle getSourceSprite(struct pieces pieces)
{
	int player = pieces.player;
	int piece = pieces.piece;
	Rectangle source;
	if (player == WHITE_PLAYER) {
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

	if (player == BLACK_PLAYER) {
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

void getLegalMoves(struct pieces pieces, int column, int row)
{
	int piece = pieces.piece;
	int player = pieces.player;
	switch (piece) {
	case PAWN:
		setPawnMoves(player, column, row);
		break;
	case KNIGHT:
		setKnightMoves(player, column, row);
		break;
	case BISHOP:
		setBishopMoves(player, column, row);
		break;
	case ROOK:
		setRookMoves(player, column, row);
		break;
	case QUEEN:
		setQueenMoves(player, column, row);
		break;
	case KING:
		setKingMoves(player, column, row);
		break;
	}
}

void movePiece()
{
	// initiating variables
	struct pieces piece;
	int piece_column; 
	int piece_row;
	struct pieces moveTo;
	int moveTo_column; 
	int moveTo_row;

	// assign value to variables
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			// get row and column of selected piece
			if (chess_board[column][row].isSelected) {
				piece = chess_board[column][row];
				piece_column = column;
				piece_row = row;
			}
			// get row and column of clicked piece
			if (chess_board[column][row].isMouseHovering) {
				moveTo = chess_board[column][row];
				moveTo_column = column;
				moveTo_row = row;
			}
		}
	}
	// castle rights
	int piece_type = chess_board[piece_column][piece_row].piece;
	int piece_player = chess_board[piece_column][piece_row].player;
	if (piece_type == KING)
		updateCastleRightsKing(piece_column, piece_row, piece_player);
	if (piece_type == ROOK)
		updateCastleRightsRook(piece_column, piece_row, piece_player);

	// castle
	if (piece_type == KING && chess_board[moveTo_column][moveTo_row].piece == ROOK) {
		castle(piece_column, piece_row, moveTo_column, moveTo_row, piece_player);
	} else {
		// move pieces
		chess_board[moveTo_column][moveTo_row] = chess_board[piece_column][piece_row];
		chess_board[moveTo_column][moveTo_row].isSelected = false;
		chess_board[moveTo_column][moveTo_row].isMouseHovering = false;
		chess_board[piece_column][piece_row] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER, .isSelected = false, .isMouseHovering = false};
	}

	saveMove();
}

void castle(int piece_column, int piece_row, int moveTo_column, int moveTo_row, int piece_player)
{
	if (piece_player == WHITE_PLAYER) {
		if (moveTo_column == 0) {
			chess_board[2][7] = (struct pieces){.piece = KING, .player = piece_player};
			chess_board[4][7] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
			chess_board[3][7] = (struct pieces){.piece = ROOK, .player = piece_player};
			chess_board[0][7] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
		}
		if (moveTo_column == 7) {
			chess_board[6][7] = (struct pieces){.piece = KING, .player = piece_player};
			chess_board[4][7] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
			chess_board[5][7] = (struct pieces){.piece = ROOK, .player = piece_player};
			chess_board[7][7] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
		}
	}
	if (piece_player == BLACK_PLAYER) {
		if (moveTo_column == 0) {
			chess_board[2][0] = (struct pieces){.piece = KING, .player = piece_player};
			chess_board[4][0] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
			chess_board[3][0] = (struct pieces){.piece = ROOK, .player = piece_player};
			chess_board[0][0] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
		}
		if (moveTo_column == 7) {
			chess_board[6][0] = (struct pieces){.piece = KING, .player = piece_player};
			chess_board[4][0] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
			chess_board[5][0] = (struct pieces){.piece = ROOK, .player = piece_player};
			chess_board[7][0] = (struct pieces){.piece = EMPTY, .player = NO_PLAYER};
		}
	}
}

void opponentMove(struct temp_move* previous_move)
{
	previous_move->value = 0;
	int player = BLACK_PLAYER;
	int minmax_depth = 4;
	struct temp_move move = bestMove(player, minmax_depth, *previous_move);
	chess_board[move.selected_column][move.selected_row].isSelected = true;
	chess_board[move.target_column][move.target_row].isMouseHovering = true;
	movePiece();
}

int checkWinner()
{
	// if no piece of a player is a king then the player has lost
	bool black_lost = true;
	bool white_lost = true;
	for (int column = 0; column < BOARD_SIZE; column++)
	{
		for (int row = 0; row < BOARD_SIZE; row++)
		{
			if(chess_board[column][row].player == WHITE_PLAYER && chess_board[column][row].piece == KING)
			{
				white_lost = false;
			}
			if(chess_board[column][row].player == BLACK_PLAYER && chess_board[column][row].piece == KING)
			{
				black_lost = false;
			}
		}
	}
	if (white_lost)
		return BLACK_PLAYER;
	if (black_lost)
		return WHITE_PLAYER;
	return NO_PLAYER;
}
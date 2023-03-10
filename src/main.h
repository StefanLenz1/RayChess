#include "raylib.h"
#include "board_and_pieces.h"

#ifndef MAIN
#define MAIN

struct temp_move {
	int selected_column;
	int selected_row;
	int target_column;
	int target_row;
	int value;
};

void initializeChessBoard();
Rectangle getSourceSprite(struct pieces pieces);
void drawFrame(bool* game_is_over);
void drawBoard();
void drawFinishScreen();
void updateFrame(Vector2* mouse_position, bool* piece_is_selected, bool* opponent_turn, bool* game_is_over, struct temp_move* previous_move);
void getLegalMoves(struct pieces pieces, int column, int row);
void drawLegalMoves();
void setLegalMoves();
void movePiece();
void resetBoardIsSelected();
void resetBoardIsHovering();
void resetLegalMoves();
void castle(int piece_column, int piece_row, int moveTo_column, int moveTo_row, int piece_player);
void opponentMove(struct temp_move* previous_move);
int checkWinner();
void promotion();

#endif
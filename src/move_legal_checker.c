#include "board_and_pieces.h"
#include "raylib.h"

enum isPiece { FROM_EMPTY, FROM_PLAYER, FROM_OPPONENT };

enum isPiece getPieceState(int column, int row, int player)
{
	if (chess_board[column][row].player == NO_PLAYER)
		return FROM_EMPTY;
	if (chess_board[column][row].player == player)
		return FROM_PLAYER;
	return FROM_OPPONENT;
}

bool addAndCheckMove(int column, int row, int player) // add moves and check for opponent pieces
{
	enum isPiece state = getPieceState(column, row, player);
	switch (state) {
	case FROM_EMPTY:
		// add move and continue
		legal_moves[column][row] = true;
		return false;
	case FROM_PLAYER:
		// break
		return true;
	case FROM_OPPONENT:
		// add move and break
		legal_moves[column][row] = true;
		return true;
	}
}

void setPawnMoves(int player, int column, int row)
{
	if (player == WHITE_PLAYER) {
		if (chess_board[column][row - 1].piece == EMPTY) {
			if (row == 6) // Did not move yet
			{
				legal_moves[column][row - 1] = true;
				if (chess_board[column][row - 2].piece == EMPTY)
					legal_moves[column][row - 2] = true;
			} else {
				legal_moves[column][row - 1] = true;
			}
		}

		int topLeftcolumn = column - 1;
		int topRightcolumn = column + 1;
		int topRow = row - 1;
		if (topRow >= 0) {
			if (topLeftcolumn >= 0) {
				if (getPieceState(topLeftcolumn, topRow, player) == FROM_OPPONENT) // top-left of pawn
					legal_moves[topLeftcolumn][topRow] = true;
			}
			if (topRightcolumn < BOARD_SIZE) {
				if (getPieceState(topRightcolumn, topRow, player) == FROM_OPPONENT) // top-right of pawn
					legal_moves[topRightcolumn][topRow] = true;
			}
		}
	}
	if (player == BLACK_PLAYER) {
		if (chess_board[column][row + 1].piece == EMPTY) {
			if (row == 1) // Did not move yet
			{
				legal_moves[column][row + 1] = true;
				if (chess_board[column][row + 2].piece == EMPTY)
					legal_moves[column][row + 2] = true;
			} else {
				legal_moves[column][row + 1] = true;
			}
		}
		int bottomLeftcolumn = column - 1;
		int bottomRightcolumn = column + 1;
		int bottomRow = row + 1;
		if (bottomRow < BOARD_SIZE) {
			if (bottomLeftcolumn >= 0) {
				if (getPieceState(bottomLeftcolumn, bottomRow, player) == FROM_OPPONENT) // bottom-left of pawn
					legal_moves[bottomLeftcolumn][bottomRow] = true;
			}
			if (bottomRightcolumn < BOARD_SIZE) {
				if (getPieceState(bottomLeftcolumn, bottomRow, player) == FROM_OPPONENT) // bottom-right of pawn
					legal_moves[bottomRightcolumn][bottomRow] = true;
			}
		}
	}
}
void setRookMoves(int player, int column, int row)
{
	// move up
	for (int i = row - 1; i >= 0; i--) {
		if (addAndCheckMove(column, i, player))
			break;
	}
	// move down
	for (int i = row + 1; i < BOARD_SIZE; i++) {
		if (addAndCheckMove(column, i, player))
			break;
	}
	// move left
	for (int i = column - 1; i >= 0; i--) {
		if (addAndCheckMove(i, row, player))
			break;
	}
	// move right
	for (int i = column + 1; i < BOARD_SIZE; i++) {
		if (addAndCheckMove(i, row, player))
			break;
	}
}

void setBishopMoves(int player, int column, int row)
{
	// move up-left
	for (int i = column - 1, j = row - 1; i >= 0 && j >= 0; i--, j--) {
		if (addAndCheckMove(i, j, player))
			break;
	}
	// move up-right
	for (int i = column + 1, j = row - 1; i < BOARD_SIZE && j >= 0; i++, j--) {
		if (addAndCheckMove(i, j, player))
			break;
	}
}
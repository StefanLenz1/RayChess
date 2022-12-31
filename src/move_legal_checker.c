#include "board_and_pieces.h"
#include "raylib.h"

struct knight_move_pairs {
	int x;
	int y;
};
const int amount_pairs = 8;
struct knight_move_pairs pairs[] = {
  [0] = {.x = 1, .y = 2},
  [1] = {.x = 1, .y = -2},
  [2] = {.x = 2, .y = 1},
  [3] = {.x = 2, .y = -1},
  [4] = {.x = -1, .y = 2},
  [5] = {.x = -1, .y = -2},
  [6] = {.x = -2, .y = 1},
  [7] = {.x = -2, .y = -1},
};

// b is black, w is white. l is long, s is short
bool cancastle_b_king = true;
bool cancastle_w_king = true;
bool cancastle_w_l_rook = true;
bool cancastle_w_s_rook = true;
bool cancastle_b_l_rook = true;
bool cancastle_b_s_rook = true;

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
	// move bottom-left
	for (int i = column - 1, j = row + 1; i >= 0 && j < BOARD_SIZE; i--, j++) {
		if (addAndCheckMove(i, j, player))
			break;
	}
	// move bottom-right
	for (int i = column + 1, j = row + 1; i < BOARD_SIZE && j < BOARD_SIZE; i++, j++) {
		if (addAndCheckMove(i, j, player))
			break;
	}
}

void setQueenMoves(int player, int column, int row)
{
	setRookMoves(player, column, row);
	setBishopMoves(player, column, row);
}

void setKingMoves(int player, int column, int row)
{
	// normal move
	for (int i = column - 1; i <= column + 1; i++) {
		for (int j = row - 1; j <= row + 1; j++) {
			if ((i >= 0) && (i < BOARD_SIZE) && (j >= 0) && (j < BOARD_SIZE)) // out of bound check
			{
				if (getPieceState(i, j, player) == FROM_PLAYER) {
					// skip pieces from the player
					continue;
				} else {
					legal_moves[i][j] = true;
				}
			}
		}
	}
	// castle move
	if (player == WHITE_PLAYER) {
		if (cancastle_w_king) {
			if (cancastle_w_l_rook) // long castle
			{
				bool queen_field_empty = (getPieceState(3, 7, player) == FROM_EMPTY);
				bool bishop_field_empty = (getPieceState(2, 7, player) == FROM_EMPTY);
				bool knight_field_empty = (getPieceState(1, 7, player) == FROM_EMPTY);
				if (queen_field_empty && bishop_field_empty && knight_field_empty)
					legal_moves[0][7] = true;
			}
			if (cancastle_w_s_rook) // short castle
			{
				bool bishop_field_empty = (getPieceState(5, 7, player) == FROM_EMPTY);
				bool knight_field_empty = (getPieceState(6, 7, player) == FROM_EMPTY);
				if (bishop_field_empty && knight_field_empty)
					legal_moves[7][7] = true;
			}
		}
	}
	if (player == BLACK_PLAYER) {
		if (cancastle_b_king) {
			if (cancastle_b_l_rook) // long castle
			{
				bool queen_field_empty = (getPieceState(3, 0, player) == FROM_EMPTY);
				bool bishop_field_empty = (getPieceState(2, 0, player) == FROM_EMPTY);
				bool knight_field_empty = (getPieceState(1, 0, player) == FROM_EMPTY);
				if (queen_field_empty && bishop_field_empty && knight_field_empty)
					legal_moves[0][0] = true;
			}
			if (cancastle_b_s_rook) // short castle
			{
				bool bishop_field_empty = (getPieceState(5, 0, player) == FROM_EMPTY);
				bool knight_field_empty = (getPieceState(6, 0, player) == FROM_EMPTY);
				if (bishop_field_empty && knight_field_empty)
					legal_moves[7][0] = true;
			}
		}
	}
}

void setKnightMoves(int player, int column, int row)
{
	for (int i = 0; i < amount_pairs; i++) {
		int x = pairs[i].x + column;
		int y = pairs[i].y + row;
		if ((x >= 0) && (x < BOARD_SIZE) && (y >= 0) && (y < BOARD_SIZE)) {
			if (getPieceState(x, y, player) == FROM_PLAYER) {
				// skip pieces from the player
				continue;
			} else {
				legal_moves[x][y] = true;
			}
		}
	}
}

void updateCastleRightsKing(int column, int row, int player)
{
	/*
	Black King: [4][0]
	White King: [4][7]
	*/
	if (player == WHITE_PLAYER) {
		cancastle_w_king = false;
	}
	if (player == BLACK_PLAYER) {
		cancastle_b_king = false;
	}
}
void updateCastleRightsRook(int column, int row, int player)
{
	/*
	White long rook: [0][7]
	White short rook: [7][7]
	Black long rook: [0][0]
	Black short rook: [7][0]
	*/
	if (player == WHITE_PLAYER) {
		if (column == 0)
			cancastle_w_l_rook = false;
		if (column == 7)
			cancastle_w_s_rook = false;
	}
	if (player == BLACK_PLAYER) {
		if (column == 0)
			cancastle_b_l_rook = false;
		if (column == 7)
			cancastle_b_s_rook = false;
	}
}
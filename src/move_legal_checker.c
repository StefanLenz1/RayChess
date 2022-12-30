#include "board_and_pieces.h"
#include "raylib.h"

bool isPieceFromPlayerOrEmpty(int collumn, int row, int player)
{
	return (chess_board[collumn][row].player == player || chess_board[collumn][row].player == NO_PLAYER);
}

bool isPieceFromOpponent(int collumn, int row, int player) { return (!isPieceFromPlayerOrEmpty(collumn, row, player)); }

bool isPieceFromPlayer(int collumn, int row, int player) { return (chess_board[collumn][row].player == player); }

void setPawnMoves(int player, int piece, int collumn, int row)
{
	if (player == WHITE_PLAYER) {
		if (chess_board[collumn][row - 1].piece == EMPTY) {
			if (row == 6) // Did not move yet
			{
				legal_moves[collumn][row - 1] = true;
				if (chess_board[collumn][row - 2].piece == EMPTY)
					legal_moves[collumn][row - 2] = true;
			} else {
				legal_moves[collumn][row - 1] = true;
			}
		}

		int topLeftCollumn = collumn - 1;
		int topRightCollumn = collumn + 1;
		int topRow = row - 1;
		if (topRow >= 0) {
			if (topLeftCollumn < BOARD_SIZE) {
				if (isPieceFromOpponent(topLeftCollumn, topRow, player)) // top-left of pawn
					legal_moves[topLeftCollumn][topRow] = true;
			}
			if (topRightCollumn >= 0)
			{
				if (isPieceFromOpponent(topRightCollumn, topRow, player)) // top-right of pawn
					legal_moves[topRightCollumn][topRow] = true;
			}
		}
	}

	if (player == BLACK_PLAYER) {
		if (chess_board[collumn][row + 1].piece == EMPTY) {
			if (row == 1) // Did not move yet
			{
				legal_moves[collumn][row + 1] = true;
				if (chess_board[collumn][row + 2].piece == EMPTY)
					legal_moves[collumn][row + 2] = true;
			} else {
				legal_moves[collumn][row + 1] = true;
			}
		}
		int bottomLeftCollumn = collumn - 1;
		int bottomRightCollumn = collumn + 1;
		int bottomRow = row + 1;
		if (bottomRow < BOARD_SIZE) {
			if (bottomLeftCollumn < BOARD_SIZE) {
				if (isPieceFromOpponent(bottomLeftCollumn, bottomRow, player)) // bottom-left of pawn
					legal_moves[bottomLeftCollumn][bottomRow] = true;
			}
			if (bottomRightCollumn >= 0)
			{
				if (isPieceFromOpponent(bottomRightCollumn, bottomRow, player)) // bottom-right of pawn
					legal_moves[bottomRightCollumn][bottomRow] = true;
			}
		}
	}
}

void setRookMoves(int player, int piece, int collumn, int row)
{
	// move up
	for (int i = row - 1; i >= 0; i--) {
		if (isPieceFromOpponent(collumn, i, player)) {
			legal_moves[collumn][i] = true;
			break;
		} else if (!isPieceFromPlayer(collumn, i, player))
			legal_moves[collumn][i] = true;
		else
			break;
	}
	// move down
	for (int i = row + 1; i < BOARD_SIZE; i++) {
		if (isPieceFromOpponent(collumn, i, player)) {
			legal_moves[collumn][i] = true;
			break;
		} else if (!isPieceFromPlayer(collumn, i, player))
			legal_moves[collumn][i] = true;
		else
			break;
	}
	// move left
	for (int i = collumn - 1; i >= 0; i--) {
		if (isPieceFromOpponent(i, row, player)) {
			legal_moves[i][row] = true;
			break;
		} else if (!isPieceFromPlayer(i, row, player))
			legal_moves[i][row] = true;
		else
			break;
	}
	// move right
	for (int i = collumn + 1; i < BOARD_SIZE; i++) {
		if (isPieceFromOpponent(i, row, player)) {
			legal_moves[i][row] = true;
			break;
		} else if (!isPieceFromPlayer(i, row, player))
			legal_moves[i][row] = true;
		else
			break;
	}
}
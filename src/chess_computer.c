#include "chess_computer.h"
#include "board_and_pieces.h"
#include "move_legal_checker.h"
#include <stdlib.h>
#include <string.h>

struct pos {
	int column;
	int row;
};

struct player_piece {
	int column;
	int row;
	int piece;
};

struct board {
	struct pieces t_chess_board[BOARD_SIZE][BOARD_SIZE];
	bool t_cancastle_b_king;
	bool t_cancastle_w_king;
	bool t_cancastle_w_l_rook;
	bool t_cancastle_w_s_rook;
	bool t_cancastle_b_l_rook;
	bool t_cancastle_b_s_rook;
};

struct board *history_of_turns;
int count_turn = 0;

int evaluateBoard(int player);
int evaluatePlayerWorth(int player);
int amountOfPlayerPieces(int player);
void getPlayerPieces(int player, int amount_of_pieces, struct player_piece *possible_pieces);
int amountOfLegalMoves();
void getPossibleMoves(struct pos *possible_legal_moves);

const int PIECE_MULTIPLIER = 8; // higher piece multiplier -> position is less important

const int PAWN_WORTH = 1 * PIECE_MULTIPLIER;
const int KNIGHT_WORTH = 3 * PIECE_MULTIPLIER;
const int BISHOP_WORTH = 3 * PIECE_MULTIPLIER;
const int ROOK_WORTH = 5 * PIECE_MULTIPLIER;
const int QUEEN_WORTH = 9 * PIECE_MULTIPLIER;
const int KING_WORTH = 100 * PIECE_MULTIPLIER; // value highter than other pieces combined

// center position have a higher worth (pawns, bishop, knights)
const int POS_WORTH_CENTERPIECES[BOARD_SIZE][BOARD_SIZE] = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 2, 2, 2, 2, 1, 1},
	{1, 1, 2, 3, 3, 2, 1, 1},
	{1, 1, 2, 3, 3, 2, 0, 0},
	{1, 1, 2, 2, 2, 2, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0}
};

// corner position have a higher worth (king)
const int POS_WORTH_CORNERPIECES[BOARD_SIZE][BOARD_SIZE] = {
	{1, 1, 5, 1, 1, 1, 5, 1},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 5, 1, 1, 1, 5, 1}
};

extern bool cancastle_b_king;
extern bool cancastle_w_king;
extern bool cancastle_w_l_rook;
extern bool cancastle_w_s_rook;
extern bool cancastle_b_l_rook;
extern bool cancastle_b_s_rook;

struct temp_move bestMove(int player, int minmax_depth, struct temp_move previous_move)
{
	// return best moves at last depth
	previous_move.value = evaluateBoard(player);
	if (minmax_depth == 0)
		return previous_move;

	// get possible pieces to select to move
	int amount_of_pieces = amountOfPlayerPieces(player);
	struct player_piece possible_pieces[amount_of_pieces];
	getPlayerPieces(player, amount_of_pieces, possible_pieces);

	// max for black min for white
	struct temp_move best_move;
	if (player == BLACK_PLAYER) {
		best_move.value = -100;
	} else {
		best_move.value = 100;
	}

	// iterate through legal move for the specific selected pieces
	for (int i = 0; i < amount_of_pieces; i++) {
		// get legal moves for pieces
		chess_board[possible_pieces[i].column][possible_pieces[i].row].isSelected = true;
		resetLegalMoves();
		setLegalMoves();
		int amount_of_legal_moves = amountOfLegalMoves();
		if (amount_of_legal_moves == 0) {
			// no moves, skip
			chess_board[possible_pieces[i].column][possible_pieces[i].row].isSelected = false;
			continue;
		}
		struct pos possible_legal_moves[amount_of_legal_moves];
		getPossibleMoves(possible_legal_moves);
		chess_board[possible_pieces[i].column][possible_pieces[i].row].isSelected = false;

		for (int pos = 0; pos < amount_of_legal_moves; pos++) {
			// flip player and select squares
			chess_board[possible_pieces[i].column][possible_pieces[i].row].isSelected = true;
			chess_board[possible_legal_moves[pos].column][possible_legal_moves[pos].row].isMouseHovering = true;
			int next_player;
			(player == WHITE_PLAYER) ? (next_player = BLACK_PLAYER) : (next_player = WHITE_PLAYER);

			// move and evaluate move
			movePiece();
			struct temp_move previous_move = {.selected_column = possible_pieces[i].column,
			  .selected_row = possible_pieces[i].row,
			  .target_column = possible_legal_moves[pos].column,
			  .target_row = possible_legal_moves[pos].row};
			struct temp_move temp = bestMove(next_player, minmax_depth - 1, previous_move);
			unMove();

			// check if move was best
			if (player == BLACK_PLAYER) {
				if (temp.value > best_move.value) {
					best_move = previous_move;
					best_move.value = temp.value;
				}
			} else {
				if (temp.value < best_move.value) {
					best_move = previous_move;
					best_move.value = temp.value;
				}
			}
		}
		resetLegalMoves();
	}
	return (best_move);
}

void saveMove()
{
	count_turn++;
	history_of_turns = realloc(history_of_turns, sizeof(struct board) * (count_turn + 1));
	history_of_turns[count_turn] = (struct board){.t_cancastle_b_king = cancastle_b_king,
	  .t_cancastle_b_l_rook = cancastle_b_l_rook,
	  .t_cancastle_b_s_rook = cancastle_b_s_rook,
	  .t_cancastle_w_king = cancastle_w_king,
	  .t_cancastle_w_l_rook = cancastle_w_l_rook,
	  .t_cancastle_w_s_rook = cancastle_w_s_rook};
	memcpy(history_of_turns[count_turn].t_chess_board, chess_board, sizeof(chess_board));
}

void unMove()
{
	count_turn--;
	if (count_turn < 0) {
		count_turn = 0;
		return;
	}
	memcpy(chess_board, history_of_turns[count_turn].t_chess_board, sizeof(chess_board));
	cancastle_b_king = history_of_turns[count_turn].t_cancastle_b_king;
	cancastle_b_l_rook = history_of_turns[count_turn].t_cancastle_b_l_rook;
	cancastle_b_s_rook = history_of_turns[count_turn].t_cancastle_b_s_rook;
	cancastle_w_king = history_of_turns[count_turn].t_cancastle_w_king;
	cancastle_w_l_rook = history_of_turns[count_turn].t_cancastle_w_l_rook;
	cancastle_w_s_rook = history_of_turns[count_turn].t_cancastle_w_s_rook;
	history_of_turns = realloc(history_of_turns, sizeof(struct board) * (count_turn + 1));
}

void initaliazeMoveHistory()
{
	history_of_turns = malloc(sizeof(struct board));
	history_of_turns[0] = (struct board){.t_cancastle_b_king = cancastle_b_king,
	  .t_cancastle_b_l_rook = cancastle_b_l_rook,
	  .t_cancastle_b_s_rook = cancastle_b_s_rook,
	  .t_cancastle_w_king = cancastle_w_king,
	  .t_cancastle_w_l_rook = cancastle_w_l_rook,
	  .t_cancastle_w_s_rook = cancastle_w_s_rook};
	memcpy(history_of_turns[0].t_chess_board, chess_board, sizeof(chess_board));
}

int evaluateBoard(int player)
{
	int white_worth = evaluatePlayerWorth(WHITE_PLAYER);
	int black_worth = evaluatePlayerWorth(BLACK_PLAYER);
	if (player == WHITE_PLAYER)
		return (white_worth - black_worth);
	if (player == BLACK_PLAYER)
		return (black_worth - white_worth);
}

int evaluatePlayerWorth(int player)
{
	int sum_piece_worth = 0;
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			if (chess_board[column][row].player == player) {
				switch (chess_board[column][row].piece) {
				case PAWN:
					sum_piece_worth += 2 * POS_WORTH_CENTERPIECES[column][row];
					sum_piece_worth += PAWN_WORTH;
					break;
				case KNIGHT:
					sum_piece_worth += POS_WORTH_CENTERPIECES[column][row];
					sum_piece_worth += KNIGHT_WORTH;
					break;
				case BISHOP:
					sum_piece_worth += POS_WORTH_CENTERPIECES[column][row];
					sum_piece_worth += BISHOP_WORTH;
					break;
				case ROOK:
					sum_piece_worth += ROOK_WORTH;
					break;
				case QUEEN:
					sum_piece_worth += QUEEN_WORTH;
					break;
				case KING:
					sum_piece_worth += POS_WORTH_CORNERPIECES[column][row];
					sum_piece_worth += KING_WORTH;
					break;
				}
			}
		}
	}
	return sum_piece_worth;
}

int amountOfPlayerPieces(int player)
{
	int amount_moves = 0;
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			if (chess_board[column][row].player == player) {
				amount_moves++;
			}
		}
	}
	return amount_moves;
}

void getPlayerPieces(int player, int amount_of_pieces, struct player_piece *possible_pieces)
{
	int iterate_piece = 0;
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			if (chess_board[column][row].player == player) {
				possible_pieces[iterate_piece].column = column;
				possible_pieces[iterate_piece].row = row;
				possible_pieces[iterate_piece].piece = chess_board[column][row].piece;
				iterate_piece++;
			}
		}
	}
}

int amountOfLegalMoves()
{
	int sum = 0;
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			sum += legal_moves[column][row]; // true is 1 and false is zero. so false doesnt add to count
		}
	}
	return sum;
}

void getPossibleMoves(struct pos *possible_legal_moves)
{
	int amount_of_legal_moves = 0;
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			if (legal_moves[column][row]) {
				possible_legal_moves[amount_of_legal_moves].column = column;
				possible_legal_moves[amount_of_legal_moves].row = row;
				amount_of_legal_moves++;
			}
		}
	}
}
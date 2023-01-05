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

int evaluate_board(int player);
int evalutate_player_worth(int player);
int evalutate_player_worth(int player);
int amountOfPlayerPieces(int player);
void getPlayerPieces(int player, int amount_of_pieces, struct player_piece *possible_pieces);
int amountOfLegalMoves();
void getPossibleMoves(struct pos *possible_legal_moves);

const int pawn_worth = 1;
const int knight_worth = 3;
const int bishop_worth = 3;
const int rook_worth = 5;
const int queen_worth = 9;
const int king_worth = 100; // value highter than other pieces combined

extern bool cancastle_b_king;
extern bool cancastle_w_king;
extern bool cancastle_w_l_rook;
extern bool cancastle_w_s_rook;
extern bool cancastle_b_l_rook;
extern bool cancastle_b_s_rook;

struct temp_move bestMove(int player, int minmax_depth, struct temp_move previous_move)
{
	// return best moves at last depth
	previous_move.value = evaluate_board(player);
	if (minmax_depth == 0)
		return previous_move;

	// get possible pieces to select to move
	int amount_of_pieces = amountOfPlayerPieces(player);
	struct player_piece *possible_pieces = malloc(sizeof(struct player_piece) * amount_of_pieces);
	getPlayerPieces(player, amount_of_pieces, possible_pieces);

	// iterate through legal move for the specific selected pieces
	struct temp_move best_move = {.value = -100};
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
		struct pos *possible_legal_moves = malloc(sizeof(struct pos) * amount_of_legal_moves);
		getPossibleMoves(possible_legal_moves);
		chess_board[possible_pieces[i].column][possible_pieces[i].row].isSelected = false;

		for (int pos = 0; pos < amount_of_legal_moves; pos++) {
			chess_board[possible_pieces[i].column][possible_pieces[i].row].isSelected = true;
			chess_board[possible_legal_moves[pos].column][possible_legal_moves[pos].row].isMouseHovering = true;
			int next_player;
			(player == WHITE_PLAYER) ? (next_player = BLACK_PLAYER) : (next_player = WHITE_PLAYER);
			movePiece();
			struct temp_move previous_move = {.selected_column = possible_pieces[i].column,
			  .selected_row = possible_pieces[i].row,
			  .target_column = possible_legal_moves[pos].column,
			  .target_row = possible_legal_moves[pos].row};
			struct temp_move temp = bestMove(player, minmax_depth - 1, previous_move);
			unMove();
			if (temp.value > best_move.value)
			{
				best_move = previous_move;
				best_move.value = temp.value;
			}
		}
		resetLegalMoves();
		free(possible_legal_moves);
	}
	free(possible_pieces);
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

int evaluate_board(int player)
{
	int white_worth = evalutate_player_worth(WHITE_PLAYER);
	int black_worth = evalutate_player_worth(BLACK_PLAYER);
	if (player == WHITE_PLAYER)
		return (white_worth - black_worth);
	if (player == BLACK_PLAYER)
		return (black_worth - white_worth);
}

int evalutate_player_worth(int player)
{
	int sum_piece_worth = 0;
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			if (chess_board[column][row].player == player) {
				switch (chess_board[column][row].piece) {
				case PAWN:
					sum_piece_worth += pawn_worth;
					break;
				case KNIGHT:
					sum_piece_worth += knight_worth;
					break;
				case BISHOP:
					sum_piece_worth += bishop_worth;
					break;
				case ROOK:
					sum_piece_worth += rook_worth;
					break;
				case QUEEN:
					sum_piece_worth += queen_worth;
					break;
				case KING:
					sum_piece_worth += king_worth;
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
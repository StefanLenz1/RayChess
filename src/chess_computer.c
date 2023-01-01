#include "chess_computer.h"
#include "board_and_pieces.h"
#include "main.h"
#include "move_legal_checker.h"
#include <stdlib.h>
#include <string.h>

struct temp_move {
	int column;
	int row;
	int value;
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
int amountOfMoves(int player);
void getPlayerPieces(int player, int amount_of_pieces, struct player_piece *possible_pieces);

const int pawn_worth = 1;
const int knight_worth = 3;
const int bishop_worth = 3;
const int rook_worth = 5;
const int queen_worth = 9;
const int king_worth = 30; // value highter than other pieces combined

extern bool cancastle_b_king;
extern bool cancastle_w_king;
extern bool cancastle_w_l_rook;
extern bool cancastle_w_s_rook;
extern bool cancastle_b_l_rook;
extern bool cancastle_b_s_rook;

void bestMove(int player, int minmax_depth)
{
	int amount_of_pieces = amountOfMoves(player);
	struct player_piece *possible_pieces = malloc(sizeof(struct player_piece) * amount_of_pieces);
	getPlayerPieces(player, amount_of_pieces, possible_pieces);
	int value = evaluate_board(player);

	resetLegalMoves();
	chess_board[possible_pieces[1].column][possible_pieces[1].row].isSelected = true;
	setLegalMoves();
	for (int column = 0; column < BOARD_SIZE; column++) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			if (legal_moves[column][row]) {
				chess_board[column][row].isMouseHovering = true;
				movePiece();
				// unMove();
				resetBoardIsHovering();
				resetBoardIsSelected();
				resetLegalMoves();
				return;
			}
		}
	}
	free(possible_pieces);
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
	history_of_turns = realloc(history_of_turns, sizeof(struct board) * (count_turn));
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

int amountOfMoves(int player)
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
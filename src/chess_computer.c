#include "board_and_pieces.h"

const int pawn_worth = 1;
const int knight_worth = 3;
const int bishop_worth = 3;
const int rook_worth = 5;
const int queen_worth = 9;
const int king_worth = 30;  // value highter than other pieces combined

void bestMove()
{
        ;
}

int evaluate_board()
{
        ;
}

int evalutate_player_worth(player)
{
        int sum_piece_worth = 0;
        for (int column = 0; column < BOARD_SIZE; column++)
        {
                for (int row = 0; row < BOARD_SIZE; row++)
                {
                        if(chess_board[column][row].player == player)
                        {
                                switch(chess_board[column][row].piece)
                                {
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
}
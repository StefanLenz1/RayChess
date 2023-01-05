#ifndef CHESS_COMPUTER
#define CHESS_COMPUTER

#include "main.h"
struct temp_move bestMove(int player, int minmax_depth, struct temp_move previous_move);
void saveMove();
void initaliazeMoveHistory();
void unMove();

#endif
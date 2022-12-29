#include <raylib.h>
#define BOARD_SIZE 8
extern bool legal_moves[8][8];

void setPawnMoves(int player, int piece, int collumn, int row)
{
        if (player == 1) // White
        {
                if (row == 1) // Did not move yet
                {
                        legal_moves[collumn + 1][row] = true;
                        legal_moves[collumn + 2][row] = true;
                }
        }
}
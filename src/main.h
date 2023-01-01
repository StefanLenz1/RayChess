#include "raylib.h"
#include "board_and_pieces.h"

void initializeChessBoard();
Rectangle getSourceSprite(struct pieces pieces);
void drawFrame();
void drawBoard();
void updateFrame(Vector2* mouse_position, bool* piece_is_selected, bool* opponent_turn, bool* game_is_over);
void getLegalMoves(struct pieces pieces, int column, int row);
void drawLegalMoves();
void setLegalMoves();
void movePiece();
void resetBoardIsSelected();
void resetBoardIsHovering();
void resetLegalMoves();
void castle(int piece_column, int piece_row, int moveTo_column, int moveTo_row, int piece_player);
void opponentMove();
int checkWinner();
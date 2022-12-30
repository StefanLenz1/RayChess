struct pieces {
	int piece;
	int player; // 0 is empty, 1 is player one, 2 is player two
	bool isSelected;
	bool isMouseHovering;
};

enum { BOARD_SIZE = 8 }; // cross file compile-time integer constant

extern bool legal_moves[BOARD_SIZE][BOARD_SIZE];
extern struct pieces chess_board[BOARD_SIZE][BOARD_SIZE];

/* 
chess board array
  0 1 2 3 4 5 6 7
0 O O O O O O O O
1 O O O O O O O O
2 O O O O O O O O
3 O O O O O O O O
4 O O O O O O O O
5 O O O O O O O O
6 O O O O O O O O
7 O O O O O O O O
*/

enum {
        NO_PLAYER = 0,
        WHITE_PLAYER = 1,
        BLACK_PLAYER = 2
};

enum {
	EMPTY,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING,
	PAWN,
	ROOK
};
#ifndef TYPES_H
#define TYPES_H

#define XLEN            18
#define YLEN            21
#define BOARDXLEN       10
#define BOARDYLEN       20
#define DEFAULTSCALE    40
#define PIECE_NUM   7

typedef enum{PIECE_I, PIECE_J, PIECE_L, PIECE_O, PIECE_S, PIECE_T, PIECE_Z, PIECE_EMPTY}PieceType;

typedef struct{
    PieceType type;
    int rotation;
    u16 minos;
}Piece;

typedef struct{
    PieceType *board;
    int linesCleared;
    Coord piecePos;
    Piece piece;
    PieceType nextPieceType;
    PieceType holdPiece;
    bool holdAvailable;
    uint lastMoveDown;
    uint fallTime;
}GameState;

#endif
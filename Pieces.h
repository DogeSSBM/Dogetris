#ifndef PIECES_H
#define PIECES_H

#define PIECE_NUM   7

typedef enum{PIECE_I, PIECE_J, PIECE_L, PIECE_O, PIECE_S, PIECE_T, PIECE_Z, PIECE_EMPTY}PieceType;

Color pieceColor1[PIECE_NUM] = {
    {.r=  0,.g=240,.b=240,.a=255},
    {.r=  0,.g=  0,.b=240,.a=255},
    {.r=240,.g=161,.b=  0,.a=255},
    {.r=240,.g=240,.b=  0,.a=255},
    {.r=  0,.g=240,.b=  0,.a=255},
    {.r=162,.g=  0,.b=240,.a=255},
    {.r=241,.g=  0,.b=  0,.a=255}
};

Color pieceColor2[PIECE_NUM] = {
    {.r=  0,.g=121,.b=121,.a=255},
    {.r=  0,.g=  0,.b=121,.a=255},
    {.r=121,.g= 79,.b=  0,.a=255},
    {.r=121,.g=121,.b=  0,.a=255},
    {.r=  0,.g=121,.b=  0,.a=255},
    {.r= 79,.g=  0,.b=121,.a=255},
    {.r=121,.g=  0,.b=  0,.a=255}
};

typedef struct{
    PieceType type;
    int rotation;
    u16 minos;
}Piece;

bool minoPos(const u16 minos, const int x, const int y)
{
    assertExpr(x >= 0 && x < 4 && y >= 0 && y < 4);
    return (1 << 15) & (minos << (y*4 + x));
}

bool minoCoord(const u16 minos, const Coord pos)
{
    assertExpr(pos.x >= 0 && pos.x < 4 && pos.y >= 0 && pos.y < 4);
    return (1 << 15) & (minos << (pos.y*4 + pos.x));
}

Piece getPiece(const PieceType type, const int rotation)
{
    assertExpr(rotation >= 0 && rotation < 4);
    assertExpr(type >= PIECE_I && type <= PIECE_Z);
    switch(type){
        case PIECE_I:
            return (Piece){.type = type, .rotation = rotation, .minos = rots_I[rotation]}; break;
        case PIECE_J:
            return (Piece){.type = type, .rotation = rotation, .minos = rots_J[rotation]}; break;
        case PIECE_L:
            return (Piece){.type = type, .rotation = rotation, .minos = rots_L[rotation]}; break;
        case PIECE_O:
            return (Piece){.type = type, .rotation = rotation, .minos = rots_O[rotation]}; break;
        case PIECE_S:
            return (Piece){.type = type, .rotation = rotation, .minos = rots_S[rotation]}; break;
        case PIECE_T:
            return (Piece){.type = type, .rotation = rotation, .minos = rots_T[rotation]}; break;
        case PIECE_Z:
            return (Piece){.type = type, .rotation = rotation, .minos = rots_Z[rotation]}; break;
        default:
            break;
    }
    panic("unreachable");
    return (Piece){0};
}

#endif
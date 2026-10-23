#ifndef PIECES_H
#define PIECES_H

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

// returns true if the mino at pos x y is filled
bool minoPos(const u16 minos, const int x, const int y)
{
    assertExpr(x >= 0 && x < 4 && y >= 0 && y < 4);
    return (1 << 15) & (minos << (y*4 + x));
}

// returns true if the mino at pos is filled
bool minoCoord(const u16 minos, const Coord pos)
{
    assertExpr(pos.x >= 0 && pos.x < 4 && pos.y >= 0 && pos.y < 4);
    return (1 << 15) & (minos << (pos.y*4 + pos.x));
}

// gets piece of corrosponding type and rotation
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

// returns true if all minos are visible on board
bool pieceVisible(const u16 minos, const Coord pos)
{
    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(minoPos(minos, x, y) && pos.y+y >= 0)
                return true;
        }
    }
    return false;
}

// sets pos to the first valid rotation offset and returns its index. if none are valid returns -1
int rotateOff(PieceType *board, const u16 minos, Coord *pos, const Offset off[5])
{
    for(int i = 0; i < 5; i++){
        const Coord kickPos = coordAdd(*pos, off[i]);
        if(minosFit(board, minos, kickPos)){
            *pos = kickPos;
            return i;
        }
    }
    return -1;
}

// attempts to rotate piece. modifies piece and pos and returns true on success. otherwise returns false
bool rotate(PieceType *board, Piece *piece, Coord *pos, const int rotation)
{
    assertExpr(piece && pos && (rotation == -1 || rotation == 1));
    const int nextRotation = wrap(piece->rotation + rotation, 0, 4);
    if(piece->type == PIECE_O){
        piece->rotation = nextRotation;
        return true;
    }
    const Piece nextPiece = getPiece(piece->type, nextRotation);
    int kickIndex = -1;
    if(piece->type == PIECE_I){
        if      (piece->rotation == 0 && rotation == -1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x=-1,.y= 0},{.x= 2,.y= 0},{.x=-1,.y= 2},{.x= 2,.y=-1}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 0 && rotation ==  1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x=-2,.y= 0},{.x= 1,.y= 0},{.x=-2,.y=-1},{.x= 1,.y= 2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 1 && rotation == -1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x= 2,.y= 0},{.x=-1,.y= 0},{.x= 2,.y= 1},{.x=-1,.y=-2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 1 && rotation ==  1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x=-1,.y= 0},{.x= 2,.y= 0},{.x=-1,.y= 2},{.x= 2,.y=-1}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 2 && rotation == -1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x= 1,.y= 0},{.x=-2,.y= 0},{.x= 1,.y=-2},{.x=-2,.y= 1}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 2 && rotation ==  1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x= 2,.y= 0},{.x=-1,.y= 0},{.x= 2,.y= 1},{.x=-1,.y=-2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 3 && rotation == -1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x=-2,.y= 0},{.x= 1,.y= 0},{.x=-2,.y=-1},{.x= 1,.y= 2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 3 && rotation ==  1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x= 1,.y= 0},{.x=-2,.y= 0},{.x= 1,.y=-2},{.x=-2,.y= 1}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }
    }else{
        if      (piece->rotation == 0 && rotation == -1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x= 1,.y= 0},{.x= 1,.y= 1},{.x= 0,.y=-2},{.x= 1,.y=-2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 0 && rotation ==  1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x=-1,.y= 0},{.x=-1,.y= 1},{.x= 0,.y=-2},{.x=-1,.y=-2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 1 && rotation == -1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x= 1,.y= 0},{.x= 1,.y=-1},{.x= 0,.y= 2},{.x= 1,.y= 2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 1 && rotation ==  1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x= 1,.y= 0},{.x= 1,.y=-1},{.x= 0,.y= 2},{.x= 1,.y= 2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 2 && rotation == -1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x=-1,.y= 0},{.x=-1,.y= 1},{.x= 0,.y=-2},{.x=-1,.y=-2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 2 && rotation ==  1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x= 1,.y= 0},{.x= 1,.y= 1},{.x= 0,.y=-2},{.x= 1,.y=-2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 3 && rotation == -1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x=-1,.y= 0},{.x=-1,.y=-1},{.x= 0,.y= 2},{.x= 1,.y= 2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }else if(piece->rotation == 3 && rotation ==  1){
            const Offset off[5] = {{.x= 0,.y= 0},{.x=-1,.y= 0},{.x=-1,.y=-1},{.x= 0,.y= 2},{.x=-1,.y= 2}};
            kickIndex = rotateOff(board, nextPiece.minos, pos, off);
        }
    }
    if(kickIndex >= 0){
        *piece = nextPiece;
        return true;
    }

    return false;
}

#endif
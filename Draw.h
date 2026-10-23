#ifndef DRAW_H
#define DRAW_H

// draws board grid lines
void drawBoardLines(const Offset off, const int scale)
{
    setColor(DARKGREY);
    for(int y = 0; y < BOARDYLEN; y++){
        for(int x = 0; x < BOARDXLEN; x++){
            fillBorderCoordSquare(coordAdd(coordMuli(iC(x+1,y), scale), off), scale, -imax(scale/24, 1));
        }
    }
}

// draws mino at board x y pos
void drawMinoPos(const int x, const int y, const Offset off, const int scale, const Color color1, const Color color2)
{
    const Coord boardPos = bposwCoord(x, y, off, scale);
    setColor(color2);
    fillSquareCoord(boardPos, scale);
    setColor(color1);
    fillSquareCoordResize(boardPos, scale, -imax(scale/24, 1));
}

// draws grey squares surrounding board
void drawBorders(const Offset off, const int scale)
{
    for(int y = 0; y < YLEN-1; y++){
        drawMinoPos( 0,  y, off, scale, DARKGREY, GREY);
        drawMinoPos(11,  y, off, scale, DARKGREY, GREY);
        drawMinoPos(17,  y, off, scale, DARKGREY, GREY);
    }
    for(int x = 0; x < XLEN; x++){
        drawMinoPos( x, 20, off, scale, DARKGREY, GREY);
    }
}

// draws piece at board pos
void drawPiece(const Piece piece, const Coord pos, const Offset off, const int scale)
{
    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(pos.y + y >= 0 && minoPos(piece.minos, x, y))
                drawMinoPos(pos.x+x+1, pos.y+y, off, scale, pieceColor1[piece.type], pieceColor2[piece.type]);
        }
    }
}

// draws next piece
void drawNext(const PieceType piece, const Offset off, const int scale)
{
    setTextSize(scale);
    drawTextCoord("Next", bposwCoord(12, 0, off, scale));
    drawPiece(getPiece(piece, 0), (const Coord){.x=12, .y=1}, off, scale);
}

// draws held piece
void drawHoldPiece(const PieceType piece, const Offset off, const int scale)
{
    setTextSize(scale);
    drawTextCoord("Hold", bposwCoord(12, 4, off, scale));
    if(piece != PIECE_EMPTY)
        drawPiece(getPiece(piece, 0), (const Coord){.x=12, .y=5}, off, scale);
}

// draws lines cleared
void drawLinesCleared(const int linesCleared, const Offset off, const int scale)
{
    setTextSize(scale);
    drawTextCoord("Lines", bposwCoord(12, 17, off, scale));
    drawTextCoord("Cleared", bposwCoord(12, 18, off, scale));
    char buf[32] = {0};
    sprintf(buf, "%i", linesCleared);
    drawTextCoord(buf, bposwCoord(12, 19, off, scale));
}

// draws all minos on board
void drawBoard(PieceType *board, const Offset off, const int scale)
{
    for(int y = 0; y < 20; y++){
        for(int x = 0; x < 10; x++){
            const PieceType pt = board[y*20+x];
            if(pt != PIECE_EMPTY)
                drawMinoPos(x+1, y, off, scale, pieceColor1[pt], pieceColor2[pt]);
        }
    }
}

// draws everything
void drawAll(GameState *state, const Offset off, const int scale)
{
    drawBorders(off, scale);
    drawBoardLines(off, scale);
    drawPiece(state->piece, state->piecePos, off, scale);
    drawBoard(state->board, off, scale);
    drawNext(state->nextPieceType, off, scale);
    drawHoldPiece(state->holdPiece, off, scale);
    drawLinesCleared(state->linesCleared, off, scale);
}

#endif
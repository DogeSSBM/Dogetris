#ifndef DRAW_H
#define DRAW_H

void drawBoardLines(const Offset off, const int scale)
{
    setColor(DARKGREY);
    for(int y = 0; y < BOARDYLEN; y++){
        for(int x = 0; x < BOARDXLEN; x++){
            fillBorderCoordSquare(coordAdd(coordMuli(iC(x+1,y), scale), off), scale, -imax(scale/24, 1));
        }
    }
}

void drawMinoPos(const int x, const int y, const Offset off, const int scale, const Color color1, const Color color2)
{
    const Coord boardPos = atPos(x, y, off, scale);
    setColor(color2);
    fillSquareCoord(boardPos, scale);
    setColor(color1);
    fillSquareCoordResize(boardPos, scale, -imax(scale/24, 1));
}

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

void drawPiece(const Piece piece, const Coord pos, const Offset off, const int scale)
{
    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(pos.y + y >= 0 && minoPos(piece.minos, x, y))
                drawMinoPos(pos.x+x+1, pos.y+y, off, scale, pieceColor1[piece.type], pieceColor2[piece.type]);
        }
    }
}

void drawNext(const PieceType piece, const Offset off, const int scale)
{
    setTextSize(scale);
    drawTextCoord("Next", atPos(12, 0, off, scale));
    drawPiece(getPiece(piece, 0), (const Coord){.x=12, .y=1}, off, scale);
}

void drawHoldPiece(const PieceType piece, const Offset off, const int scale)
{
    setTextSize(scale);
    drawTextCoord("Hold", atPos(12, 4, off, scale));
    if(piece != PIECE_EMPTY)
        drawPiece(getPiece(piece, 0), (const Coord){.x=12, .y=5}, off, scale);
}

void drawLinesCleared(const int linesCleared, const Offset off, const int scale)
{
    setTextSize(scale);
    drawTextCoord("Lines", atPos(12, 17, off, scale));
    drawTextCoord("Cleared", atPos(12, 18, off, scale));
    char buf[32] = {0};
    sprintf(buf, "%i", linesCleared);
    drawTextCoord(buf, atPos(12, 19, off, scale));
}

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

#endif
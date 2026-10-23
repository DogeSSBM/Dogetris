#ifndef BOARD_H
#define BOARD_H

PieceType globalBoard[10*20];

// prints state of board to stdout
void printBoard(PieceType *board)
{
    printf("board:\n");
    for(int y = 0; y < 20; y++){
        printf("|");
        for(int x = 0; x < 10; x++){
            printf("%c%c", board[y*20 + x] == PIECE_EMPTY ? ' ' : '#', x < 9 ? ' ' : '|');
        }
        printf("\n");
    }
    for(int x = 0; x < 10; x++)
        printf("+-");
    printf("+\n");
}

// places all valid minos of a piece on the board
void boardPlacePiece(PieceType *board, const Piece piece, const Coord pos)
{
    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(pos.x+x < 10 && pos.y + y < 20 && minoPos(piece.minos, x, y))
                board[(pos.y+y)*20 + pos.x+x] = piece.type;
        }
    }
}

// returns true if all minos of a piece are in bounds and fit on the board at pos. doesnt check minos above visible board
bool minosFit(PieceType *board, const u16 minos, const Coord pos)
{
    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(minoPos(minos, x, y)){
                if(pos.x+x >= 10 || pos.y + y >= 20 || pos.x+x < 0)
                    return false;
                if(pos.y+y < 0)
                    continue;
                if(board[(pos.y+y)*20+(pos.x+x)] != PIECE_EMPTY)
                    return false;
            }
        }
    }
    return true;
}

// shifts all lines above stopLine down one. fills top row with PIECE_EMPTY
void shiftLinesDown(PieceType *board, const int stopLine)
{
    for(int y = stopLine; y > 0; y--){
        for(int x = 0; x < 10; x++){
            board[y*20 + x] = board[(y-1)*20 + x];
        }
    }
    for(int x = 0; x < 10; x++){
        board[x] = PIECE_EMPTY;
    }
}

// returns true if line at y is full of minos
bool lineFull(PieceType *board, const int y)
{
    for(int x = 0; x < 10; x++){
        if(board[x+y*20] == PIECE_EMPTY)
            return false;
    }
    return true;
}

// clears all full lines on board. returns number of lines cleared
int clearLines(PieceType *board)
{
    int lines = 0;
    for(size_t y = 0; y < 20; y++){
        if(lineFull(board, y)){
            lines++;
            shiftLinesDown(board, y);
        }
    }
    return lines;
}

// returns true if all minos of piece fit when moved down one
bool canMoveDown(PieceType *board, const u16 minos, Coord pos)
{
    return minosFit(board, minos, iC(pos.x, pos.y+1));
}

// returns lowest possible pos where piece will fit. returns pos if minos already dont fit
Coord sonicDrop(PieceType *board, const u16 minos, Coord pos)
{
    if(!minosFit(board, minos, pos))
        return pos;
    while(canMoveDown(board, minos, pos))
        pos.y++;
    return pos;
}

// returns true if piece should move down for a soft drop
bool shouldSoftDrop(const uint frameTime, const uint lastMoveDown)
{
    const uint downHeldTime = imax(keyHeldTime(SC_S), keyHeldTime(SC_DOWN));
    return downHeldTime > 500 && (frameTime - lastMoveDown > 50);
}

// returns true if piece should be placed based on when it was last moved down, down key state, soft drop state, and if piece can move down
bool shouldPlacePiece(PieceType *board, const u16 minos, const Coord pos, const uint frameTime, const uint lastMoveDown)
{
    if(!canMoveDown(board, minos, pos)){
        if(dirKeyPressedOffset().y > 0 || shouldSoftDrop(frameTime, lastMoveDown) || frameTime - lastMoveDown > 1000)
            return true;
    }
    return false;
}

// gets piece movement based on direction key presses / held time
Offset movement(const uint frameTime, const uint lastMoveDown, const uint lastMoveLR, const uint fallTime)
{
    Offset move = {
        .x = (keyPressed(SC_RIGHT) || keyPressed(SC_D)) - (keyPressed(SC_LEFT) || keyPressed(SC_A)),
        .y = keyPressed(SC_DOWN) || keyPressed(SC_S) || shouldSoftDrop(frameTime, lastMoveDown) || (frameTime - lastMoveDown > fallTime)
    };
    const int lrHeld = (keyHeld(SC_RIGHT) || keyHeld(SC_D)) - (keyHeld(SC_LEFT) || keyHeld(SC_A));
    if(!move.x && lrHeld){
        const int lrHoldTime = lrHeld > 0 ? imax(keyHeldTime(SC_RIGHT), keyHeldTime(SC_D)) : imax(keyHeldTime(SC_LEFT), keyHeldTime(SC_A));
        if(iabs(lrHoldTime) > 500 && (frameTime - lastMoveLR > 50))
            move.x = lrHeld;
    }
    return move;
}

#endif
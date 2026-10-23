#ifndef BOARD_H
#define BOARD_H

PieceType globalBoard[10*20];

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

void boardPlacePiece(PieceType *board, const Piece piece, const Coord pos)
{
    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(pos.x+x < 10 && pos.y + y < 20 && minoPos(piece.minos, x, y))
                board[(pos.y+y)*20 + pos.x+x] = piece.type;
        }
    }
}

bool minosFit(PieceType *board, const u16 minos, const Coord pos)
{
    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(pos.y+y < 0)
                continue;
            if(minoPos(minos, x, y)){
                if(pos.x+x >= 10 || pos.y + y >= 20 || pos.x+x < 0)
                    return false;
                if(board[(pos.y+y)*20+(pos.x+x)] != PIECE_EMPTY)
                    return false;
            }
        }
    }
    return true;
}

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

bool lineFull(PieceType *board)
{
    for(int x = 0; x < 10; x++){
        if(board[x] == PIECE_EMPTY)
            return false;
    }
    return true;
}

int clearLines(PieceType *board)
{
    int lines = 0;
    for(size_t y = 0; y < 20; y++){
        if(lineFull(board+y*20)){
            lines++;
            shiftLinesDown(board, y);
        }
    }
    return lines;
}

bool canMoveDown(PieceType *board, const u16 minos, Coord pos)
{
    return minosFit(board, minos, iC(pos.x, pos.y+1));
}

Coord softDrop(PieceType *board, const u16 minos, Coord pos)
{
    while(canMoveDown(board, minos, pos))
        pos.y++;
    return pos;
}

bool shouldPlacePiece(PieceType *board, const u16 minos, const Coord pos, const uint frameTime, const uint lastMoveDown)
{
    if(!canMoveDown(board, minos, pos)){
        if(dirKeyPressedOffset().y > 0 || frameTime - lastMoveDown > 1000)
            return true;
    }
    return false;
}

#endif
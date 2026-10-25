#ifndef STATE_H
#define STATE_H

// resets state to starting game state
void resetState(GameState *state)
{
    for(int y = 0; y < 20; y++){
        for(int x = 0; x < 10; x++){
            state->board[y*20+x] = PIECE_EMPTY;
        }
    }
    state->linesCleared = 0;
    state->piecePos = iC(3, -2);
    state->piece = getPiece(rand()%PIECE_NUM, 0);
    state->nextPieceType = rand()%PIECE_NUM;
    state->holdPiece = PIECE_EMPTY;
    state->holdAvailable = true;
    state->lastMoveDown = getTicks();
    state->lastMoveLR = state->lastMoveDown;
    state->fallTime = 1000;
    state->score = 0;
}

// places piece on board and adds score. gets next piece. returns false on lose
bool placePiece(GameState *state, const uint frameTime)
{
    if(!pieceVisible(state->piece.minos, state->piecePos))
        return false;
    boardPlacePiece(state->board, state->piece, state->piecePos);
    state->piecePos = (Coord){.x = 3, .y = -2};
    state->piece = getPiece(state->nextPieceType, 0);
    state->nextPieceType = rand()%PIECE_NUM;
    state->lastMoveDown = frameTime;
    const int lines = clearLines(state->board);
    if(lines){
        int scoreAdded = 0;
        switch(lines){
            case 1: scoreAdded = 100; break;
            case 2: scoreAdded = 300; break;
            case 3: scoreAdded = 500; break;
            case 4: scoreAdded = 800; break;
            default: panic("unreachable"); break;
        }
        printf("%i lines cleared. +%i score\n", lines, scoreAdded);
        state->score += scoreAdded;
    }
    state->linesCleared += lines;
    state->holdAvailable = true;
    return true;
}

// holds piece if available. returns true if successful
bool holdPiece(GameState* state, const uint frameTime)
{
    if(!state->holdAvailable)
        return false;
    const PieceType temp = state->holdPiece;
    state->holdPiece = state->piece.type;
    if(temp != PIECE_EMPTY){
        state->piece = getPiece(temp, 0);
    }else{
        state->piece = getPiece(state->nextPieceType, 0);
        state->nextPieceType = rand()%PIECE_NUM;
    }
    state->piecePos = (Coord){.x = 3, .y = -2};
    state->lastMoveDown = frameTime;
    state->holdAvailable = false;
    return true;
}

#endif
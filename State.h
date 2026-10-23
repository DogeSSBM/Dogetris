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
    state->fallTime = 1000;
    state->score = 0;
}

#endif
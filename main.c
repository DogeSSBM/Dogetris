#include "DogeLib/Includes.h"
#include "Types.h"
#include "PieceRotDefs.h"
#include "Decls.h"
#include "WindowStuff.h"
#include "Pieces.h"
#include "Board.h"
#include "State.h"
#include "Draw.h"

void loose(GameState *state, const Offset lastOff, int lastScale, const uint lastFrameTime)
{
    drawAll(state, lastOff, lastScale);
    frameEnd(lastFrameTime);
    int index = 0;
    Color color = RED;

    while(keyHeldTime(SC_SPACE) < 1500){
        const uint frameTime = frameStart();

        if(keyPressed(SC_ESCAPE))
            exit(0);

        const Length windowLen = getWindowLen();
        const int scale = getScale(windowLen);
        const Offset off = getOffset(windowLen, scale);

        if(keyPressed(SC_P))
            printBoard(state->board);

        drawAll(state, off, scale);
        drawHoldToX("Game Over!", "Hold space to start a new game.", color, keyHeldTime(SC_SPACE), windowLen, scale);
        rainbowNextColor(&color, &index);

        frameEnd(frameTime);
    }
    resetState(state);
}

void pause(GameState *state, const Offset lastOff, int lastScale, const uint lastFrameTime)
{
    drawAll(state, lastOff, lastScale);
    frameEnd(lastFrameTime);
    int index = 0;
    Color color = RED;
    uint frameTime = lastFrameTime;

    while(keyHeldTime(SC_RETURN) < 1500){
        frameTime = frameStart();

        if(keyPressed(SC_ESCAPE))
            exit(0);

        const Length windowLen = getWindowLen();
        const int scale = getScale(windowLen);
        const Offset off = getOffset(windowLen, scale);

        if(keyPressed(SC_P))
            printBoard(state->board);

        drawAll(state, off, scale);
        drawHoldToX("Paused!", "Hold Enter to resume.", color, keyHeldTime(SC_RETURN), windowLen, scale);
        rainbowNextColor(&color, &index);

        frameEnd(frameTime);
    }
    state->lastMoveDown += frameTime - lastFrameTime;
}

int main(void)
{
    init();
    windowInit();
    setTextColor(GREY);
    GameState state = {.board = globalBoard};
    resetState(&state);

    while(1){
        const uint frameTime = frameStart();

        if(keyPressed(SC_ESCAPE))
            return 0;

        const Length windowLen = getWindowLen();
        const int scale = getScale(windowLen);
        const Offset off = getOffset(windowLen, scale);

        if(keyPressed(SC_RETURN)){
            pause(&state, off, scale, frameTime);
            continue;
        }

        if(keyPressed(SC_RSHIFT))
            state.editMode = !state.editMode;
        
        if(mouseBtnPressed(MOUSE_L) && state.editMode)
            edit(state.board, state.piece.minos, state.piecePos, off, scale);

        if(keyPressed(SC_SPACE)){
            state.piecePos = sonicDrop(state.board, state.piece.minos, state.piecePos);
            state.lastMoveDown = frameTime;
        }

        const int rotation = (keyPressed(SC_E) || keyPressed(SC_X)) - (keyPressed(SC_Q) || keyPressed(SC_Z));
        if(rotation)
            rotate(state.board, &state.piece, &state.piecePos, rotation);

        if(keyPressed(SC_LSHIFT))
            holdPiece(&state, frameTime);

        if(shouldPlacePiece(state.board, state.piece.minos, state.piecePos, frameTime, state.lastMoveDown)){
            if(!placePiece(&state, frameTime)){
                loose(&state, off, scale, frameTime);
                continue;
            }
        }else{
            const Offset move = getMovement(state.editMode, frameTime, state.lastMoveDown, state.lastMoveLR, state.fallTime);
            movePiece(&state, move, frameTime);
        }

        if(keyPressed(SC_P))
            printBoard(state.board);

        drawAll(&state, off, scale);

        frameEnd(frameTime);
    }
    return 0;
}

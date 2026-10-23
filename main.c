#include "DogeLib/Includes.h"
#include "Types.h"
#include "PieceRotDefs.h"
#include "Decls.h"
#include "WindowStuff.h"
#include "Pieces.h"
#include "Board.h"
#include "State.h"
#include "Draw.h"

void drawHoldToX(char *xStr, char *contStr, const Color xColor, const int heldTime, const Length windowLen, const int scale)
{
    setTextColor(xColor);
    setTextSize(scale*2);
    const Length overLen = textLength(xStr);
    const Coord overPos = iC(windowLen.x/2, windowLen.y/4);
    setColor(BLACK);
    fillRectCenteredCoordLength(overPos, overLen);
    setColor(DARKERGREY);
    fillBorderCenteredCoord(overPos, overLen, imax(scale/12, 4));
    drawTextCenteredCoord(xStr, overPos);

    setTextSize((scale*2)/3);
    setTextColor(LIGHTGREY);
    const Length contLen = textLength(contStr);
    const Coord contPos = iC(windowLen.x/2, windowLen.y/2);
    setColor(BLACK);
    fillRectCenteredCoordLength(contPos, contLen);
    setColor(DARKERGREY);
    fillBorderCenteredCoord(contPos, contLen, imax(scale/12, 4));
    drawTextCenteredCoord(contStr, contPos);

    if(heldTime){
        const Coord barPos = iC(windowLen.x/2, (windowLen.y/4)*3);
        setColor(BLACK);
        fillRectCenteredCoordLength(barPos, contLen);
        setColor(xColor);
        fillRectCoordLength(coordUncenter(barPos, contLen), iC(contLen.x*(heldTime/1500.0f), contLen.y));
        setColor(DARKERGREY);
        fillBorderCenteredCoord(barPos, contLen, imax(scale/12, 4));
    }
}

void rainbowNextColor(Color *color, int *index)
{
    u8 *rgb[3] = {&color->r, &color->g, &color->b};
    if(*rgb[*index] == 0){
        *index = wrap(*index+1, 0, 3);
    }else{
        *rgb[*index] = imax((int)*rgb[*index] - 4, 0);
        *rgb[wrap(*index+1, 0, 3)] = imin((int)*rgb[wrap(*index+1, 0, 3)] + 4, 255);
    }
}

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

        if(keyPressed(SC_SPACE)){
            state.piecePos = softDrop(state.board, state.piece.minos, state.piecePos);
            state.lastMoveDown = frameTime;
        }

        const int rotation = (keyPressed(SC_E) || keyPressed(SC_X)) - (keyPressed(SC_Q) || keyPressed(SC_Z));
        if(rotation)
            rotate(state.board, &state.piece, &state.piecePos, rotation);

        if(keyPressed(SC_LSHIFT) && state.holdAvailable){
            const PieceType temp = state.holdPiece;
            state.holdPiece = state.piece.type;
            if(temp != PIECE_EMPTY){
                state.piece = getPiece(temp, 0);
            }else{
                state.piece = getPiece(state.nextPieceType, 0);
                state.nextPieceType = rand()%PIECE_NUM;
            }
            state.piecePos = (Coord){.x = 3, .y = -2};
            state.lastMoveDown = frameTime;
            state.holdAvailable = false;
        }

        if(shouldPlacePiece(state.board, state.piece.minos, state.piecePos, frameTime, state.lastMoveDown)){
            if(!pieceVisible(state.piece.minos, state.piecePos)){
                loose(&state, off, scale, frameTime);
                continue;
            }
            boardPlacePiece(state.board, state.piece, state.piecePos);
            state.piecePos = (Coord){.x = 3, .y = -2};
            state.piece = getPiece(state.nextPieceType, 0);
            state.nextPieceType = rand()%PIECE_NUM;
            state.lastMoveDown = frameTime;
            const int lines = clearLines(state.board);
            if(lines)
                printf("%i lines cleared\n", lines);
            state.linesCleared += lines;
            state.holdAvailable = true;
        }else{
            Offset move = dirKeyPressedOffset();
            move.y = move.y < 0 ? 0 : move.y;
            if(frameTime - state.lastMoveDown > state.fallTime){
                move.y = 1;
                state.lastMoveDown = frameTime;
            }
            const Coord newPiecePos = coordAdd(state.piecePos, move);
            if(minosFit(state.board, state.piece.minos, newPiecePos))
                state.piecePos = newPiecePos;
        }

        if(keyPressed(SC_P))
            printBoard(state.board);

        drawAll(&state, off, scale);

        frameEnd(frameTime);
    }
    return 0;
}

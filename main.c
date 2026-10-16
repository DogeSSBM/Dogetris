#include "DogeLib/Includes.h"
#define XLEN            18
#define YLEN            21
#define BOARDXLEN       10
#define BOARDYLEN       20
#define DEFAULTSCALE    40
#include "PieceRotDefs.h"
#include "Pieces.h"

PieceType globalBoard[10*20];

typedef struct{
    PieceType *board;
    int linesCleared;
    Coord piecePos;
    Piece piece;
    PieceType nextPieceType;
    PieceType holdPiece;
    bool holdAvailable;
    uint lastMoveDown;
}GameState;

void windowInit(void)
{
    const Length defaultWindowLen = {
        .x = DEFAULTSCALE*XLEN,
        .y = DEFAULTSCALE*YLEN
    };
    setWindowLen(defaultWindowLen);
    const Coord defaultWindowPos = coordSub(
        coordDivi(getWinDisplayLen(), 2),
        coordDivi(defaultWindowLen, 2)
    );
    winSetPosCoord(defaultWindowPos);
    setWindowTitle("Dogetris");
}

int getScale(const Length windowLen)
{
    return imin(
        windowLen.x / XLEN,
        windowLen.y / YLEN
    );
}

Offset getOffset(const Length windowLen, const int scale)
{
    const Length totalLen = {.x=XLEN*scale, .y=YLEN*scale};
    const Length diff = coordSub(windowLen, totalLen);
    return coordDivi(diff, 2);
}

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

Coord atCoord(const Coord pos, const Offset off, const int scale)
{
    return coordAdd(coordMuli(pos, scale), off);
}

Coord atPos(const int x, const int y, const Offset off, const int scale)
{
    return coordAdd(coordMuli((const Coord){.x=x, .y=y}, scale), off);
}

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

bool placePiece(PieceType *board, const u16 minos, const Coord pos, const uint frameTime, const uint lastMoveDown)
{
    if(!canMoveDown(board, minos, pos)){
        if(dirKeyPressedOffset().y > 0 || frameTime - lastMoveDown > 1000)
            return true;
    }
    return false;
}

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
}

void loose(GameState *state, const Offset lastOff, int lastScale, const uint lastFrameTime)
{
    drawBorders(lastOff, lastScale);
    drawBoardLines(lastOff, lastScale);
    drawPiece(state->piece, state->piecePos, lastOff, lastScale);
    drawBoard(state->board, lastOff, lastScale);
    drawNext(state->nextPieceType, lastOff, lastScale);

    char *overStr = "Game Over!";
    char *contStr = "Hold space to start a new game.";

    frameEnd(lastFrameTime);
    int index = 0;
    Color overColor = RED;
    u8 *rgb[3] = {&overColor.r, &overColor.g, &overColor.b};
    while(keyHeldTime(SC_SPACE) < 1500){
        const uint frameTime = frameStart();

        if(keyPressed(SC_ESCAPE))
            exit(0);

        const Length windowLen = getWindowLen();
        const int scale = getScale(windowLen);
        const Offset off = getOffset(windowLen, scale);

        if(keyPressed(SC_P))
            printBoard(state->board);

        drawBorders(off, scale);
        drawBoardLines(off, scale);
        drawPiece(state->piece, state->piecePos, off, scale);
        drawBoard(state->board, off, scale);
        drawNext(state->nextPieceType, off, scale);
        drawHoldPiece(state->holdPiece, off, scale);
        drawLinesCleared(state->linesCleared, off, scale);

        setTextColor(overColor);
        setTextSize(scale*2);
        const Length overLen = textLength(overStr);
        const Coord overPos = iC(windowLen.x/2, windowLen.y/4);
        setColor(BLACK);
        fillRectCenteredCoordLength(overPos, overLen);
        setColor(DARKERGREY);
        fillBorderCenteredCoord(overPos, overLen, imax(scale/12, 4));
        drawTextCenteredCoord(overStr, overPos);

        setTextSize((scale*2)/3);
        setTextColor(LIGHTGREY);
        const Length contLen = textLength(contStr);
        const Coord contPos = iC(windowLen.x/2, windowLen.y/2);
        setColor(BLACK);
        fillRectCenteredCoordLength(contPos, contLen);
        setColor(DARKERGREY);
        fillBorderCenteredCoord(contPos, contLen, imax(scale/12, 4));
        drawTextCenteredCoord(contStr, contPos);

        if(keyState(SC_SPACE)){
            const Coord barPos = iC(windowLen.x/2, (windowLen.y/4)*3);
            setColor(BLACK);
            fillRectCenteredCoordLength(barPos, contLen);
            setColor(overColor);
            fillRectCoordLength(coordUncenter(barPos, contLen), iC(contLen.x*(keyHeldTime(SC_SPACE)/1500.0f), contLen.y));
            setColor(DARKERGREY);
            fillBorderCenteredCoord(barPos, contLen, imax(scale/12, 4));
        }

        if(*rgb[index] == 0)
            index = wrap(index+1, 0, 3);
        else{
            *rgb[index] = imax((int)*rgb[index] - 4, 0);
            *rgb[wrap(index+1, 0, 3)] = imin((int)*rgb[wrap(index+1, 0, 3)] + 4, 255);
        }

        frameEnd(frameTime);
    }
    resetState(state);
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

        if(keyPressed(SC_SPACE))
            state.piecePos = softDrop(state.board, state.piece.minos, state.piecePos);

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

        if(placePiece(state.board, state.piece.minos, state.piecePos, frameTime, state.lastMoveDown)){
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
            if(frameTime - state.lastMoveDown > 1000){
                move.y = 1;
                state.lastMoveDown = frameTime;
            }
            const Coord newPiecePos = coordAdd(state.piecePos, move);
            if(minosFit(state.board, state.piece.minos, newPiecePos))
                state.piecePos = newPiecePos;
        }

        if(keyPressed(SC_P))
            printBoard(state.board);

        drawBorders(off, scale);
        drawBoardLines(off, scale);
        drawPiece(state.piece, state.piecePos, off, scale);
        drawBoard(state.board, off, scale);
        drawNext(state.nextPieceType, off, scale);
        drawHoldPiece(state.holdPiece, off, scale);
        drawLinesCleared(state.linesCleared, off, scale);

        frameEnd(frameTime);
    }
    return 0;
}

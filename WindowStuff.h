#ifndef WINDOWSTUFF_H
#define WINDOWSTUFF_H

// inits size, position, and name of window
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

// gets the size that one mino should be based on window size
int getScale(const Length windowLen)
{
    return imin(
        windowLen.x / XLEN,
        windowLen.y / YLEN
    );
}

// gets the top left offset the board should be drawn at based on window size and scale
Offset getOffset(const Length windowLen, const int scale)
{
    const Length totalLen = {.x=XLEN*scale, .y=YLEN*scale};
    const Length diff = coordSub(windowLen, totalLen);
    return coordDivi(diff, 2);
}

// takes a board pos coord and returns its window position coord
Coord bwCoord(const Coord pos, const Offset off, const int scale)
{
    return coordAdd(coordMuli(pos, scale), off);
}

// takes a board x y pos and returns its window position coord
Coord bposwCoord(const int x, const int y, const Offset off, const int scale)
{
    return bwCoord(iC(x, y), off, scale);
}

// takes a window pos coord and returns its board coord
Coord wbCoord(const Coord pos, const Offset off, const int scale)
{
    return coordDivi(coordSub(pos, off), scale);
}

#endif
#ifndef WINDOWSTUFF_H
#define WINDOWSTUFF_H

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

Coord atCoord(const Coord pos, const Offset off, const int scale)
{
    return coordAdd(coordMuli(pos, scale), off);
}

Coord atPos(const int x, const int y, const Offset off, const int scale)
{
    return coordAdd(coordMuli((const Coord){.x=x, .y=y}, scale), off);
}


#endif
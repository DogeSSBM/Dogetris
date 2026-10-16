#ifndef DECLS_H
#define DECLS_H

// WindowStuff.h --------------------------------------------------------------
void windowInit(void);
int getScale(const Length windowLen);
Offset getOffset(const Length windowLen, const int scale);
Coord atCoord(const Coord pos, const Offset off, const int scale);
Coord atPos(const int x, const int y, const Offset off, const int scale);

// Pieces.h -------------------------------------------------------------------
bool minoPos(const u16 minos, const int x, const int y);
bool minoCoord(const u16 minos, const Coord pos);
Piece getPiece(const PieceType type, const int rotation);
bool pieceVisible(const u16 minos, const Coord pos);
int rotateOff(PieceType *board, const u16 minos, Coord *pos, const Offset off[5]);
bool rotate(PieceType *board, Piece *piece, Coord *pos, const int rotation);

// Board.h --------------------------------------------------------------------
void printBoard(PieceType *board);
void boardPlacePiece(PieceType *board, const Piece piece, const Coord pos);
bool minosFit(PieceType *board, const u16 minos, const Coord pos);
void shiftLinesDown(PieceType *board, const int stopLine);
bool lineFull(PieceType *board);
int clearLines(PieceType *board);
bool canMoveDown(PieceType *board, const u16 minos, Coord pos);
Coord softDrop(PieceType *board, const u16 minos, Coord pos);
bool placePiece(PieceType *board, const u16 minos, const Coord pos, const uint frameTime, const uint lastMoveDown);

// State.h --------------------------------------------------------------------
void resetState(GameState *state);

// Draw.h ---------------------------------------------------------------------
void drawBoardLines(const Offset off, const int scale);
void drawMinoPos(const int x, const int y, const Offset off, const int scale, const Color color1, const Color color2);
void drawBorders(const Offset off, const int scale);
void drawPiece(const Piece piece, const Coord pos, const Offset off, const int scale);
void drawNext(const PieceType piece, const Offset off, const int scale);
void drawHoldPiece(const PieceType piece, const Offset off, const int scale);
void drawLinesCleared(const int linesCleared, const Offset off, const int scale);
void drawBoard(PieceType *board, const Offset off, const int scale);

#endif
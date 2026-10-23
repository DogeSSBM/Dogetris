#ifndef DECLS_H
#define DECLS_H

// WindowStuff.h --------------------------------------------------------------
// inits size, position, and name of window
void windowInit(void);
// gets the size that one mino should be based on window size
int getScale(const Length windowLen);
// gets the top left offset the board should be drawn at based on window size and scale
Offset getOffset(const Length windowLen, const int scale);
// takes a board pos coord and returns its window position coord
Coord bwCoord(const Coord pos, const Offset off, const int scale);
// takes a board x y pos and returns its window position coord
Coord bposwCoord(const int x, const int y, const Offset off, const int scale);

// Pieces.h -------------------------------------------------------------------
// returns true if the mino at pos x y is filled
bool minoPos(const u16 minos, const int x, const int y);
// returns true if the mino at pos is filled
bool minoCoord(const u16 minos, const Coord pos);
// gets piece of corrosponding type and rotation
Piece getPiece(const PieceType type, const int rotation);
// returns true if all minos are visible on board
bool pieceVisible(const u16 minos, const Coord pos);
// sets pos to the first valid rotation offset and returns its index. if none are valid returns -1
int rotateOff(PieceType *board, const u16 minos, Coord *pos, const Offset off[5]);
// attempts to rotate piece. modifies piece and pos and returns true on success. otherwise returns false
bool rotate(PieceType *board, Piece *piece, Coord *pos, const int rotation);

// Board.h --------------------------------------------------------------------
// prints state of board to stdout
void printBoard(PieceType *board);
// places all valid minos of a piece on the board
void boardPlacePiece(PieceType *board, const Piece piece, const Coord pos);
// returns true if all minos of a piece fit on the board at pos. doesnt check minos above visible board
bool minosFit(PieceType *board, const u16 minos, const Coord pos);
// shifts all lines above stopLine down one. fills top row with PIECE_EMPTY
void shiftLinesDown(PieceType *board, const int stopLine);
// returns true if line at y is full of minos
bool lineFull(PieceType *board, const int y);
// clears all full lines on board. returns number of lines cleared
int clearLines(PieceType *board);
// returns true if all minos of piece fit when moved down one
bool canMoveDown(PieceType *board, const u16 minos, Coord pos);
// returns lowest possible pos where piece will fit. returns pos if minos already dont fit
Coord softDrop(PieceType *board, const u16 minos, Coord pos);
// returns true if piece should be placed based on when it was last moved down, down key state, and if piece can move down
bool shouldPlacePiece(PieceType *board, const u16 minos, const Coord pos, const uint frameTime, const uint lastMoveDown);

// State.h --------------------------------------------------------------------
// resets state to starting game state
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
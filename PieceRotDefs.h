#ifndef PIECEROTDEFS_H
#define PIECEROTDEFS_H

// PIECE_I
#define I_R0 0b\
0000\
1111\
0000\
0000
#define I_R1 0b\
0010\
0010\
0010\
0010
#define I_R2 0b\
0000\
0000\
1111\
0000
#define I_R3 0b\
0100\
0100\
0100\
0100
const u16 rots_I[4] = {I_R0, I_R1, I_R2, I_R3};

// PIECE_J
#define J_R0 0b\
1000\
1110\
0000\
0000
#define J_R1 0b\
0110\
0100\
0100\
0000
#define J_R2 0b\
0000\
1110\
0010\
0000
#define J_R3 0b\
0100\
0100\
1100\
0000
const u16 rots_J[4] = {J_R0, J_R1, J_R2, J_R3};

// PIECE_L
#define L_R0 0b\
0010\
1110\
0000\
0000
#define L_R1 0b\
0100\
0100\
0110\
0000
#define L_R2 0b\
0000\
1110\
1000\
0000
#define L_R3 0b\
1100\
0100\
0100\
0000
const u16 rots_L[4] = {L_R0, L_R1, L_R2, L_R3};

// PIECE_O
#define O_R0 0b\
0110\
0110\
0000\
0000
#define O_R1 0b\
0110\
0110\
0000\
0000
#define O_R2 0b\
0110\
0110\
0000\
0000
#define O_R3 0b\
0110\
0110\
0000\
0000
const u16 rots_O[4] = {O_R0, O_R1, O_R2, O_R3};

// PIECE_S
#define S_R0 0b\
0110\
1100\
0000\
0000
#define S_R1 0b\
0100\
0110\
0010\
0000
#define S_R2 0b\
0000\
0110\
1100\
0000
#define S_R3 0b\
1000\
1100\
0100\
0000
const u16 rots_S[4] = {S_R0, S_R1, S_R2, S_R3};

// PIECE_T
#define T_R0 0b\
0100\
1110\
0000\
0000
#define T_R1 0b\
0100\
0110\
0100\
0000
#define T_R2 0b\
0000\
1110\
0100\
0000
#define T_R3 0b\
0100\
1100\
0100\
0000
const u16 rots_T[4] = {T_R0, T_R1, T_R2, T_R3};

// PIECE_Z
#define Z_R0 0b\
1100\
0110\
0000\
0000
#define Z_R1 0b\
0010\
0110\
0100\
0000
#define Z_R2 0b\
0000\
1100\
0110\
0000
#define Z_R3 0b\
0100\
1100\
1000\
0000
const u16 rots_Z[4] = {Z_R0, Z_R1, Z_R2, Z_R3};

#endif
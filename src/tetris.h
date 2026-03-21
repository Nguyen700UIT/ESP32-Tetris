#pragma once
#include <Arduino.h>
#include <pieces.h>

//Tetris board matrix
#define BOARD_NUM_ROW 16 
#define BOARD_NUM_COL 10

extern uint8_t tetrisBoard[BOARD_NUM_ROW][BOARD_NUM_COL];

struct Piece
{
    int type;
    int x;
    int y;
    int rotation;
    uint8_t shape[4][4];
};

extern Piece currPiece;
extern const int gravity;


bool checkCollision(u_int8_t shape[4][4], int x, int y);
void initBoard();
void spawnPiece();
void clearLine();
void movePiece();
void rotatePiece();
void lockPiece();
void drawPiece(const Piece& currPiece);

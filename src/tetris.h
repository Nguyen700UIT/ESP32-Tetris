#pragma once
#include <Arduino.h>
#include <pieces.h>

//Tetris board matrix 60x60 pixel
#define BOARD_NUM_ROW 10 
#define BOARD_NUM_COL 10
#define BOARD_PIXEL_WIDTH 60
#define BOARD_PIXEL_HEIGHT 60
#define BOARD_OFFSET 2 // Bỏ 2 pixel đầu và 2 pixel cuối với mỗi trục X và Y


extern uint8_t tetrisBoard[BOARD_NUM_ROW][BOARD_NUM_COL];

struct Piece
{
    int x;
    int y;
    uint8_t shape[4][4];
};

extern int score;
extern Piece currPiece;
extern u_int8_t nextPieceShape[4][4];
extern unsigned long lastDelayedFall;
extern unsigned long lastGameOverTime;



inline u_int8_t pixelCordToCellCord(int cord); //Can be x or y
bool checkCollision(u_int8_t shape[4][4], int x, int y);
void initBoard();
void gameOver();
void drawBoard();
void spawnPiece();
void initPiece();
int clearLine();
void movePiece();
void delayedFall();
void rotatePiece();
void lockPiece();
void drawNextPiece();
void drawPiece();
void drawScore();

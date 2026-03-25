#pragma once
#include <Arduino.h>
#include <pieces.h>

//Tetris board matrix 200x240 pixel
#define BOARD_NUM_ROW 12 
#define BOARD_NUM_COL 10
#define BOARD_PIXEL_WIDTH 200 
#define BOARD_PIXEL_HEIGHT 240
#define BOARD_OFFSET 0 // Bỏ 0 pixel đầu và  pixel cuối với mỗi trục X và Y

extern uint16_t tetrisBoard[BOARD_NUM_ROW][BOARD_NUM_COL];

struct Piece
{
    int x;
    int y;
    uint8_t shape[4][4];
    uint16_t pieceColor;
};

extern int score;
extern Piece prevPiece;
extern Piece currPiece;
extern uint8_t nextPieceShape[4][4];
extern uint16_t nextPieceColor;
extern unsigned long lastDelayedFall;
extern unsigned long lastGameOverTime;



inline u_int8_t pixelCordToCellCord(int cord); //Can be x or y
bool checkCollision(u_int8_t shape[4][4], int x, int y);
void gameReset();
void initBoard();
void gameOver();
void eraseBoard();
void drawBoard();
void spawnPiece();
void initPiece();
int clearLine();
void movePiece();
void delayedFall();
void rotatePiece();
void lockPiece();

void erasePlayingUI();
void drawPlayingUI();
void eraseGameOverUI();
void drawGameOverUI();
void drawNextPiece();
void drawPiece();
void erasePrevPiece();
void drawGameOver();
void drawScore();

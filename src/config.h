#pragma once

//Screen definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//Buttons
#define UP 4    
#define DOWN 5  
#define LEFT 16
#define RIGHT 17
#define RESET 13
#define BUTTON_DEBOUNCE 100
//Game definitions
#define BLOCK_SIZE 4
#define TETRIS_BOARD_WIDTH 64
#define TETRIS_BOARD_HEIGHT 64
#define UI_BOARD_WIDTH 88
#define UI_BOARD_HEIGHT 64



//Handling buttons
extern volatile bool isUp; 
extern volatile bool isDown;
extern volatile bool isLeft;
extern volatile bool isRight;
extern volatile unsigned long lastInterruptTime[4];
//Stop ISR flag
extern volatile bool gameOverFlag;
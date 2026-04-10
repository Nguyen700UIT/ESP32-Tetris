#include <Arduino.h>
#include <WiFi.h>
#include <esp_bt.h>
#include <audio.h>
#include <display.h>
#include <tetris.h>
#include <config.h>



void IRAM_ATTR upISR()
{
  if(gameOverFlag) return;
  unsigned long currInterrruptTime = millis();
  if (currInterrruptTime - lastInterruptTime[0] > 100)
  {
    isUp = true;
    lastInterruptTime[0] = currInterrruptTime;
  }
}

void IRAM_ATTR downISR()
{
  if(gameOverFlag) return;
  unsigned long currInterrruptTime = millis();
  if (currInterrruptTime - lastInterruptTime[1] > 100)
  {
    isDown = true;
    lastInterruptTime[1] = currInterrruptTime;
  }
}

void IRAM_ATTR leftISR()
{
  if(gameOverFlag) return;
  unsigned long currInterrruptTime = millis();
  if (currInterrruptTime - lastInterruptTime[2] > 100)
  {
    isLeft = true;
    lastInterruptTime[2] = currInterrruptTime;
  }
}

void IRAM_ATTR rightISR()
{
  if(gameOverFlag) return;
  unsigned long currInterrruptTime = millis();
  if (currInterrruptTime - lastInterruptTime[3] > 100)
  {
    isRight = true;
    lastInterruptTime[3] = currInterrruptTime;
  }
}

void IRAM_ATTR resetISR()
{
  unsigned long currInterruptTime = millis();
  if(currInterruptTime - lastResetInterruptTime > 100)
  {
    reseted = true;
    lastResetInterruptTime = currInterruptTime;
  }
}

void setup() {
  //Turn off WiFi BLuetooth
  WiFi.mode(WIFI_OFF);
  btStop();

  gameOverFlag = false;

  initAudio();

  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);
  Serial.begin(9600);
  initDisplay();
 

  attachInterrupt(UP, upISR, FALLING);
  attachInterrupt(DOWN, downISR, FALLING);
  attachInterrupt(LEFT, leftISR, FALLING);
  attachInterrupt(RIGHT, rightISR, FALLING);
  attachInterrupt(RESET, resetISR, FALLING);
  initPiece();

  drawPlayingUI();
  drawNextPiece();
  drawScore();
}

void loop() {
  if (!gameOverFlag)
  {
    //Flag handling
    erasedPlayingUI = false;
    erasedDeathScreen = false;
    //Prevent future reset
    if (reseted)
      reseted = false;
    //Logic
    movePiece();
    calculateGhostPiece();
    bool drawingNextPieceFlag = delayedFallAndLogic();
    //Drawing
    renderBoard();
    if(drawingNextPieceFlag)
    {
      eraseNextPiece();
      drawNextPiece();  
    }

    if(prevScore != score)
    {
      drawScore();
      prevScore = score;
    }
  }
  else
  {
    redrawUI = true;
    unsigned currGameOverTime = millis();
    if (currGameOverTime - lastGameOverTime > 1500)
    {
      if(!erasedDeathScreen)
      {
        eraseScreen();
        erasedDeathScreen = true;
      }
      drawGameOverUI();
      if(erasedDeathScreen && reseted)
      {
        gameReset();
        drawNextPiece();
        drawScore();
        drawPlayingUI();
      }

    }
    else //Death screen
    {
      if(!erasedPlayingUI)
      {
        eraseScreen();
        erasedPlayingUI = true;
      }
      drawDeathUI();
    }
  }
}

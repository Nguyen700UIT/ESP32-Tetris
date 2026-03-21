#include <Arduino.h>
#include <display.h>
#include <tetris.h>
#include <config.h>

volatile bool gameOverFlag = false;

//Handling buttons
volatile bool isUp = false;
volatile bool isDown = false;
volatile bool isLeft = false;
volatile bool isRight = false;
volatile unsigned long lastInterruptTime[4] = {0};

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

void setup() {
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  Serial.begin(9600);
  initDisplay();

  attachInterrupt(UP, upISR, FALLING);
  attachInterrupt(DOWN, downISR, FALLING);
  attachInterrupt(LEFT, leftISR, FALLING);
  attachInterrupt(RIGHT, rightISR, FALLING);
}

void loop() {

}

#include <Arduino.h>
#include <Wire.h>
#include <display.h>
#include <config.h>
#include <tetris.h>

void setup() {
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);
  
  Serial.begin(9600);
  initDisplay();
}

void loop() {

}

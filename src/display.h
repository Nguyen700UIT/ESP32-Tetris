#pragma once
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

extern Adafruit_SSD1306 display;

void initDisplay();
void drawBlock(int x, int y);
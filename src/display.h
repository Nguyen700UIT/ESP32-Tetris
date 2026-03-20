#pragma once
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

void initDisplay();
void drawBlock(int x, int y);
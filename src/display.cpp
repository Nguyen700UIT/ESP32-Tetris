#include <display.h>
#include <config.h>

void initDisplay()
{
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
}


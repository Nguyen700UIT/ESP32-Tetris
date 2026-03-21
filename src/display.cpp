#include <display.h>
#include <Wire.h>
#include <config.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initDisplay()
{
    //Init OLED
    Wire.begin(21, 22);
    if(display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("Display allocated");
    }
    else 
        Serial.println("Display allocation failed");
    
    display.clearDisplay();
    display.display();
}


#include <tetris.h>
#include <pieces.h>
#include <display.h>
#include <config.h>

void drawPiece(int currPiece, int X, int Y) //X, Y = Tetris board coordinates (0 <= X <= 40) & (0 <= Y <= 64)
{
  for (int tempY = 0; tempY < 4; tempY++)
  {
    for (int tempX = 0; tempX < 4; tempX++)
    {
      if (pieces[currPiece][tempY][tempX])
      {
        display.fillRect(X + tempX*BLOCK_SIZE, Y + tempY*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SSD1306_WHITE);
        display.drawRect(X + tempX*BLOCK_SIZE, Y + tempY*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SSD1306_BLACK);
      }
    }
  }
}
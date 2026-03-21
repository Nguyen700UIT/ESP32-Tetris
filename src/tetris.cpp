#include <tetris.h>
#include <pieces.h>
#include <display.h>
#include <config.h>

uint8_t tetrisBoard[BOARD_NUM_ROW][BOARD_NUM_COL];
Piece currPiece;
unsigned long lastDelayedFall = 0;


inline u_int8_t pixelCordToCellCord(int cord)
{
    return (cord - BOARD_OFFSET) / BLOCK_SIZE;
}


void initBoard()
{
  for (int y = 0; y < BOARD_NUM_ROW; y++)
  {
    for (int x = 0; x < BOARD_NUM_COL; x++)
      tetrisBoard[y][x] = 0;
  }
}

void spawnPiece()
{
  currPiece.type = random(0, 7);
  currPiece.x = BOARD_OFFSET + (BOARD_PIXEL_WIDTH/2) - (BLOCK_SIZE*2); //Spawn tại tâm của piece
  currPiece.y = BOARD_OFFSET;
  memcpy(currPiece.shape, piecesShape[currPiece.type], sizeof(currPiece.shape));
  currPiece.rotation = 0;
  lastDelayedFall = millis();
}

bool checkCollision(u_int8_t shape[4][4], int x, int y)
{
  for (int tempY = 0; tempY < 4; ++tempY)
  {
    for (int tempX = 0; tempX < 4; ++tempX)
    {
      if (!shape[tempY][tempX]) continue;


      int px = x + tempX*BLOCK_SIZE; //Pixel cord;
      int py = y + tempY*BLOCK_SIZE; //Pixel cord;
      //Wall collision x cords
      if (px < BOARD_OFFSET || px + BLOCK_SIZE > BOARD_OFFSET + BOARD_PIXEL_WIDTH)
        return true;
      
      //Wall collision y cords
      if(py < BOARD_OFFSET || py + BLOCK_SIZE > BOARD_OFFSET + BOARD_PIXEL_HEIGHT)
        return true;
      
      int boardX = pixelCordToCellCord(px);
      int boardY = pixelCordToCellCord(py);
      
      if (boardY >= 0 && tetrisBoard[boardY][boardX])
        return true;      
    }
  }
  return false;
}

void movePiece()
{
  if(!checkCollision(currPiece.shape, currPiece.x, currPiece.y + BLOCK_SIZE))
  {
    if (isUp)
    {
      rotatePiece();
      isUp = false;
    }
    else if (isLeft)
    {
      currPiece.x -= BLOCK_SIZE;
      isLeft = false;
    }
    else if (isRight)
    {
      currPiece.x += BLOCK_SIZE;
      isRight = false;
    }
    else if (isDown)
    {
      currPiece.y += BLOCK_SIZE;
      isDown = false; 
    }
  }
  
}

void rotatePiece()
{
  //Tranpose matrix
  u_int8_t temp[4][4];
  for (int i = 0; i < 4; ++i) //Hàng i = cột i
  {
    for (int j = 0; j < 4; ++j)
    {
      temp[i][j] = currPiece.shape[j][i];
    }
  }

  for (int i = 0; i < 4; ++i) //Đảo cột
  {
    for (int j = 0; j < 2; ++j)
    {
      u_int8_t swap = temp[j][i];
      temp[j][i] = temp[3-j][i];
      temp[3-j][i] = swap;
    }
  }

  if (!checkCollision(temp, currPiece.x, currPiece.y))
    memcpy(currPiece.shape, temp, sizeof(currPiece.shape));
}

void delayedFall()
{
  unsigned long now = millis();
  if(now - lastDelayedFall > 5000)
  {
    if(!checkCollision(currPiece.shape, currPiece.x, currPiece.y + BLOCK_SIZE))
      currPiece.y += GRAVITY;
    lastDelayedFall = now;
    Serial.println(now - lastDelayedFall);
  }

}

void drawPiece(const Piece& currPiece) //X, Y = Tetris board coordinates (0 <= X <= 64) & (0 <= Y <= 64)
{
  for (int tempY = 0; tempY < 4; tempY++)
  {
    for (int tempX = 0; tempX < 4; tempX++)
    {
      if (currPiece.shape[tempY][tempX])
      {
        display.fillRect(currPiece.x + tempX*BLOCK_SIZE, currPiece.y + tempY*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SSD1306_WHITE);
        display.drawRect(currPiece.x + tempX*BLOCK_SIZE, currPiece.y + tempY*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SSD1306_BLACK);
      }
    }
  }
}
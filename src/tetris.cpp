#include <tetris.h>
#include <pieces.h>
#include <display.h>
#include <config.h>

uint8_t tetrisBoard[BOARD_NUM_ROW][BOARD_NUM_COL];
Piece currPiece;
const int gravity = 3;

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
  currPiece.x = 20;
  currPiece.y = 0;
  memcpy(currPiece.shape, piecesShape[currPiece.type], sizeof(currPiece.shape));
  currPiece.rotation = 0;
}

void movePiece()
{
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
  memcpy(currPiece.shape, temp, sizeof(currPiece.shape));
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
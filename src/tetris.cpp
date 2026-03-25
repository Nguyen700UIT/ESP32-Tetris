#include <tetris.h>
#include <pieces.h>
#include <display.h>
#include <config.h>
//Variables
uint16_t tetrisBoard[BOARD_NUM_ROW][BOARD_NUM_COL];
int score = 0;
Piece prevPiece;
Piece currPiece;
uint8_t nextPieceShape[4][4];
uint16_t nextPieceColor;
unsigned long lastDelayedFall = 0;
unsigned long lastGameOverTime= 0 ;


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

void drawBoard()
{
  for (int y = 0; y < BOARD_NUM_ROW; y++)
  {
    for (int x = 0; x < BOARD_NUM_COL; x++)
    {
      if (tetrisBoard[y][x])
      {
        tft.fillRect(BOARD_OFFSET + x*BLOCK_SIZE, BOARD_OFFSET + y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, tetrisBoard[y][x]);
        tft.drawRect(BOARD_OFFSET + x*BLOCK_SIZE, BOARD_OFFSET + y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, TFT_WHITE);      
      }
    }
  }
}

void eraseBoard()
{
  for (int y = 0; y < BOARD_NUM_ROW; y++)
  {
    for (int x = 0; x < BOARD_NUM_COL; x++)
    {
      if (tetrisBoard[y][x])
      {
        tft.fillRect(BOARD_OFFSET + x*BLOCK_SIZE, BOARD_OFFSET + y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, TFT_BLACK);      
      }
    }
  }
}

void gameOver()
{
  if(checkCollision(currPiece.shape, currPiece.x, currPiece.y))
  {
    gameOverFlag = true;
    lastGameOverTime = millis();
  }
}

void spawnPiece()
{
  currPiece.x = BOARD_OFFSET + (BOARD_PIXEL_WIDTH/2) - (BLOCK_SIZE*2); //Spawn tại tâm của piece
  currPiece.y = BOARD_OFFSET;
  memcpy(currPiece.shape, nextPieceShape, sizeof(currPiece.shape));
  currPiece.pieceColor = nextPieceColor;
  //Next piece
  int nextType = random(0, 7);
  memcpy(nextPieceShape, piecesShape[nextType], sizeof(nextPieceShape));
  nextPieceColor = pieceColor[nextType];
}

void initPiece()
{
  //Prev piece
  prevPiece.pieceColor = 0;
  prevPiece.x = -1000;
  prevPiece.y = -1000;
  memcpy(prevPiece.shape, piecesShape[0], sizeof(prevPiece.shape));

  //Current piece
  int type = random(0, 7);
  currPiece.pieceColor = pieceColor[type];
  currPiece.x = BOARD_OFFSET + (BOARD_PIXEL_WIDTH/2) - (BLOCK_SIZE*2); //Spawn tại tâm của piece
  currPiece.y = BOARD_OFFSET;
  memcpy(currPiece.shape, piecesShape[type], sizeof(currPiece.shape));

  //Next piece
  int nextType = random(0, 7);
  memcpy(nextPieceShape, piecesShape[nextType], sizeof(nextPieceShape));
  
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
    if (isUp)
    {
      rotatePiece();
      isUp = false;
    }
    else if (isLeft)
    {
      if (!checkCollision(currPiece.shape, currPiece.x - BLOCK_SIZE, currPiece.y))
        currPiece.x -= BLOCK_SIZE;
      isLeft = false;
    }
    else if (isRight)
    {
      if (!checkCollision(currPiece.shape, currPiece.x + BLOCK_SIZE, currPiece.y))
        currPiece.x += BLOCK_SIZE;
      isRight = false;
    }
    else if (isDown)
    {
      if (!checkCollision(currPiece.shape, currPiece.x, currPiece.y + BLOCK_SIZE))
        currPiece.y += BLOCK_SIZE;
      isDown = false; 
    }
}

void lockPiece() //Lock piece and add piece to board array
{
  for (int tempY = 0; tempY < 4; ++tempY)
  {
    for (int tempX = 0; tempX < 4; ++tempX)
    {
      if (!currPiece.shape[tempY][tempX]) continue;


      int px = currPiece.x + tempX*BLOCK_SIZE; //Pixel cord;
      int py = currPiece.y + tempY*BLOCK_SIZE; //Pixel cord;
      
      int boardX = pixelCordToCellCord(px);
      int boardY = pixelCordToCellCord(py);
      
      if (boardY >= 0)
      {
        tetrisBoard[boardY][boardX] = currPiece.pieceColor;
      }
    }
  }
}



int clearLine()
{
  int cleared = 0;
  for (int y = BOARD_NUM_ROW - 1; y > 0; --y)
  {
    bool full = true;
    for (int x = 0; x < BOARD_NUM_COL; ++x)
    {
      if(tetrisBoard[y][x] == 0)
      {
        full = false;
        break;
      }
    }
    if (full)
    {
      for (int i = y; i > 0; --i) // Shift xuống
      {
      memcpy(tetrisBoard[i], tetrisBoard[i-1], sizeof(tetrisBoard[i]));
      }
      //Sau khi shift thì hàng trên cùng sẽ trống
      memset(tetrisBoard[0], 0, sizeof(tetrisBoard[0]));
      //Kiểm tra sau khi shift hàng mới có full không
      y++;
      cleared++;
    }
  }
  return cleared;
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
  if(now - lastDelayedFall > 500)
  {
    if (!checkCollision(currPiece.shape, currPiece.x, currPiece.y + BLOCK_SIZE))
      currPiece.y += GRAVITY;
    else
    {
      eraseBoard();
      drawBoard();
      lockPiece();
      int temp = clearLine();
      switch(temp)
      {
        case 1: 
          score += 100;
          break;
        case 2: 
          score += 300;
          break;
        case 3: 
          score += 500;
          break;
        case 4:
          score += 800;
          break;
      }
      spawnPiece();
      gameOver();
    }
    lastDelayedFall = now;
  }
}



void drawPiece() //X, Y = Tetris board coordinates (0 <= X <= 64) & (0 <= Y <= 64)
{
  for (int tempY = 0; tempY < 4; tempY++)
  {
    for (int tempX = 0; tempX < 4; tempX++)
    {
      if (currPiece.shape[tempY][tempX])
      {
        tft.fillRect(currPiece.x + tempX*BLOCK_SIZE, currPiece.y + tempY*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, currPiece.pieceColor);
        tft.drawRect(currPiece.x + tempX*BLOCK_SIZE, currPiece.y + tempY*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, TFT_BLACK);
      }
    }
  }
}

void erasePlayingUI()
{
  tft.drawFastVLine(BOARD_PIXEL_WIDTH + 4, 0, BOARD_PIXEL_HEIGHT, TFT_BLACK);

  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(BOARD_PIXEL_WIDTH + 10, BOARD_PIXEL_HEIGHT - (BOARD_PIXEL_HEIGHT/2));
  tft.println("Score: ");
  tft.setCursor(BOARD_PIXEL_WIDTH + 10, BOARD_PIXEL_HEIGHT - (BOARD_PIXEL_HEIGHT/2) + 20);
  tft.println(score);

  tft.setCursor(BOARD_PIXEL_WIDTH + 10 + 20, BOARD_PIXEL_HEIGHT - (BOARD_PIXEL_HEIGHT/2));
  tft.println("Next piece: ");
}

void drawPlayingUI()
{

  tft.drawFastVLine(BOARD_PIXEL_WIDTH + 4, 0, BOARD_PIXEL_HEIGHT, TFT_WHITE);

  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(BOARD_PIXEL_WIDTH + 10, BOARD_PIXEL_HEIGHT - (BOARD_PIXEL_HEIGHT/2));
  tft.println("Score: ");
  tft.setCursor(BOARD_PIXEL_WIDTH + 10, BOARD_PIXEL_HEIGHT - (BOARD_PIXEL_HEIGHT/2) + 20);
  tft.println(score);

  tft.setCursor(BOARD_PIXEL_WIDTH + 10 + 20, BOARD_PIXEL_HEIGHT - (BOARD_PIXEL_HEIGHT/2));
  tft.println("Next piece: ");
}

void eraseGameOverUI()
{
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  tft.println("GAME OVER");
}

void drawGameOverUI()
{
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  tft.println("GAME OVER");
}

void drawNextPiece()
{
  for (int tempY = 0; tempY < 4; tempY++)
  {
    for (int tempX = 0; tempX < 4; tempX++)
    {
      if (nextPieceShape[tempY][tempX])
      {
        tft.fillRect(280 + tempX*BLOCK_SIZE,100  + tempY*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, nextPieceColor);
        tft.drawRect(280 + tempX*BLOCK_SIZE, 100 + tempY*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, TFT_WHITE);
      }
    }
  }
}

void drawGameOver()
{
  tft.setTextSize(2);
  tft.setTextColor(SSD1306_WHITE);
  tft.setCursor(15, 20);
  tft.println("GAME OVER");
  if (reseted)
  {  
    gameReset();
    reseted = false;
  }
}



void gameReset()
{
  score = 0;
  initBoard();
  initPiece();
  gameOverFlag = false;
}

void erasePrevPiece()
{

}
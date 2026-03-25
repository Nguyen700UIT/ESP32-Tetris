#include <pieces.h>

//Pieces
const uint8_t piecesShape[7][4][4] = 
{
  //I piece
  {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },

  //T piece 
  {
    {0, 0, 0, 0}, 
    {1, 1, 1, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0}
  },
  //O piece
  {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
  },
  //S piece
  {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
  },
  //Z piece
  {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 0}
  },
  //J piece
  {
    {1, 0, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  //L piece
  {
    {0, 0, 0, 1},
    {0, 1, 1, 1},
    {0, 0, 0, 0}, 
    {0, 0, 0, 0}
  }
};

//Pieces color
const uint16_t pieceColor[7] = {TFT_CYAN, TFT_PURPLE, TFT_YELLOW, TFT_GREEN, TFT_RED, TFT_BLUE, TFT_ORANGE};
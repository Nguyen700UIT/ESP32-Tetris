#pragma once
struct Piece
{
    int type;
    int x;
    int y;
    int rotation;
};

extern Piece currPiece;

void drawPiece(int currPiece, int X, int Y);

void spawnPiece();
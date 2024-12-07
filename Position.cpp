#include "header.h"
#include <cmath>

Position::Position(int x, int y)
{
    posX = x;
    posY = y;
}

bool Position::operator==(const Position& p)
{
    return (posX == p.posX) && (posY == p.posY);
}

float Position::sld(const Position& p) //Calculating Straight Line Distance
{
    return sqrt(pow(posX - p.posX, 2) + pow(posY - p.posY, 2));
}
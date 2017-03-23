#include "trianglenet.h"

TriangleNet::TriangleNet()
{
    this->minTurn = 1;
    this->maxTurn = 5;
    this->countDirection = 6;
}

Coord TriangleNet::getDirectionCoord(int direction, Coord c)
{
    switch(direction)
    {
    case 0:c.x--;
        break;
    case 1:c.x++;
        break;
    case 2:c.y--;
        break;
    case 3:c.y++;
        break;
    case 4:c.x++;c.y--;
        break;
    case 5:c.x--;c.y++;
        break;
    }
    return c;
}

#ifndef NET_H
#define NET_H
#include "settings.h"

struct Coord
{
    int x;
    int y;
    Coord(int x,int y):x(x),y(y){}
    Coord(){}
};

class Net
{
protected:
    int minTurn;
    int maxTurn;
    int countDirection;
public:
    int turnToDirection(int currentDirection,int turn);
    virtual Coord getDirectionCoord(int direction, Coord c) = 0;
    int getMinTurn();
    int getMaxTurn();
    Net();
};

#endif // NET_H

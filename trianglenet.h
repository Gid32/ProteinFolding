#ifndef TRIANGLENET_H
#define TRIANGLENET_H
#include "net.h"

class TriangleNet
        :public Net
{
public:
    TriangleNet();
    Coord getDirectionCoord(int direction, Coord c);
};

#endif // TRIANGLENET_H

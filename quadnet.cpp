#include "quadnet.h"

QuadNet::QuadNet()
{
    this->minTurn = 1;
    this->maxTurn = 3;
    this->countDirection = 4;
}

QVector3D QuadNet::getDirectionCoord(int direction, QVector3D c, bool isDraw)
{
    int operation = isDraw ? DISTANCE_NODE : 1;
    switch(direction)
    {
    case 0:c.setX(c.x()-operation);
        break;
    case 1:c.setX(c.x()+operation);
        break;
    case 2:c.setY(c.y()-operation);
        break;
    case 3:c.setY(c.y()+operation);
        break;
    }
    return c;
}

QVector3D QuadNet::getCoords(QVector3D prevCoord, int direction)
{
    QVector3D coords;
    coords = prevCoord;

    return coords;
}

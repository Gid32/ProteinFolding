#include "trianglenet.h"

TriangleNet::TriangleNet()
{
    this->minTurn = 1;
    this->maxTurn = 5;
    this->countDirection = 6;
}

QVector3D TriangleNet::getDirectionCoord(int direction, QVector3D c, bool isDraw)
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
    case 4:c.setX(c.x()+operation);c.setY(c.y()-operation);
        break;
    case 5:c.setX(c.x()-operation);c.setY(c.y()+operation);
        break;
    }
    return c;
}

QVector3D TriangleNet::getCoords(QVector3D mCoord)
{
    QVector3D coords;
    coords.setX(mCoord.x()+mCoord.y()/2.0);
    coords.setY(mCoord.y()*0.8660254037844);
    return coords;
}

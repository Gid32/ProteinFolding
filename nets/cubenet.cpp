#include "cubenet.h"

CubeNet::CubeNet()
{
    this->minTurn = 1;
    this->maxTurn = 5;
    this->countDirection = 6;
}

QVector3D CubeNet::getDirectionCoord(int direction, QVector3D c, bool isDraw)
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
    case 4:c.setZ(c.z()+operation);
        break;
    case 5:c.setZ(c.z()-operation);
        break;
    }
    return c;
}

QVector3D CubeNet::getCoords(QVector3D mCoord)
{
    return mCoord;
}

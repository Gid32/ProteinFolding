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

QVector3D TriangleNet::getCoords(QVector3D prevCoord, int direction)
{
    QVector3D coords;
    qDebug()<<prevCoord.x()<<" - "<<prevCoord.y();
    coords.setX(prevCoord.x()+prevCoord.y()/2.0);
    coords.setY(prevCoord.y()*COORD_FORMULE);
    qDebug()<<coords.x()<<" - "<<coords.y();

    return coords;
}

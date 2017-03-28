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
    //qDebug()<<mCoord.x()<<" - "<<mCoord.y();
    coords.setX(mCoord.x()+mCoord.y()/2.0);
    coords.setY(mCoord.y()*COORD_FORMULE);
//    coords.setX(coords.x()-(int)COUNT/2);
//    coords.setY(coords.y()-(int)COUNT/2);
    //coords.setZ(0+mCoord.z());
//    coords.setX(coords.x()+COUNT/2);
//    coords.setY(coords.y()+COUNT/2);
//    coords.setZ(COUNT/2);
    //qDebug()<<coords.x()<<" - "<<coords.y();

    return coords;
}

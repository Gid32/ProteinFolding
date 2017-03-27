#include "triangle3dnet.h"

Triangle3DNet::Triangle3DNet()
{
    this->minTurn = 1;
    this->maxTurn = 11;
    this->countDirection = 12;
}

QVector3D Triangle3DNet::getDirectionCoord(int direction, QVector3D c, bool isDraw)
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
    case 6:c.setX(c.x()+operation);c.setY(c.y()-operation);
        break;
    case 7:c.setX(c.x()+operation);c.setZ(c.z()-operation);
        break;
    case 8:c.setX(c.x()-operation);c.setY(c.y()+operation);
        break;
    case 9:c.setY(c.y()+operation);c.setZ(c.z()-operation);
        break;
    case 10:c.setY(c.y()-operation);c.setZ(c.z()+operation);
        break;
    case 11:c.setX(c.x()-operation);c.setZ(c.z()+operation);
        break;
    }
    return c;
}

QVector3D Triangle3DNet::getCoords(QVector3D mCoord, int direction)
{
    QVector3D coords;

//    qDebug()<<prevCoord.x()<<" - "<<prevCoord.y()<<" - "<<prevCoord.z();;
//    coords.setY(prevCoord.y()/DIV);
//    coords.setX(prevCoord.x()-coords.y()/2);
//    //add direction
//    qDebug()<<coords.x()<<" - "<<coords.y()<<" - "<<coords.z();
//    coords = getDirectionCoord(direction,coords,true);
//    coords.setX(coords.x()+coords.y()/2.0);
//    coords.setY(coords.y()*DIV);
//    qDebug()<<coords.x()<<" - "<<coords.y()<<" - "<<coords.z();
//    qDebug()<<"ENDgetCoords";


    coords.setX(mCoord.x()*SQRT3DIV2 + mCoord.z()*SQRT3DIV6);
    coords.setY(mCoord.x()*0.5 + mCoord.y() + mCoord.z()*0.5);
    coords.setZ(mCoord.z()*SQRT33DIV6);

    return coords;

}

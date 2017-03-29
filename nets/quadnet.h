#ifndef QUADNET_H
#define QUADNET_H

#include "net.h"

class QuadNet
        :public Net
{
    Q_OBJECT
public:
    QuadNet();
    QVector3D getDirectionCoord(int direction, QVector3D c,bool isDraw = false);
    QVector3D getCoords(QVector3D mCoord);
};

#endif // QUADNET_H

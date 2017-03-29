#ifndef TRIANGLENET_H
#define TRIANGLENET_H
#include "net.h"
class TriangleNet
        :public Net
{
     Q_OBJECT
private:
public:
    explicit TriangleNet();
    QVector3D getDirectionCoord(int direction, QVector3D c,bool isDraw = false);
    QVector3D getCoords(QVector3D prevCoord);
};

#endif // TRIANGLENET_H

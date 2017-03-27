#ifndef TRIANGLENET_H
#define TRIANGLENET_H
#include "net.h"
#define COORD_FORMULE 0.8660254037844
class TriangleNet
        :public Net
{
     Q_OBJECT
public:
    explicit TriangleNet();
    QVector3D getDirectionCoord(int direction, QVector3D c,bool isDraw = false);
    QVector3D getCoords(QVector3D prevCoord, int direction);
};

#endif // TRIANGLENET_H

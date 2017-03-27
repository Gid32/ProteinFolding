#ifndef CUBENET_H
#define CUBENET_H

#include <QObject>
#include "net.h"

class CubeNet : public Net
{
    Q_OBJECT
public:
    explicit CubeNet();
    QVector3D getDirectionCoord(int direction, QVector3D c,bool isDraw = false);
    QVector3D getCoords(QVector3D prevCoord, int direction);
};

#endif // CUBENET_H

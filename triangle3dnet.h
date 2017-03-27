#ifndef TRIANGLE3DNET_H
#define TRIANGLE3DNET_H

#include "trianglenet.h"
#define SQRT3DIV2 0.86602540378
#define SQRT3DIV6 0.28867513459
#define SQRT33DIV6 0.95742710775
class Triangle3DNet : public Net
{
    Q_OBJECT
public:
    explicit Triangle3DNet();
    QVector3D getDirectionCoord(int direction, QVector3D c,bool isDraw = false);
    QVector3D getCoords(QVector3D mCoord, int direction);
};

#endif // TRIANGLE3DNET_H

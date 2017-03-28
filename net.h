#ifndef NET_H
#define NET_H
#include "settings.h"
#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QtCore/QObject>
#include <QDebug>

class Net:public QObject
{
     Q_OBJECT
protected:
    int minTurn;
    int maxTurn;
    int countDirection;
public:
    int turnToDirection(int currentDirection,int turn);
    virtual QVector3D getDirectionCoord(int direction, QVector3D c, bool isDraw = false) = 0;
    virtual QVector3D getCoords(QVector3D mCoord) = 0;
    int getMinTurn();
    int getMaxTurn();
    explicit Net();
};

#endif // NET_H

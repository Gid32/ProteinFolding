#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QMap>
#include <QTime>
#include "netfactory.h"
#include "settings.h"

struct History
{
    QVector3D coords;
    int direction;
    int turn;
    History(QVector3D coords,int direction,int turn)
    {
        this->coords = coords;
        this->direction = direction;
        this->turn = turn;
    }
    History(){}
};

struct Convolution
{
    QMap<QString,BYTE> area_;
    QVector<History> history_;

    QString coordToQString(QVector3D coord);
    void setValueByCoord(QVector3D coord, BYTE value);
    BYTE getValueByCoord(QVector3D coord);
    bool isHydroFobByCoord(QVector3D coord);
};

#endif // CONVOLUTION_H

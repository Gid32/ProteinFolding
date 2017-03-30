#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include "netfactory.h"
#include "settings.h"

struct History
{
    QVector3D coords;
    int direction;
    History(QVector3D coords,int direction)
    {
        this->coords = coords;
        this->direction = direction;
    }
    History(){}
};

class Convolution : public QObject
{
    Q_OBJECT
private:
    BYTE area[AREA_SIZE][AREA_SIZE][AREA_SIZE];
    QVector3D currentCoords_;
    QVector<History> history_;
    int currentDirection_;
    void setValueByCoord(QVector3D coord, BYTE value);
    BYTE getValueByCoord(QVector3D coord);
    bool getNext(QVector3D &coords, int &direction);
    QVector3D getCoordsRelationZeroPosition(QVector3D coords);
    bool isHydroFobByCoord(QVector3D coord);
    int getCount(QVector3D coord,QVector3D blockCoordPrev,QVector3D blockCoordNext);
    void clearArea();
public:
    static Net* net;
    static QVector<BYTE> protein;
    explicit Convolution(Convolution* prevConvolution, QObject *parent = 0);
    int getResult();
    void create();
    QVector<QVector3D> getVectorCoords();
signals:

public slots:
};

#endif // CONVOLUTION_H

#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QVector>
#include "settings.h"

#include "netfactory.h"
#include <QtCore/QObject>

struct History
{
    QVector3D coords;
    int turn;
    int direction;
    History(QVector3D coords,int direction)
    {
        this->coords = coords;
        this->direction = direction;
    }
    History(){}

};

class Core:public QObject
{
  Q_OBJECT
signals:
    void hasBetterVariant(QVector<QVector3D> vectorCoords);
    void proteinLoaded(VECTORBYTE protein);
private:
    Net *net;
    BYTE area[AREA_SIZE][AREA_SIZE][AREA_SIZE];
    QVector<BYTE> protein_;
    QVector3D currentCoords_;
    int currentDirection_;
    int bestResult_;
    QVector<History> history_;
    bool isProteinLoaded_;
    bool isBreak_;
    int getElementNumByCoords(QVector3D coord);
    int getResult();
    void createConvolution();
    int getCount(QVector3D coord, QVector3D blockCoordPrev, QVector3D blockCoordNext);
    bool isHydroFobByCoord(QVector3D coord);
    void setValueByCoord(QVector3D coord, BYTE value);
    BYTE getValueByCoord(QVector3D coord);
    bool getNextCoordsByCurrentDirection(QVector3D &coords);
    QVector3D getCoordsRelationZeroPosition(QVector3D coords);
    void debugCoord(QVector3D coord);
    void debugHistoryCoord();
    void init();
    void loadProtein();
public:
    explicit Core();
    void setNet(QString netName);
public slots:
    void start();
    void stop();
};

#endif // CORE_H

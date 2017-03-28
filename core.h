#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QVector>
#include "settings.h"
#include "trianglenet.h"
#include "quadnet.h"
#include "cubenet.h"
#include "triangle3dnet.h"
#include <QtCore/QObject>

struct History
{
    QVector3D coord;
    int turn;
    int direction;
    History(QVector3D coord,int turn,int direction)
    {
        this->coord = coord;
        this->turn = turn;
        this->direction = direction;
    }
    History(){}

};

class Core:public QObject
{
  Q_OBJECT
signals:
    void hasBetterVariant(QVector<QVector3D> vectorCoords);
    void proteinLoaded(QVector<bool> protein);
private:
    Net *net;
    int area[COUNT * 2 - 1][COUNT * 2 - 1][COUNT * 2 - 1];
    QVector<bool> protein;
    QVector3D currentCoords;
    int currentDirection;
    int bestResult;
    bool isBreak;
    QVector<History> history;
    int getResult();
    QVector<int> canTurn();
    void createConvolution();
    int getCount(QVector3D coord, QVector3D blockCoordPrev, QVector3D blockCoordNext);
    bool isHydroFobByCoord(QVector3D coord);
    void debugCoord(QVector3D coord);
public:
    explicit Core();
    void init();
public slots:
    void start();
};

#endif // CORE_H

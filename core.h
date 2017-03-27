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
    QVector<int> vectorTurn;
    QVector<bool> protein;
    QVector3D currentCoords;
    int currentDirection;
    QVector<int> createConvolution();
    int bestResult;
    int blockTurn;
    int getResult();
    bool isBreak;
    QVector<int> canTurn();
    QVector<History> history;
public:
    explicit Core();
    QVector<int> getvectorDirection(QVector<int> vectorTurn);
    QVector<QVector3D> getVectorCoords(QVector<int> vectorDirection);
    QVector<QVector3D> getVectorMCoords(QVector<int> vectorDirection);
    void init();
public slots:
    void start();
};

#endif // CORE_H

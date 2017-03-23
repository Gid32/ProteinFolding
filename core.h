#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QVector>
#include "settings.h"
#include "trianglenet.h"
#include <QtCore/QObject>

struct History
{
    Coord coord;
    int turn;
    int direction;
    History(Coord coord,int turn,int direction)
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
    void hasBetterVariant(QVector<int> vectorTurn);
    void proteinLoaded(QVector<bool> protein);
private:
    Net *net;
    int area[COUNT * 2 - 1][COUNT * 2 - 1];
    QVector<int> vectorTurn;
    QVector<bool> protein;
    Coord currentCoords;
    int currentDirection;
    //Coord getDirectionCoord(int direction, Coord c);
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
    //int turnToDirection(int currentDirection,int turn);
    void init();
public slots:
    void start();
};

#endif // CORE_H

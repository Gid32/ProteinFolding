#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QMap>
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
    QMap<QString,BYTE> area;
    QVector3D currentCoords_;
    QVector<History> history_;
    int currentDirection_;
    int currentTurnNumber_;
    void setValueByCoord(QVector3D coord, BYTE value);
    BYTE getValueByCoord(QVector3D coord);
    bool getNext(QVector3D &coords, int &direction);
    QVector3D randomMethod(QVector<QVector3D> possible);
    QVector3D withoutProbabilistic(QVector<QVector3D> possible);
    QVector3D probabilistic(QVector<QVector3D> possible);
    QVector3D getCoordsRelationZeroPosition(QVector3D coords);
    bool isHydroFobByCoord(QVector3D coord);
    int getCount(QVector3D coord,QVector3D blockCoordPrev,QVector3D blockCoordNext);
    QString coordToQString(QVector3D coord);
    void create();
public:
    static Net* net;
    static QVector<BYTE> protein;
    static int method;
    explicit Convolution(QObject *parent = 0);
    explicit Convolution(Convolution *convolution, int turnNumber);
    ~Convolution();
    int getResult();
    QVector<QVector3D> getVectorCoords();
    QVector<History> getHistory();
signals:

public slots:
};

#endif // CONVOLUTION_H

#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QVector>
#include <QTime>
#include "settings.h"

#include "netfactory.h"
#include <QtCore/QObject>

#include "convolution.h"
#include "convolutionfactory.h"
#include "ant.h"

class Core:public QObject
{
  Q_OBJECT
signals:
    void hasBetterVariant(QVector<QVector3D> vectorCoords,int,QString time);
    void proteinLoaded(VECTORBYTE protein);
    void countConvolution(int);
    void stopped();
private:
    QTime startTime_;
    int bestResult_;
    bool isBreak_;
    QVector<Ant*> ants_;
    QVector<Convolution*> allConvolutions_;
    QVector<Convolution*> tempConvolutions_;
    int countAntsReady_;
    void runAnts();
    void deleteAnts();
    void createAnts(int count);
    void clearVectorConvolution(QVector<Convolution *> *vect);
public:
    explicit Core();
public slots:
    void start(int count);
    void stop();
    void antFinish();
    void getConvolution(Convolution *);
};

#endif // CORE_H

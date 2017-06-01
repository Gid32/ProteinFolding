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
    void hasBetterVariant(Convolution, QString);
    void hasVariant(Convolution, QString);
    void proteinLoaded(VECTORBYTE protein);
    void countConvolution(int,int);
    void changeStatus(int);
    void factoryStopped();
private:
    Convolution* stepBestConvolution_;
    QTime startTime_;
    QTime timeWithoutBetter_;
    int bestResult_;
    bool isBreak_;
    int exit_;
    QTime timeExit_;
    int countExit_;
    int countWithoutBetter_;
    QVector<Ant*> ants_;
    int allConvolutions_;
    int allResult_;
    QVector<Convolution*> tempConvolutions_;
    int countAntsReady_;
    void runAnts();
    void deleteAnts();
    void createAnts(int countAnt, int countThreads);
    void clearVectorConvolution(QVector<Convolution *> *vect);
    bool isExit();
    void performLocalSearch();
public:
    explicit Core();
public slots:
    void start(SETTINGS settings);
    void stop();
    void antFinish();
    void getConvolution(Convolution *);
};

#endif // CORE_H

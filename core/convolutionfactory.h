#ifndef CONVOLUTIONFACTORY_H
#define CONVOLUTIONFACTORY_H

#include <QObject>
#include <QTime>
#include "settings.h"
#include "netfactory.h"
#include "convolution.h"

class ConvolutionFactory : public QObject
{
    Q_OBJECT
private:
    Net *net_;
    int method_;
    int count_;
    double traceMin_;
    double traceMax_;
    double traceEvaporation_;
    int traceM_;
    int traceN_;
    double traceCoef_;
    double weights_[2][4];
//    double weightsFOBFOB_;
//    double weightsFOBFIL_;
//    double weightsFOBFREE_;
//    double weightsFILFOB_;
//    double weightsFILFIL_;
//    double weightsFILFREE_;
    double **trace_;

    VECTORBYTE protein_;
    bool isStarted_;

    bool getNext(Convolution *convolution, int currentDirection, QVector3D currentCoords, QVector3D &coords, int &direction,int &turn);
    int probabilistic(Convolution *convolution, QVector<QVector3D> possible, QVector<int> turns, int currentDirection);
    int getCount(Convolution *convolution,QVector3D coord, QVector3D blockCoordPrev, QVector3D blockCoordNext,int currentDirection);
    double getWeights(Convolution *convolution, QVector3D coord, BYTE currentAm,int currentDirection);
    static ConvolutionFactory* instance;

    explicit ConvolutionFactory(QObject *parent = 0);
    void clearTrace();
    void createTrace();
public:
    static ConvolutionFactory* getFactory();
    Convolution *getConvolution();
    int getResult(Convolution *convolution);
    QVector<QVector3D> getVectorCoords(Convolution *convolution);
    void changeTrace(QVector<Convolution*> convolutions);
signals:
    void ready(SETTINGS settings);
    void createdProtein(VECTORBYTE);
    void error(QString);
public slots:
    void createProtein(int count);
    void setSettings(SETTINGS settings);
    void stop();
};

#endif // CONVOLUTIONFACTORY_H

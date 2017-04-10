#include "ant.h"
#include <QTime>
Ant::Ant()
{
}

void Ant::setConvolution(Convolution *convolution)
{
    convolution_ = convolution;
}

void Ant::run()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    int i = qrand()%(Convolution::count-1);
    Convolution *convolution = new Convolution(convolution_,i);
    emit convolutionCreated(convolution);
}

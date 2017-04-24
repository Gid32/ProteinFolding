#include "ant.h"
#include <QTime>
Ant::Ant()
{
}

void Ant::run()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    Convolution *convolution = ConvolutionFactory::getFactory()->getConvolution();
    emit convolutionCreated(convolution);
}

#include "ant.h"
#include <QTime>
Ant::Ant()
{
}

void Ant::setCount(int count)
{
    count_ = count;
}

void Ant::run()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    for (int i = 0; i < count_; ++i)
    {
        Convolution *convolution = ConvolutionFactory::getFactory()->getConvolution();
        emit convolutionCreated(convolution);
    }
}

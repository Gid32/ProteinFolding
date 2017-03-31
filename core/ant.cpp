#include "ant.h"

Ant::Ant()
{

}

void Ant::setConvolution(Convolution *convolution)
{
    convolution_ = convolution;
}

void Ant::run()
{
     Convolution *conv = new Convolution();
     emit convolutionCreated(conv);
}

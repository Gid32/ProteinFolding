#include "ant.h"
#include <ctime>
Ant::Ant()
{

}

void Ant::setConvolution(Convolution *convolution)
{
    convolution_ = convolution;
}

void Ant::run()
{
    srand(time(NULL));
     Convolution *conv = new Convolution();
     emit convolutionCreated(conv);
    //emit test(rand()%n);
}

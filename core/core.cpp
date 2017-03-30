#include "core.h"
#include <ctime>
#include <windows.h>
#include <QtCore/QDebug>
#include <QTimer>
using namespace std;

Core::Core()
{
    isProteinLoaded_ = false;
    isBreak_ = false;
}

void Core::loadProtein()
{
    srand(time(NULL));
    for(int i=0;i<COUNT;i++)
    {
        if(rand()%2)
            Convolution::protein.push_back(H_FOB);
        else
            Convolution::protein.push_back(H_FILL);
    }
    isProteinLoaded_ = true;
    emit proteinLoaded(Convolution::protein);
}

void Core::setNet(QString netName)
{
   Convolution::net = NetFactory::getInstance()->getNetByName(netName);
}

void Core::start()
{
    if(!isProteinLoaded_)
        loadProtein();
    isBreak_ = false;
    bestResult_ = 0;

    Convolution *conv;
    while(!isBreak_)
    {
        conv = new Convolution(nullptr);
        conv->create();
        int result = conv->getResult();
        if(result > bestResult_)
        {
            bestResult_ = result;
            qDebug()<<bestResult_;
            QVector<QVector3D> vectorCoords = conv->getVectorCoords();
            emit hasBetterVariant(vectorCoords);
        }
        delete conv;
    }
}

void Core::stop()
{
    isBreak_ = true;
}

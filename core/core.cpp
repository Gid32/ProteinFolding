#include "core.h"
#include <windows.h>
#include <QtCore/QDebug>
#include <QTime>
#include <QTimer>
using namespace std;

Core::Core()
{
    isProteinLoaded_ = false;
    isBreak_ = false;
}

void Core::loadProtein()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    for(int i=0;i<COUNT;i++)
    {
        if(qrand()%2)
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

void Core::createStartConvolutions()
{
    int method = Convolution::method;
    Convolution::method = 0;//random
    for(int i=0;i<ants_.size();i++)
        currentConvolutions_.push_back(new Convolution());
    Convolution::method = method;
}

void Core::start()
{
    if(!isProteinLoaded_)
        loadProtein();
    isBreak_ = false;
    bestResult_ = 0;
    createStartConvolutions();
    runAnts();
}

void Core::deleteAnts()
{
    for(int i=0;i<ants_.size();i++)
        delete ants_[i];
    ants_.clear();
}

void Core::createAnts(int count)
{
    deleteAnts();
    for(int i=0;i<count;i++)
    {
        Ant *ant = new Ant();
        connect(ant,SIGNAL(finished()),this,SLOT(antFinish()));
        connect(ant,SIGNAL(convolutionCreated(Convolution*)),this,SLOT(getConvolution(Convolution*)));
        ants_.push_back(ant);
    }
}

void Core::runAnts()
{
     countAntsReady_ = 0;
     for(int i=0;i<ants_.size();i++)
     {
         QVector<QVector3D> vectorCoords = currentConvolutions_[i]->getVectorCoords();
         emit hasBetterVariant(vectorCoords,bestResult_);
         ants_[i]->setConvolution(currentConvolutions_[i]);
         ants_[i]->start();
     }
}

void Core::antFinish()
{
    countAntsReady_++;
    if(countAntsReady_ == ants_.size())
    {
        //qDebug()<<"ants finished";
        runAnts();
    }
}

void Core::getConvolution(Convolution *convolution)
{
    int result = convolution->getResult();
    allConvolutions_.push_back(convolution);
    if(result > bestResult_)
    {
        bestResult_ = result;
        qDebug()<<bestResult_;
        QVector<QVector3D> vectorCoords = convolution->getVectorCoords();
        emit hasBetterVariant(vectorCoords,bestResult_);
    }
    delete convolution;
    emit countConvolution(allConvolutions_.size());
}

void Core::stop()
{
    isBreak_ = true;
}

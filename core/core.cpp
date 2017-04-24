#include "core.h"
#include <windows.h>
#include <QtCore/QDebug>
#include <QTime>
#include <QTimer>
using namespace std;

Core::Core()
{
}

void Core::clearVectorConvolution(QVector<Convolution*> *vect)
{
//    for(int i=0;i<vect->size();i++)
//        delete vect->at(i);
    vect->clear();
}


void Core::start(SETTINGS settings)
{
    int countAnt = settings.value("antsCount").toInt();
    int countThread = settings.value("threadsCount").toInt();
    createAnts(countAnt,countThread);
    isBreak_ = false;
    bestResult_ = 0;
    startTime_.start();
    runAnts();
}

void Core::deleteAnts()
{
    for(int i=0;i<ants_.size();i++)
        delete ants_[i];
    ants_.clear();
}

void Core::createAnts(int countAnt, int countThreads)
{
    deleteAnts();
    int countConvInAnt = countAnt/countThreads;
    for(int i=0;i<countThreads;i++)
    {
        Ant *ant = new Ant();
        if(i==0)
            ant->setCount(countConvInAnt+(countAnt%countThreads));
        ant->setCount(countConvInAnt);
        connect(ant,SIGNAL(finished()),this,SLOT(antFinish()));
        connect(ant,SIGNAL(convolutionCreated(Convolution*)),this,SLOT(getConvolution(Convolution*)));
        connect(this,SIGNAL(stopped()),ant,SLOT(quit()));
        ants_.push_back(ant);
    }
}

void Core::runAnts()
{
     countAntsReady_ = 0;
     for(int i=0;i<ants_.size();i++)
         ants_[i]->start();
}

void Core::antFinish()
{
    countAntsReady_++;
    if(countAntsReady_ == ants_.size())
    {
        ConvolutionFactory::getFactory()->changeTrace(tempConvolutions_);
        clearVectorConvolution(&tempConvolutions_);
        if(!isBreak_)
            runAnts();
    }
}

void Core::getConvolution(Convolution *convolution)
{
    int result = ConvolutionFactory::getFactory()->getResult(convolution);
    allConvolutions_.push_back(convolution);
    tempConvolutions_.push_back(convolution);
    if(result > bestResult_)
    {
        bestResult_ = result;
        QVector<QVector3D> vectorCoords = ConvolutionFactory::getFactory()->getVectorCoords(convolution);
        QTime time = QTime(0,0,0,0).addMSecs(startTime_.elapsed());
        QString timeStr = time.toString("hh:mm:ss.zzz");
        emit hasBetterVariant(vectorCoords,bestResult_,timeStr);
    }
    emit countConvolution(allConvolutions_.size());
}

void Core::stop()
{
    isBreak_ = true;
    emit stopped();
}

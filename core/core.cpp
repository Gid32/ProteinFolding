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
    for(int i=0;i<vect->size();i++)
        delete vect->at(i);
    vect->clear();
}


void Core::start(SETTINGS settings)
{
    int countAnt = settings.value("antsCount").toInt();
    int countThread = settings.value("threadsCount").toInt();
    allConvolutions_ = 0;
    allResult_ = 0;
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
        else
            ant->setCount(countConvInAnt);
        connect(ant,SIGNAL(finished()),this,SLOT(antFinish()));
        connect(ant,SIGNAL(convolutionCreated(Convolution*)),this,SLOT(getConvolution(Convolution*)));
        connect(this,SIGNAL(stopped()),ant,SLOT(terminate()));
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
        emit countConvolution(allConvolutions_, allResult_);
    }
}

void Core::getConvolution(Convolution *convolution)
{
    int result = ConvolutionFactory::getFactory()->getResult(convolution);
    convolution->result_ = result;
    allConvolutions_++;
    allResult_ += result;
    tempConvolutions_.push_back(convolution);
    if(result > bestResult_)
    {
        bestResult_ = result;
        QTime time = QTime(0,0,0,0).addMSecs(startTime_.elapsed());
        QString timeStr = time.toString("hh:mm:ss.zzz");
        emit hasBetterVariant(*convolution,timeStr);
    }

    emit hasVariant(*convolution);
}

void Core::stop()
{
    isBreak_ = true;
    emit stopped();
}

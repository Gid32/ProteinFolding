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
    emit changeStatus(1);
    exit_ = settings.value("exit").toInt();
    timeExit_ = settings.value("timeExit").toTime();
    countExit_ = settings.value("countExit").toInt();
    int countAnt = settings.value("antsCount").toInt();
    int countThread = settings.value("threadsCount").toInt();
    allConvolutions_ = 0;
    allResult_ = 0;
    createAnts(countAnt,countThread);
    isBreak_ = false;
    bestResult_ = 0;
    startTime_.start();
    timeWithoutBetter_.start();
    countWithoutBetter_ = 0;
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
        //connect(this,SIGNAL(stopped()),ant,SLOT(terminate()));
        ants_.push_back(ant);
    }
}

void Core::runAnts()
{
     countAntsReady_ = 0;
     for(int i=0;i<ants_.size();i++)
         ants_[i]->start();
}

bool Core::isExit()
{
    if(exit_==0)
    {
        QTime time = QTime(0,0,0,0).addMSecs(startTime_.elapsed());
        if(timeExit_<=time)
            return true;
    }
    else if(exit_==1)
    {
        QTime time = QTime(0,0,0,0).addMSecs(timeWithoutBetter_.elapsed());
        if(timeExit_<=time)
            return true;
    }
    else
    {
        if(countWithoutBetter_>countExit_)
            return true;
    }
    return false;
}

void Core::antFinish()
{
    countAntsReady_++;
    if(countAntsReady_ == ants_.size())
    {
        ConvolutionFactory::getFactory()->changeTrace(tempConvolutions_);
        clearVectorConvolution(&tempConvolutions_);
        bool done = isExit();
        if(done)
            stop();
        countWithoutBetter_++;
        if(!isBreak_)
            runAnts();
        else
        {
            if(done)
                emit changeStatus(10);
            else
                emit changeStatus(0);
        }

    }
}

void Core::getConvolution(Convolution *convolution)
{
    allConvolutions_++;
    allResult_ += convolution->result_;
    tempConvolutions_.push_back(convolution);
    if(convolution->result_ > bestResult_)
    {
        bestResult_ = convolution->result_;
        QTime time = QTime(0,0,0,0).addMSecs(startTime_.elapsed());
        QString timeStr = time.toString("hh:mm:ss.zzz");
        timeWithoutBetter_.start();
        countWithoutBetter_ = 0;
        emit hasBetterVariant(*convolution,timeStr);
        return;
    }
    emit countConvolution(allConvolutions_, allResult_);
    emit hasVariant(*convolution);
}

void Core::stop()
{
    isBreak_ = true;
    emit factoryStopped();
}

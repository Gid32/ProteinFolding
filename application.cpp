#include "application.h"

Application::Application(QObject *parent) : QObject(parent)
{
    //init core and scene
    core_ = new Core();
    scene_ = new Scene("Protein Folding",800,600);

    qRegisterMetaType<QVector<QVector3D>>();
    qRegisterMetaType<VECTORBYTE>("VECTORBYTE");
    QObject::connect(core_, SIGNAL(hasBetterVariant(QVector<QVector3D>,int,QString)), scene_, SLOT(update(QVector<QVector3D>,int,QString)));
    QObject::connect(core_, SIGNAL(proteinLoaded(VECTORBYTE)), scene_, SLOT(genericNodes(VECTORBYTE)));
    QObject::connect(core_, SIGNAL(countConvolution(int)), scene_, SLOT(countConvolution(int)));

    coreThread_ = new QThread;
    core_->moveToThread(coreThread_);

    QObject::connect(scene_,SIGNAL(started(QString,int,int)),this,SLOT(coreStart(QString,int,int)));
    QObject::connect(scene_,SIGNAL(stopped()),this,SLOT(coreStop()));

    QObject::connect(coreThread_,SIGNAL(started()),core_,SLOT(start()));

    isStart = false;

}

void Application::coreStart(QString netName,int method, int countAnt)
{
    if(isStart)
        return;
    isStart = true;
    core_->setSettings(netName,method,countAnt);
    coreThread_->start();
}

void Application::coreStop()
{
    core_->stop();
    coreThread_->terminate();
    isStart = false;
}



void Application::start()
{
    scene_->show();
}



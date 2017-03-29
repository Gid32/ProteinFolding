#include "application.h"

Application::Application(QObject *parent) : QObject(parent)
{
    //init core and scene
    core_ = new Core();
    scene_ = new Scene("Protein Folding",1200,800);

    qRegisterMetaType<QVector<QVector3D>>();
    qRegisterMetaType<VECTORBYTE>("VECTORBYTE");
    QObject::connect(core_, SIGNAL(hasBetterVariant(QVector<QVector3D>)), scene_, SLOT(update(QVector<QVector3D>)));
    QObject::connect(core_, SIGNAL(proteinLoaded(VECTORBYTE)), scene_, SLOT(genericNodes(VECTORBYTE)));

    coreThread_ = new QThread;
    core_->moveToThread(coreThread_);

    QObject::connect(scene_,SIGNAL(started(QString)),this,SLOT(coreStart(QString)));
    QObject::connect(scene_,SIGNAL(stopped()),this,SLOT(coreStop()));

    QObject::connect(coreThread_,SIGNAL(started()),core_,SLOT(start()));

    //    coreThread_->start();
}

void Application::coreStart(QString netName)
{
    core_->setNet(netName);
    coreThread_->start();
}

void Application::coreStop()
{
    coreThread_->terminate();
}



void Application::start()
{
    scene_->show();
}



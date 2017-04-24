#include "application.h"

Application::Application(QObject *parent) : QObject(parent)
{
    //init core and scene
    core_ = new Core();
    scene_ = new Scene("Protein Folding",800,600);

    qRegisterMetaType<QVector<QVector3D>>();
    qRegisterMetaType<VECTORBYTE>("VECTORBYTE");
    qRegisterMetaType<SETTINGS>("SETTINGS");
    qRegisterMetaType<QString>();

    connect(core_, SIGNAL(hasBetterVariant(QVector<QVector3D>,int,QString)), scene_, SLOT(update(QVector<QVector3D>,int,QString)));
    connect(core_, SIGNAL(hasVariant(QVector<QVector3D>)), scene_, SLOT(hasVariant(QVector<QVector3D>)));
    connect(core_, SIGNAL(countConvolution(int)), scene_, SLOT(countConvolution(int)));

    ConvolutionFactory *factory = ConvolutionFactory::getFactory();
    connect(scene_,SIGNAL(createdProtein(int)),factory,SLOT(createProtein(int)));
    connect(factory, SIGNAL(createdProtein(VECTORBYTE)), scene_, SLOT(genericNodes(VECTORBYTE)));

    connect(scene_,SIGNAL(started(SETTINGS)),factory,SLOT(setSettings(SETTINGS)));
    connect(factory,SIGNAL(ready(SETTINGS)),core_,SLOT(start(SETTINGS)));
    connect(scene_,SIGNAL(stopped()),factory,SLOT(stop()));
    connect(scene_,SIGNAL(stopped()),core_,SLOT(stop()));

    connect(factory,SIGNAL(error(QString)),scene_,SLOT(getError(QString)));

}

void Application::start()
{
    scene_->show();
}



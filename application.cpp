#include "application.h"

Application::Application(QObject *parent) : QObject(parent)
{
    //init core and scene
    core_ = new Core();
    mainWindow_ = new MainWindow;

    qRegisterMetaType<QVector<QVector3D>>();
    qRegisterMetaType<VECTORBYTE>("VECTORBYTE");
    qRegisterMetaType<SETTINGS>("SETTINGS");
    qRegisterMetaType<QString>();

    connect(core_, SIGNAL(hasBetterVariant(Convolution,QString)), mainWindow_, SLOT(hasBetterVariant(Convolution,QString)));
    connect(core_, SIGNAL(hasVariant(Convolution)), mainWindow_, SLOT(hasVariant(Convolution)));
    connect(core_, SIGNAL(countConvolution(int,int)), mainWindow_, SLOT(countConvolution(int,int)));

    ConvolutionFactory *factory = ConvolutionFactory::getFactory();
    connect(mainWindow_->getSettingsForm(),SIGNAL(createdProtein(int,double)),factory,SLOT(createProtein(int,double)));
    connect(mainWindow_->getSettingsForm(),SIGNAL(loadedProtein(QByteArray)),factory,SLOT(loadProtein(QByteArray)));
    connect(factory, SIGNAL(createdProtein(VECTORBYTE)), mainWindow_,SLOT(createProtein(VECTORBYTE)));
    connect(factory, SIGNAL(createdProtein(VECTORBYTE)), mainWindow_->getSettingsForm(),SLOT(getProtein(VECTORBYTE)));

    connect(mainWindow_,SIGNAL(started(SETTINGS)),factory,SLOT(setSettings(SETTINGS)));
    connect(factory,SIGNAL(ready(SETTINGS)),core_,SLOT(start(SETTINGS)));
    connect(mainWindow_,SIGNAL(stopped()),factory,SLOT(stop()));
    connect(mainWindow_,SIGNAL(stopped()),core_,SLOT(stop()));

    connect(factory,SIGNAL(error(QString)),mainWindow_,SLOT(getError(QString)));

}

void Application::start()
{
    mainWindow_->show();
}



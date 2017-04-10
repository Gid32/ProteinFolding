#include "Scene.h"
#include <QtCore/QDebug>
#include <QTimer>
#include <windows.h>

void Scene::clear()
{
    for(int i=0;i<nodes.size();i++)
        delete nodes.at(i);
    nodes.clear();
}

void Scene::test()
{
    QVector3D coords(0.0,0.0,0.0);
    for(int i=0;i<nodes.size();i++)
        nodes.at(i)->changeLocation(coords);
}

void Scene::genericNodes(VECTORBYTE protein)
{
    clear();
    Node *node = nullptr;
    for(int i=0;i<protein.size();i++)
    {
        nodes.push_back(new Node(rootEntity_,protein.at(i),QVector3D(0, 0, 0),node));
        node = nodes.last();
    }
}

void Scene::reDraw()
{
    if(!hasVariantToUpdate_)
        return;
    hasVariantToUpdate_ = false;

    for(int i=1;i<nodes.size();i++)//0 нод не трогаем
         nodes.at(i)->changeLocation(vectorCoords.at(i-1));
}

Scene::Scene(const char* title, int width, int height)
{
    title_ = title;
    width_ = width;
    height_ = height;
    view_ = new Qt3DExtras::Qt3DWindow();
    rootEntity_ = new Qt3DCore::QEntity();
    view_->setRootEntity(rootEntity_);

    widget_ = new QWidget;
    widget_->setWindowTitle(title_);
    widget_->setStyleSheet("QLabel {color:#1E90FF;font-size:15px;}"
                           "QLabel#settingsLabel {color:#DC143C;font-size:20px;margin:5px}"
                           "QLabel#settingsRate,QLabel#settingsCountConvolution,QLabel#settingsTimeBest {color:#DC143C}"
                           "QPushButton {color:white;background-color:green;font-size:15px}"
                           "QPushButton#settingsStopApplication {background-color:#DC143C}"
                           );

    mainLayout_ = new QHBoxLayout(widget_);
    mainLayout_->setMargin(0);

    initCamera();
    initLight();
    setColor(QColor(255,255,255));

    initMainContainer();
    initSettingsLayout();

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view_->registerAspect(input);


    hasVariantToUpdate_ = false;

    timerUpdate_ = new QTimer(this);
    QObject::connect(timerUpdate_, SIGNAL(timeout()), this, SLOT(reDraw()));

}

void Scene::update(QVector<QVector3D> vectorCoords, int value, QString time)
{
    this->vectorCoords = vectorCoords;
    settingsRate_->setText(QString::number(value));
    settingsTimeBest_->setText(time);
    hasVariantToUpdate_ = true;
}

void Scene::countConvolution(int count)
{
    settingsCountConvolution_->setText(QString::number(count));
}

void Scene::initMainContainer()
{
    mainContainer_ = QWidget::createWindowContainer(view_);
    QSize screenSize = view_->screen()->size();
    mainContainer_->setMinimumSize(QSize(500, 500));
    mainContainer_->setMaximumSize(screenSize);

    mainLayout_->addWidget(mainContainer_,1);
}

void Scene::initSettingsLayout()
{
    settingsLayout_ = new QVBoxLayout();
    settingsLayout_->setAlignment(Qt::AlignTop);

    mainLayout_->addLayout(settingsLayout_);

    initSettingsLabel();
    initSettingsGenericConvolution();
    initSettingsNetChanges();
    initSettingsSelectMethod();
    initSettingsCountAnt();
    initSettingsStartStopApplication();
    initSettingsRate();
    initSettingsTimeBest();
    initSettingsCountConvolution();

}

void Scene::initSettingsLabel()
{
    settingsLabel_ = new QLabel(widget_);
    settingsLabel_->setText("Настройки");
    settingsLabel_->setMinimumWidth(150);
    settingsLabel_->setObjectName("settingsLabel");

    settingsLayout_->addWidget(settingsLabel_);
}

void Scene::initSettingsGenericConvolution()
{
    settingsGenericLabel_ = new QLabel(widget_);
    settingsGenericLabel_->setText("Генерация свертки:");
    settingsGenericLabel_->setMinimumWidth(150);

    settingsGenericCount_ = new QSpinBox(widget_);
    settingsGenericCount_->setMaximumWidth(100);

    settingsGenericStart_ = new QPushButton("Сгенерировать",widget_);
    settingsGenericStart_->setMaximumWidth(120);

    settingsLayout_->addWidget(settingsGenericLabel_);
    settingsLayout_->addWidget(settingsGenericCount_);
    settingsLayout_->addWidget(settingsGenericStart_);

    connect(settingsGenericStart_,SIGNAL(clicked()),this,SLOT(createProtein()));
}

void Scene::initSettingsNetChanges()
{
    settingsNetChangesLabel_ = new QLabel(widget_);
    settingsNetChangesLabel_->setText("Выбрать сетку:");
    settingsNetChangesLabel_->setObjectName("settingsNetChangesLabel");

    settingsLayout_->addWidget(settingsNetChangesLabel_);

    settingsNetChanges_ = new QComboBox(widget_);
    settingsNetChanges_->addItems(NetFactory::getInstance()->getStringList());
    settingsNetChanges_->setCurrentIndex(1);
    settingsNetChanges_->setMaximumSize(100,50);

    settingsLayout_->addWidget(settingsNetChanges_);
}

void Scene::initSettingsSelectMethod()
{
    settingsSelectMethodLabel_ = new QLabel(widget_);
    settingsSelectMethodLabel_->setText("Выбрать метод:");
    settingsSelectMethodLabel_->setObjectName("settingsSelectMethodLabel");

    settingsLayout_->addWidget(settingsSelectMethodLabel_);

    settingsSelectMethod_ = new QComboBox(widget_);
    settingsSelectMethod_->addItem("рэндом",0);
    settingsSelectMethod_->addItem("безвероятностный",1);
    settingsSelectMethod_->addItem("вероятностный",2);
    settingsSelectMethod_->setCurrentIndex(2);
    settingsSelectMethod_->setMaximumSize(120,50);

    settingsLayout_->addWidget(settingsSelectMethod_);
}

void Scene::initSettingsCountAnt()
{
    settingsSelectCountAntLabel_ = new QLabel(widget_);
    settingsSelectCountAntLabel_->setText("Количество муравьев:");
    settingsSelectCountAntLabel_->setObjectName("settingsSelectCountAntLabel");

    settingsLayout_->addWidget(settingsSelectCountAntLabel_);

    settingsSelectCountAnt_ = new QSpinBox(widget_);
    settingsSelectCountAnt_->setMaximumSize(100,50);

    settingsLayout_->addWidget(settingsSelectCountAnt_);
}

void Scene::initSettingsStartStopApplication()
{
    settingsStartApplication_ = new QPushButton("Старт",widget_);
    settingsStartApplication_->setMaximumWidth(100);
    settingsLayout_->addWidget(settingsStartApplication_);

    settingsStopApplication_ = new QPushButton("Стоп",widget_);
    settingsStopApplication_->setMaximumWidth(100);
    settingsStopApplication_->setObjectName("settingsStopApplication");
    settingsLayout_->addWidget(settingsStopApplication_);


    connect(settingsStartApplication_,SIGNAL(clicked(bool)),this,SLOT(start()));
    connect(settingsStopApplication_,SIGNAL(clicked(bool)),this,SLOT(stop()));

}

void Scene::initSettingsRate()
{
    settingsRateLabel_ = new QLabel(widget_);
    settingsRateLabel_->setText("Рейтинг:");
    settingsRateLabel_->setMinimumWidth(150);

    settingsRate_ = new QLabel(widget_);
    settingsRate_->setText("0");
    settingsRate_->setMinimumWidth(150);
    settingsRate_->setObjectName("settingsRate");


    settingsLayout_->addWidget(settingsRateLabel_);
    settingsLayout_->addWidget(settingsRate_);
}

void Scene::initSettingsTimeBest()
{
    settingsTimeBestLabel_ = new QLabel(widget_);
    settingsTimeBestLabel_->setText("Время:");
    settingsTimeBestLabel_->setMinimumWidth(150);

    settingsTimeBest_ = new QLabel(widget_);
    settingsTimeBest_->setText("0");
    settingsTimeBest_->setMinimumWidth(150);
    settingsTimeBest_->setObjectName("settingsTimeBest");


    settingsLayout_->addWidget(settingsTimeBestLabel_);
    settingsLayout_->addWidget(settingsTimeBest_);
}

void Scene::initSettingsCountConvolution()
{
    settingsCountConvolutionLabel_ = new QLabel(widget_);
    settingsCountConvolutionLabel_->setText("Количество сверток:");
    settingsCountConvolutionLabel_->setMinimumWidth(150);

    settingsCountConvolution_ = new QLabel(widget_);
    settingsCountConvolution_->setText("0");
    settingsCountConvolution_->setMinimumWidth(150);
    settingsCountConvolution_->setObjectName("settingsCountConvolution");


    settingsLayout_->addWidget(settingsCountConvolutionLabel_);
    settingsLayout_->addWidget(settingsCountConvolution_);
}

void Scene::setColor(QColor color)
{
    view_->defaultFrameGraph()->setClearColor(color);
}

void Scene::initLight()
{
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity_);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(3);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity_->position());
    lightEntity->addComponent(lightTransform);
}

void Scene::initCamera()
{
    cameraEntity_ = view_->camera();

    cameraEntity_->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity_->setPosition(QVector3D(0, 0, 20.0f));
    cameraEntity_->setUpVector(QVector3D(0, 1, 0));
    cameraEntity_->setViewCenter(QVector3D(0, 0, 0));

    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity_);
    camController->setCamera(cameraEntity_);
}

void Scene::start()
{
    timerUpdate_->start(1000);
    emit started(settingsNetChanges_->currentText(),settingsSelectMethod_->currentIndex(),settingsSelectCountAnt_->value());
}

void Scene::createProtein()
{
    emit createdProtein(settingsGenericCount_->value());
}

void Scene::show()
{
    widget_->show();
    widget_->resize(width_, height_);
}

void Scene::stop()
{
    timerUpdate_->stop();
    emit stopped();
    test();
}


Scene::~Scene()
{
}




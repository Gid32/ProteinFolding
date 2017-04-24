#include "Scene.h"
#include <QtCore/QDebug>
#include <QTimer>
#include <windows.h>

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

void Scene::clear()
{
    for(int i=0;i<nodes.size();i++)
        delete nodes.at(i);
    nodes.clear();
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

    styleLabel_ = "color:#1E90FF;font-size:15px;";
    styleLabelValue_ = "color:#DC143C;font-size:15px;";
    styleButtonGreen_ = "color:white;background-color:green;font-size:15px";
    styleButtonRed_ = "color:white;background-color:#DC143C;font-size:15px";

    mainLayout_ = new QHBoxLayout(widget_);
    mainLayout_->setMargin(0);

    initCamera();
    initLight();
    setColor(QColor(255,255,255));

    initMainContainer();
    initSettingsLayout();
    initResultLayout();

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view_->registerAspect(input);


    hasVariantToUpdate_ = false;

    timerUpdate_ = new QTimer(this);
    QObject::connect(timerUpdate_, SIGNAL(timeout()), this, SLOT(reDraw()));

}

void Scene::update(QVector<QVector3D> vectorCoords, int value, QString time)
{
    this->vectorCoords = vectorCoords;
    QLabel *rating = widget_->findChild<QLabel*>("rating");
    QLabel *timeBest = widget_->findChild<QLabel*>("timeBest");
    if(rating)
        rating->setText("Рейтинг: "+QString::number(value));
    if(timeBest)
        timeBest->setText("Время: "+time);
    hasVariantToUpdate_ = true;
}

void Scene::countConvolution(int count)
{
    QLabel *countConvolution = widget_->findChild<QLabel*>("countConvolution");
    if(countConvolution)
        countConvolution->setText("Количество сверток: "+QString::number(count));
}

void Scene::getError(QString error)
{
   QLabel *errorLabel = widget_->findChild<QLabel*>("error");
   errorLabel->setText(error);
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
    settingsLayout_->setMargin(5);
    settingsLayout_->setAlignment(Qt::AlignTop);

    mainLayout_->addLayout(settingsLayout_);

    //generic protein
    addSettingsLabel(settingsLayout_,"genericLabel","Генерация свертки",styleLabel_);
    addSettingsSpinBox(settingsLayout_,"genericCount",3,1000,45);
    QPushButton * genericButton = addSettingsPushButton(settingsLayout_,"genericButton","Сгенерировать",styleButtonGreen_);
    connect(genericButton,SIGNAL(clicked()),this,SLOT(createProtein()));

    //net
    addSettingsLabel(settingsLayout_,"netLabel","Выбрать сетку",styleLabel_);
    addSettingsComboBox(settingsLayout_,"netValue", NetFactory::getInstance()->getStringList(),1);

    //method
    addSettingsLabel(settingsLayout_,"methodLabel","Выбрать метод",styleLabel_);
    QString str ="рэндом,безвероятностный,вероятностный";
    addSettingsComboBox(settingsLayout_,"methodValue", str.split(","),2);

    //threads
    addSettingsLabel(settingsLayout_,"threadsLabel","Количество потоков",styleLabel_);
    addSettingsSpinBox(settingsLayout_,"threadsCount",1,MAX_ANT,3);

    //ants
    addSettingsLabel(settingsLayout_,"antsLabel","Количество муравьев",styleLabel_);
    addSettingsSpinBox(settingsLayout_,"antsCount",1,MAX_ANT,10);

    //trace
    addSettingsLabel(settingsLayout_,"traceMinLabel","След. Мин.",styleLabel_);
    addSettingsDoubleSpinBox(settingsLayout_,"traceMin",0,100,0.2);
    addSettingsLabel(settingsLayout_,"traceMaxLabel","След. Макс.",styleLabel_);
    addSettingsDoubleSpinBox(settingsLayout_,"traceMax",0,100,3);
    addSettingsLabel(settingsLayout_,"traceEvaporationLabel","След. Испарение",styleLabel_);
    addSettingsDoubleSpinBox(settingsLayout_,"traceEvaporation",0,1,0.6);
    addSettingsLabel(settingsLayout_,"traceCoefLabel","Коеф. След",styleLabel_);
    addSettingsDoubleSpinBox(settingsLayout_,"traceCoef",0,1,0.5);



}

void Scene::initResultLayout()
{
    resultLayout_ = new QVBoxLayout();
    resultLayout_->setMargin(5);
    resultLayout_->setAlignment(Qt::AlignTop);
    mainLayout_->addLayout(resultLayout_);

    //start stop
    QPushButton *start = addSettingsPushButton(resultLayout_,"startButton","Старт",styleButtonGreen_);
    QPushButton *stop = addSettingsPushButton(resultLayout_,"stopButton","Стоп",styleButtonRed_);
    connect(start,SIGNAL(clicked(bool)),this,SLOT(start()));
    connect(stop,SIGNAL(clicked(bool)),this,SLOT(stop()));

    //count Conv
    addSettingsLabel(resultLayout_,"countConvolution","Количество сверток",styleLabelValue_);

    //rating
    addSettingsLabel(resultLayout_,"rating","Рейтинг",styleLabelValue_);

    //time best
    addSettingsLabel(resultLayout_,"timeBest","Время",styleLabelValue_);

    //error
    addSettingsLabel(resultLayout_,"error","",styleLabelValue_);

    //weights
    addSettingsLabel(resultLayout_,"weights","Веса:",styleLabelValue_);
    addSettingsLabel(resultLayout_,"weightsFOBFOBLabel","FOB -> FOB",styleLabel_);
    addSettingsDoubleSpinBox(resultLayout_,"weightsFOBFOB",0,1,0.6);
    addSettingsLabel(resultLayout_,"weightsFOBFILLabel","FOB -> FIL",styleLabel_);
    addSettingsDoubleSpinBox(resultLayout_,"weightsFOBFIL",0,1,0.15);
    addSettingsLabel(resultLayout_,"weightsFOBFREEabel","FOB -> FREE",styleLabel_);
    addSettingsDoubleSpinBox(resultLayout_,"weightsFOBFREE",0,1,0.25);
    addSettingsLabel(resultLayout_,"weightsFILFOBLabel","FIL -> FOB",styleLabel_);
    addSettingsDoubleSpinBox(resultLayout_,"weightsFILFOB",0,1,0.2);
    addSettingsLabel(resultLayout_,"weightsFILFILLabel","FIL -> FIL",styleLabel_);
    addSettingsDoubleSpinBox(resultLayout_,"weightsFILFIL",0,1,0.4);
    addSettingsLabel(resultLayout_,"weightsFILFREEabel","FIL -> FREE",styleLabel_);
    addSettingsDoubleSpinBox(resultLayout_,"weightsFILFREE",0,1,0.4);
}


void Scene::start()
{
    QComboBox *net = widget_->findChild<QComboBox*>("netValue");
    QComboBox *method = widget_->findChild<QComboBox*>("methodValue");
    QSpinBox *countAnt = widget_->findChild<QSpinBox*>("antsCount");
    QSpinBox *threadsCount = widget_->findChild<QSpinBox*>("threadsCount");
    QDoubleSpinBox *traceMin = widget_->findChild<QDoubleSpinBox*>("traceMin");
    QDoubleSpinBox *traceMax = widget_->findChild<QDoubleSpinBox*>("traceMax");
    QDoubleSpinBox *traceEvaporation = widget_->findChild<QDoubleSpinBox*>("traceEvaporation");
    QDoubleSpinBox *traceCoef = widget_->findChild<QDoubleSpinBox*>("traceCoef");

    QDoubleSpinBox *weightsFOBFOB = widget_->findChild<QDoubleSpinBox*>("weightsFOBFOB");
    QDoubleSpinBox *weightsFOBFIL = widget_->findChild<QDoubleSpinBox*>("weightsFOBFIL");
    QDoubleSpinBox *weightsFOBFREE = widget_->findChild<QDoubleSpinBox*>("weightsFOBFREE");
    QDoubleSpinBox *weightsFILFOB = widget_->findChild<QDoubleSpinBox*>("weightsFILFOB");
    QDoubleSpinBox *weightsFILFIL = widget_->findChild<QDoubleSpinBox*>("weightsFILFIL");
    QDoubleSpinBox *weightsFILFREE = widget_->findChild<QDoubleSpinBox*>("weightsFILFREE");


    settings.clear();
    settings.insert(net->objectName(),net->currentText());
    settings.insert(method->objectName(),method->currentIndex());
    settings.insert(countAnt->objectName(),countAnt->value());
    settings.insert(threadsCount->objectName(),threadsCount->value());
    settings.insert(traceMin->objectName(),traceMin->value());
    settings.insert(traceMax->objectName(),traceMax->value());
    settings.insert(traceEvaporation->objectName(),traceEvaporation->value());
    settings.insert(traceCoef->objectName(),traceCoef->value());
    settings.insert(weightsFOBFOB->objectName(),weightsFOBFOB->value());
    settings.insert(weightsFOBFIL->objectName(),weightsFOBFIL->value());
    settings.insert(weightsFOBFREE->objectName(),weightsFOBFREE->value());
    settings.insert(weightsFILFOB->objectName(),weightsFILFOB->value());
    settings.insert(weightsFILFIL->objectName(),weightsFILFIL->value());
    settings.insert(weightsFILFREE->objectName(),weightsFILFREE->value());

    timerUpdate_->start(1000);
    QLabel *errorLabel = widget_->findChild<QLabel*>("error");
    errorLabel->setText("");

    emit started(settings);
}

void Scene::stop()
{
    timerUpdate_->stop();
    QLabel *errorLabel = widget_->findChild<QLabel*>("error");
    errorLabel->setText("");

    emit stopped();
}


void Scene::createProtein()
{
    QSpinBox *genericCount = widget_->findChild<QSpinBox*>("genericCount");
    emit createdProtein(genericCount->value());
}

void Scene::show()
{
    widget_->show();
    widget_->resize(width_, height_);
}


void Scene::addSettingsLabel(QVBoxLayout *layout,QString name, QString title, QString style)
{
   QLabel *label = new QLabel(widget_);
   label->setText(title);
   label->setObjectName(name);
   label->setStyleSheet(style);
   layout->addWidget(label);
}

void Scene::addSettingsComboBox(QVBoxLayout *layout,QString name, QStringList list, int defValue)
{
    QComboBox *box = new QComboBox(widget_);
    box->setObjectName(name);
    box->addItems(list);
    box->setCurrentIndex(defValue);

    layout->addWidget(box);
}

void Scene::addSettingsSpinBox(QVBoxLayout *layout,QString name, int min, int max, int defValue)
{
    QSpinBox *spinBox = new QSpinBox(widget_);
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setValue(defValue);
    spinBox->setObjectName(name);
    layout->addWidget(spinBox);
}

void Scene::addSettingsDoubleSpinBox(QVBoxLayout *layout,QString name, int min, int max, double defValue)
{
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(widget_);
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setValue(defValue);
    spinBox->setObjectName(name);
    spinBox->setSingleStep(0.1);
    layout->addWidget(spinBox);
}

QPushButton *Scene::addSettingsPushButton(QVBoxLayout *layout,QString name, QString title, QString style)
{
    QPushButton *button = new QPushButton(widget_);
    button->setObjectName(name);
    button->setText(title);
    button->setStyleSheet(style);
    layout->addWidget(button);

    return button;
}

Scene::~Scene()
{
}




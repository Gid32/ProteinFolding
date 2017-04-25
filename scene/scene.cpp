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
    protein_ = protein;
    Node *node = nullptr;
    for(int i=0;i<protein.size();i++)
    {
        nodes.push_back(new Node(rootEntity_,protein.at(i),QVector3D(0, 0, 0),node));
        node = nodes.last();
    }
}

void Scene::reDrawBetter()
{
    if(!hasVariantToUpdate_)
        return;
    hasVariantToUpdate_ = false;

    for(int i=1;i<nodes.size();i++)//0 нод не трогаем
         nodes.at(i)->changeLocation(betterVariant.at(i-1));
}

void Scene::reDraw()
{
   if(!drawAll_ || !hasVariant_)
       return;
   hasVariant_ = false;
   for(int i=1;i<nodes.size();i++)//0 нод не трогаем
       nodes.at(i)->changeLocation(variant.at(i-1));
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
    styleButtonBlue_ = "color:white;background-color:#00BFFF;font-size:12px";

    mainLayout_ = new QHBoxLayout(widget_);
    mainLayout_->setMargin(0);

    initCamera();
    initLight();
    setColor(QColor(255,255,255));

    initMainContainer();
    initResultLayout();

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view_->registerAspect(input);


    hasVariantToUpdate_ = false;
    drawAll_ = true;
    hasVariant_ = false;

    timerUpdate_ = new QTimer(this);
    QObject::connect(timerUpdate_, SIGNAL(timeout()), this, SLOT(reDraw()));
    QObject::connect(timerUpdate_, SIGNAL(timeout()), this, SLOT(reDrawBetter()));

    settingsForm = new SettingsForm();

}

void Scene::update(QVector<QVector3D> vectorCoords, int value, QString time)
{
    betterVariant = vectorCoords;
    QLabel *rating = widget_->findChild<QLabel*>("rating");
    QLabel *timeBest = widget_->findChild<QLabel*>("timeBest");
    if(rating)
        rating->setText("Рейтинг: "+QString::number(value));
    if(timeBest)
        timeBest->setText("Время: "+time);
    hasVariantToUpdate_ = true;
}

void Scene::onlyBetter(bool checked)
{
    if(checked)
    {
        drawAll_ = false;
        hasVariantToUpdate_ = true;
    }
    else
        drawAll_ = true;
}

void Scene::hasVariant(QVector<QVector3D> vectorCoords)
{
   variant = vectorCoords;
   hasVariant_ = true;
}

void Scene::saveToFileProtein()
{
    QString filename = QFileDialog::getSaveFileName();
    if(filename.isEmpty())
        return;
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    char *data = new char[protein_.size()];
    for(int i = 0;i < protein_.size();i++)
       data[i] = protein_.at(i);
    file.write(data,protein_.size());
    delete data;
    file.close();
}

void Scene::loadFromFileProtein()
{
    QString filename = QFileDialog::getOpenFileName();
    if(filename.isEmpty())
        return;
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QByteArray array = file.readAll();
    file.close();
    emit loadedProtein(array);
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


void Scene::initResultLayout()
{
    resultLayout_ = new QVBoxLayout();
    resultLayout_->setMargin(5);
    resultLayout_->setAlignment(Qt::AlignTop);
    mainLayout_->addLayout(resultLayout_);

    //settings
    QPushButton * settingsButton = addSettingsPushButton(resultLayout_,"settingsButton","Настройки",styleButtonBlue_);
    connect(settingsButton,SIGNAL(clicked()),this,SLOT(setSettings()));

    //generic protein
    addSettingsLabel(resultLayout_,"genericLabel","Генерация свертки",styleLabel_);
    addSettingsSpinBox(resultLayout_,"genericCount",3,1000,45);
    QPushButton * genericButton = addSettingsPushButton(resultLayout_,"genericButton","Сгенерировать",styleButtonBlue_);
    connect(genericButton,SIGNAL(clicked()),this,SLOT(createProtein()));

    //save
    QPushButton * saveButton = addSettingsPushButton(resultLayout_,"saveButton","Сохранить",styleButtonBlue_);
    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveToFileProtein()));

    //load
    QPushButton * loadButton = addSettingsPushButton(resultLayout_,"loadButton","Загрузить",styleButtonBlue_);
    connect(loadButton,SIGNAL(clicked()),this,SLOT(loadFromFileProtein()));

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

    //only better
    QCheckBox *box = addSettingsCheckBox(resultLayout_,"onlyBetter","показывать только лучший");
    connect(box,SIGNAL(clicked(bool)),this,SLOT(onlyBetter(bool)));
}


void Scene::start()
{
    timerUpdate_->start(1000);
    QLabel *errorLabel = widget_->findChild<QLabel*>("error");
    errorLabel->setText("");

    emit started(settingsForm->getSettings());
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

void Scene::setSettings()
{
    settingsForm->show();
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

QCheckBox *Scene::addSettingsCheckBox(QVBoxLayout *layout,QString name, QString title)
{
    QCheckBox *box = new QCheckBox(title,widget_);
    box->setObjectName(name);
    layout->addWidget(box);
    return box;
}

Scene::~Scene()
{
}




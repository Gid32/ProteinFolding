#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

    view_ = new Qt3DExtras::Qt3DWindow();
    rootEntity_ = new Qt3DCore::QEntity();
    view_->setRootEntity(rootEntity_);
    mainLayout_ = new QHBoxLayout(ui->proteinWidget);
    mainLayout_->setMargin(0);

    //camera
    Qt3DRender::QCamera *cameraEntity_ = view_->camera();
    cameraEntity_->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity_->setPosition(QVector3D(0, 0, 1.0f));
    cameraEntity_->setUpVector(QVector3D(0, 1, 0));
    cameraEntity_->setViewCenter(QVector3D(0, 0, 0));
//    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity_);
//    camController->setCamera(cameraEntity_);
    //light
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity_);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(3);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity_->position());
    lightEntity->addComponent(lightTransform);
    //color
    view_->defaultFrameGraph()->setClearColor(QColor(240,240,240));
    //mainconatainer
    QWidget *mainContainer_ = QWidget::createWindowContainer(view_);
    mainLayout_->addWidget(mainContainer_,1);

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view_->registerAspect(input);


    ui->netValue->addItems(NetFactory::getInstance()->getStringList());
    connect(ui->elitism,SIGNAL(currentIndexChanged(int)),this,SLOT(changeElitizm(int)));

    connect(ui->genericButton,SIGNAL(clicked()),this,SLOT(createProtein()));
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(saveToFileProtein()));
    connect(ui->loadButton,SIGNAL(clicked()),this,SLOT(loadFromFileProtein()));
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::changeElitizm(int index)
{
    bool enabled = false;
    if(index == 1)
        enabled = true;
    ui->traceMin->setEnabled(enabled);
    ui->traceMax->setEnabled(enabled);
}

SETTINGS SettingsForm::getSettings()
{
    SETTINGS settings;
    settings.insert(ui->netValue->objectName(),ui->netValue->currentText());
    settings.insert(ui->methodValue->objectName(),ui->methodValue->currentIndex());
    settings.insert(ui->antsCount->objectName(),ui->antsCount->value());
    settings.insert(ui->threadsCount->objectName(),ui->threadsCount->value());
    settings.insert(ui->traceMin->objectName(),ui->traceMin->value());
    settings.insert(ui->traceMax->objectName(),ui->traceMax->value());
    settings.insert(ui->traceEvoparation->objectName(),ui->traceEvoparation->value());
    settings.insert(ui->traceCoef->objectName(),ui->traceCoef->value());
    settings.insert(ui->weightsFOBFOB->objectName(),ui->weightsFOBFOB->value());
    settings.insert(ui->weightsFOBFIL->objectName(),ui->weightsFOBFIL->value());
    settings.insert(ui->weightsFOBFREE->objectName(),ui->weightsFOBFREE->value());
    settings.insert(ui->weightsFILFOB->objectName(),ui->weightsFILFOB->value());
    settings.insert(ui->weightsFILFIL->objectName(),ui->weightsFILFIL->value());
    settings.insert(ui->weightsFILFREE->objectName(),ui->weightsFILFREE->value());
    settings.insert(ui->elitism->objectName(),ui->elitism->currentIndex());

    return settings;
}

void SettingsForm::saveToFileProtein()
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

void SettingsForm::loadFromFileProtein()
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

void SettingsForm::createProtein()
{
    emit createdProtein(ui->genericCount->value(),ui->genericPercent->value());
}

void SettingsForm::getProtein(VECTORBYTE protein)
{
    protein_ = protein;
    for (int i = 0; i < nodes.size(); ++i)
        delete nodes.at(i);
    nodes.clear();

    float distance = 0.05;
    int countInRow = 24;
    QVector3D start = QVector3D(-0.57,0.38,0);
    for (int i = 0; i < protein_.size(); ++i)
    {
        float x = start.x()+i%countInRow*distance;
        float y = start.y()-(distance*(i/countInRow));
        nodes.push_back(new Node(rootEntity_,protein_.at(i),QVector3D(x,y,0),nullptr,0.02));
    }
}

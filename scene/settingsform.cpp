#include "settingsform.h"
#include "ui_settingsform.h"

Qt3DCore::QEntity* SettingsForm::initWidget(QWidget *widget, QColor color,bool control, float cameraZ)
{
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    Qt3DCore::QEntity* rootEntity = new Qt3DCore::QEntity();
    view->setRootEntity(rootEntity);
    QHBoxLayout *mainLayout = new QHBoxLayout(widget);
    mainLayout->setMargin(0);

    //camera
    Qt3DRender::QCamera *cameraEntity = view->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, cameraZ));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    if(control)
    {
        Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
        camController->setCamera(cameraEntity);
    }
    //light
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(3);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);
    //color
    view->defaultFrameGraph()->setClearColor(color);
    //mainconatainer
    QWidget *mainContainer = QWidget::createWindowContainer(view);
    mainLayout->addWidget(mainContainer,1);

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view->registerAspect(input);
    return rootEntity;
}

SettingsForm::SettingsForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

    rootEntity_ = initWidget(ui->proteinWidget,QColor(240,240,240),false,1.0f);

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

    float distance = 0.05f;
    int countInRow = 22;
    QVector3D start = QVector3D(-0.57f,0.38f,0);
    Node *prev = nullptr;
    for (int i = 0; i < protein_.size(); ++i)
    {
        float x,y;
        y = start.y()-(distance*(i/countInRow));
        if((i/countInRow)%2==0)
            x = start.x()+i%countInRow*distance;
        else
            x = start.x()+(countInRow-(i%countInRow))*distance;
        nodes.push_back(new Node(rootEntity_,protein_.at(i),QVector3D(x,y,0),prev,0.021f,0.005f,0.09f));
        if(i>0)
            nodes.at(i)->changeLocation(QVector3D(x,y,0));
         prev = nodes.at(i);
    }
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Qt3DExtras::Qt3DWindow *view_ = new Qt3DExtras::Qt3DWindow();
    rootEntity_ = new Qt3DCore::QEntity();
    view_->setRootEntity(rootEntity_);
    QHBoxLayout *mainLayout_ = new QHBoxLayout(ui->widget);
    mainLayout_->setMargin(0);

    //camera
    Qt3DRender::QCamera *cameraEntity_ = view_->camera();
    cameraEntity_->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity_->setPosition(QVector3D(0, 0, 15.0f));
    cameraEntity_->setUpVector(QVector3D(0, 1, 0));
    cameraEntity_->setViewCenter(QVector3D(0, 0, 0));
    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity_);
    camController->setCamera(cameraEntity_);
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
    view_->defaultFrameGraph()->setClearColor(QColor(255,255,255));
    //mainconatainer
    QWidget *mainContainer_ = QWidget::createWindowContainer(view_);
    mainLayout_->addWidget(mainContainer_,1);

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view_->registerAspect(input);

    settingsForm_ = new SettingsForm();
    timer_ = new QTimer;

    connect(ui->settingsButton,SIGNAL(clicked(bool)),this,SLOT(setSettings()));
    connect(ui->startButton,SIGNAL(clicked(bool)),this,SLOT(start()));
    connect(ui->stopButton,SIGNAL(clicked(bool)),this,SLOT(stop()));
    connect(ui->onlyBetter,SIGNAL(clicked(bool)),this,SLOT(onlyBetter(bool)));
    connect(timer_,SIGNAL(timeout()),this,SLOT(reDraw()));
}

void MainWindow::createProtein(VECTORBYTE protein)
{
    for(int i=0;i<nodes_.size();i++)
        delete nodes_.at(i);
    nodes_.clear();

    protein_ = protein;
    Node *node = nullptr;
    for(int i=0;i<protein.size();i++)
    {
        nodes_.push_back(new Node(rootEntity_,protein.at(i),QVector3D(0, 0, 0),node));
        node = nodes_.last();
    }
}

void MainWindow::setSettings()
{
    settingsForm_->show();
}

SettingsForm *MainWindow::getSettingsForm()
{
    return settingsForm_;
}

void MainWindow::start()
{
    timer_->start(1000);
    ui->error->setText("");
    emit started(settingsForm_->getSettings());
}

void MainWindow::stop()
{
    timer_->stop();
    ui->error->setText("");
    emit stopped();
}

void MainWindow::countConvolution(int count)
{
    ui->countConvolution->setText("Количество сверток: "+QString::number(count));
    ui->countConvolution->resize(ui->countConvolution->sizeHint());
}

void MainWindow::getError(QString error)
{
   ui->error->setText(error);
   ui->error->resize(ui->error->sizeHint());
}

void MainWindow::hasBetterVariant(QVector<QVector3D> vectorCoords, int value, QString time)
{
    betterVariant_ = vectorCoords;
    ui->rating->setText("Рейтинг: "+QString::number(value/2));
    ui->bestTime->setText("Время: "+time);
    ui->bestTime->resize(ui->bestTime->sizeHint());
    ui->rating->resize(ui->rating->sizeHint());
    hasBetterVariant_ = true;
}


void MainWindow::hasVariant(QVector<QVector3D> vectorCoords)
{
   variant_ = vectorCoords;
   hasVariant_ = true;
}

void MainWindow::reDraw()
{
   QVector<QVector3D> variant;
   if(drawAll_ && hasVariant_)
      variant = variant_;
   if(hasBetterVariant_)
      variant = betterVariant_;
   if(variant.empty())
       return;
   hasVariant_ = false;
   hasBetterVariant_ = false;
   for(int i=1;i<nodes_.size();i++)//0 нод не трогаем
       nodes_.at(i)->changeLocation(variant.at(i-1));
}

void MainWindow::onlyBetter(bool checked)
{
    if(checked)
    {
        drawAll_ = false;
        hasBetterVariant_ = true;
    }
    else
        drawAll_ = true;
}


MainWindow::~MainWindow()
{
    delete ui;
}


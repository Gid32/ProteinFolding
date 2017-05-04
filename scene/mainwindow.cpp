#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view_ = new Qt3DExtras::Qt3DWindow();
    rootEntity_ = SettingsForm::initWidget(ui->widget,view_,QColor(240,240,240),true,15.0f);

    settingsForm_ = new SettingsForm();
    timer_ = new QTimer;
    showConnection_ = false;
    drawAll_ = true;
    connect(ui->settingsButton,SIGNAL(clicked(bool)),this,SLOT(setSettings()));
    connect(ui->startButton,SIGNAL(clicked(bool)),this,SLOT(start()));
    connect(ui->stopButton,SIGNAL(clicked(bool)),this,SLOT(stop()));
    connect(ui->onlyBetter,SIGNAL(clicked(bool)),this,SLOT(onlyBetter(bool)));
    connect(ui->showConnection,SIGNAL(clicked(bool)),this,SLOT(showConnection(bool)));
    connect(ui->showGraph,SIGNAL(clicked(bool)),this,SLOT(showGraph(bool)));
    connect(timer_,SIGNAL(timeout()),this,SLOT(reDraw()));
    connect(ui->cameraBack,SIGNAL(clicked(bool)),this,SLOT(cameraBack()));
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
    ui->proteinLength->setText("длина свертки: "+QString::number(protein_.size()));
    ui->proteinLength->resize(ui->proteinLength->sizeHint());
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
    timer_->start(1500);
    ui->error->setText("");
    hasBetterVariant_ = false;
    hasVariant_ = false;
    emit started(settingsForm_->getSettings());
}

void MainWindow::stop()
{
    timer_->stop();
    ui->error->setText("");
    emit stopped();
}

void MainWindow::countConvolution(int count, int rating)
{
    ui->countConvolution->setText("Количество сверток: "+QString::number(count));
    ui->countConvolution->resize(ui->countConvolution->sizeHint());
    double avarageRating = (rating/2)/count;
    ui->avarageRating->setText("Средний рейтинг: "+QString::number(avarageRating));
    ui->avarageRating->resize(ui->avarageRating->sizeHint());
}

void MainWindow::getError(QString error)
{
   ui->error->setText(error);
   ui->error->resize(ui->error->sizeHint());
}

void MainWindow::hasBetterVariant(Convolution convolution, QString time)
{
    betterVariant_ = convolution;
    ui->rating->setText("Рейтинг: "+QString::number(convolution.result_/2));
    ui->bestTime->setText("Время: "+time);
    ui->bestTime->resize(ui->bestTime->sizeHint());
    ui->rating->resize(ui->rating->sizeHint());
    hasBetterVariant_ = true;
}


void MainWindow::hasVariant(Convolution convolution)
{
   variant_ = convolution;
   hasVariant_ = true;
}

void MainWindow::reDraw()
{
   if(!hasBetterVariant_ && !hasVariant_)
       return;
   Convolution variant;
   if(drawAll_ && hasVariant_)
      variant = variant_;
   else if(hasBetterVariant_)
      variant = betterVariant_;
   if(variant.result_==-1)
       return;
   hasVariant_ = false;
   hasBetterVariant_ = false;
   QVector<QVector3D> vectorCoords = ConvolutionFactory::getFactory()->getVectorCoords(&variant);
   for(int i=1;i<nodes_.size();i++)//0 нод не трогаем
       nodes_.at(i)->changeLocation(vectorCoords.at(i-1));

   //связи
   for(int i=0;i<currentConnections.size();i++)
       delete currentConnections.at(i);
   currentConnections.clear();
   if(!showConnection_)
       return;
   for(int i=0;i<variant.connections_.size();i++)
   {
       Connection *connection = new Connection(rootEntity_,QVector3D(0,0,0));
       connection->changeLocation(variant.connections_.at(i).second,variant.connections_.at(i).first);
       connection->changeColor(QColor(255,0,0,40));
       currentConnections.push_back(connection);
   }
}

void MainWindow::showConnection(bool checked)
{
    showConnection_ = checked;
    hasVariant_ = true;
    hasBetterVariant_ = true;
    reDraw();
}

void MainWindow::onlyBetter(bool checked)
{
    drawAll_ = !checked;
    hasBetterVariant_ = checked;
    reDraw();
}

void MainWindow::showGraph(bool checked)
{
    if(checked)
        connect(timer_,SIGNAL(timeout()),this,SLOT(reDraw()));
    else
        disconnect(timer_,SIGNAL(timeout()),this,SLOT(reDraw()));
}

void MainWindow::cameraBack()
{
    view_->camera()->setPosition(QVector3D(0.0f,0.0f,20.0f));
    view_->camera()->setViewCenter(QVector3D(0.0f,0.0f,0.0f));
}


MainWindow::~MainWindow()
{
    delete ui;
}


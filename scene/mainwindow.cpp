#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view_ = new Qt3DExtras::Qt3DWindow();
    rootEntity_ = ProteinForm::initWidget(ui->widget,view_,QColor(240,240,240),true,15.0f);

    settingsForm_ = new SettingsForm();
    proteinForm_ = new ProteinForm();
    timer_ = new QTimer;
    showConnection_ = false;
    drawAll_ = true;
    connect(ui->settingsButton,SIGNAL(clicked(bool)),settingsForm_,SLOT(show()));
    connect(ui->startButton,SIGNAL(clicked(bool)),this,SLOT(start()));
    connect(ui->stopButton,SIGNAL(clicked(bool)),this,SLOT(stop()));
    connect(ui->onlyBetter,SIGNAL(clicked(bool)),this,SLOT(onlyBetter(bool)));
    connect(ui->showConnection,SIGNAL(clicked(bool)),this,SLOT(showConnection(bool)));
    connect(ui->showGraph,SIGNAL(clicked(bool)),this,SLOT(showGraph(bool)));
    connect(timer_,SIGNAL(timeout()),this,SLOT(reDraw()));
    connect(ui->cameraBack,SIGNAL(clicked(bool)),this,SLOT(cameraBack()));
    connect(settingsForm_,SIGNAL(accepted()),this,SLOT(addLaunch()));
    connect(ui->addProtein,SIGNAL(clicked(bool)),proteinForm_,SLOT(show()));
    connect(proteinForm_,SIGNAL(accepted()),this,SLOT(addProtein()));
    status_ = -1;
    currentLaunch_ = 0;
    currentSubLaunch_ = 0;
    countFullLaunches_ = 0;
    currentProtein_ = 0;
}

void MainWindow::addLaunch()
{
    SETTINGS settings = settingsForm_->getSettings();
    launches_.push_back(settings);
    int count = settings.value("countLaunch").toInt();
    countFullLaunches_+= count;
    ui->countLaunches->setText("Количество запусков: "+QString::number(launches_.size()));
    ui->fullCounLaunch->setText("Количество всех запусков: "+QString::number(countFullLaunches_));
}

void MainWindow::addProtein()
{
    VECTORBYTE protein = proteinForm_->protein_;
    proteins_.push_back(protein);
    ui->countProtein->setText("Количество протеинов "+QString::number(proteins_.size()));
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

void MainWindow::changeStatus(int status)
{
    status_ = status;
    switch (status_)
    {
    case -1:ui->status->setText("Ожидание");
        break;
    case 0:ui->status->setText("Остановлено");
        break;
    case 1:ui->status->setText("Работает");
        break;
    case 10:ui->status->setText("Выполнено");
        break;
    case 100:ui->status->setText("Выполнено всё");
        break;
    default:
        break;
    }

    if(status_ == -1 || status_ == 10)
    {
        timer_->stop();
        if(launches_.size() <= currentLaunch_)
        {
            currentProtein_++;
            currentLaunch_ = 0;
            currentSubLaunch_ = 0;
        }
        if(proteins_.size() <= currentProtein_)
        {
            changeStatus(100);
            return;
        }
        SETTINGS settings = launches_.at(currentLaunch_);
        SETTINGS::iterator protIt = settings.find("PROTEIN");
        if(protIt == settings.end())
            settings.insert("PROTEIN",QVariant::fromValue(proteins_.at(currentProtein_)));
        else
           settings["PROTEIN"] = QVariant::fromValue(proteins_.at(currentProtein_));
        ui->status->setText("Создаем Протеин");
        createProtein(proteins_.at(currentProtein_));
        int count = settings.value("countLaunch").toInt();
        ui->currentLaunch->setText("Текущий запуск: "+ QString::number(currentLaunch_+1)+
                                   "\nСубзапуск: "+QString::number(currentSubLaunch_+1)+" из "+QString::number(count)+
                                   "\nПротеин: "+QString::number(currentProtein_+1)+" из "+QString::number(proteins_.size()));
        currentSubLaunch_++;
        if(currentSubLaunch_ == count)
        {
            currentSubLaunch_ = 0;
            currentLaunch_++;
        }
        timer_->start(1500);
        emit started(settings);
    }

}

SettingsForm *MainWindow::getSettingsForm()
{
    return settingsForm_;
}

ProteinForm *MainWindow::getProteinForm()
{
    return proteinForm_;
}

void MainWindow::start()
{
    if(status_ == 1)
    {
        getError("уже работает");
        return;
    }
    if(launches_.size()==0)
    {
        getError("Нет запусков");
        return;
    }
    if(proteins_.size()==0)
    {
        getError("Нет протеинов");
        return;
    }
    ui->error->setText("");
    hasBetterVariant_ = false;
    hasVariant_ = false;
    changeStatus(-1);
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


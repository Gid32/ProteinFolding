#include "resultform.h"
#include "ui_resultform.h"

ResultForm::ResultForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultForm)
{
    ui->setupUi(this);
    view_ = new Qt3DExtras::Qt3DWindow();
    rootEntity_ = ProteinForm::initWidget(ui->widget,view_,QColor(240,240,240),true,15.0f);

    connect(ui->protein,SIGNAL(currentIndexChanged(int)),this,SLOT(proteinChange(int)));
    connect(ui->launchNumber,SIGNAL(currentIndexChanged(int)),this,SLOT(launchChange(int)));
    connect(ui->subLaunchNumber,SIGNAL(currentIndexChanged(int)),this,SLOT(subLaunchChange(int)));
    connect(ui->show,SIGNAL(clicked(bool)),this,SLOT(showProtein()));
}

ResultForm::~ResultForm()
{
    delete ui;
}

void ResultForm::load(QMap<int, QVector<Result> > results, QVector<VECTORBYTE> proteins)
{
    results_ = results;
    proteins_ = proteins;

    ui->protein->clear();

    for(int i=0;i<proteins_.size();i++)
        ui->protein->addItem(QString::number(i+1)+" длина:"+QString::number(proteins_.at(i).size()),i);

}

void ResultForm::proteinChange(int proteinN)
{
    if(proteinN < 0)
        return;

    QVector<Result> results = results_[proteinN];

    int alltime = 0;
    int allBestTime = 0;

    double averageResult = 0;
    int bestResult = 0;
    int allResult = 0;
    int countSub = 0;
    int countConv = 0;
    ui->launchNumber->clear();
    for(int i=0;i<results.size();i++)
    {
        ui->launchNumber->addItem(QString::number(i+1)+" подзапусков:"+QString::number(results.at(i).results.size()),i);
        for(int j=0;j<results.at(i).results.size();j++)
        {
            countSub++;
            alltime += QTime(0,0,0,0).msecsTo(QTime::fromString(results.at(i).results.at(j).timeWork_,"hh:mm:ss.zzz"));
            allBestTime += QTime(0,0,0,0).msecsTo(QTime::fromString(results.at(i).results.at(j).timeBest_,"hh:mm:ss.zzz"));

            int res = results.at(i).results.at(j).convolution_.result_;
            if(res > bestResult)
                bestResult = res;
            allResult += res;
            averageResult += results.at(i).results.at(j).averageRating_;
            countConv += results.at(i).results.at(j).countConvolution_;
        }
    }

    int msAverageTime = alltime/countSub;
    int msAverageBestTime = allBestTime/countSub;
    QTime averageTime = QTime(0,0,0,0).addMSecs(msAverageTime);
    QTime averageBestTime = QTime(0,0,0,0).addMSecs(msAverageBestTime);

    ui->allTime->setText(QTime(0,0,0,0).addMSecs(alltime).toString("hh:mm:ss.zzz"));
    ui->allTime->resize(ui->allTime->sizeHint());
    ui->averageResult->setText(QString::number(averageResult/results.size()));
    ui->averageResult->resize(ui->averageResult->sizeHint());
    ui->averageTime->setText(averageTime.toString("hh:mm:ss.zzz"));
    ui->averageTime->resize(ui->averageTime->sizeHint());
    ui->averageTimeBest->setText(averageBestTime.toString("hh:mm:ss.zzz"));
    ui->averageTimeBest->resize(ui->averageTimeBest->sizeHint());
    ui->bestResult->setText(QString::number(bestResult));
    ui->bestResult->resize(ui->bestResult->sizeHint());
    ui->countConvolution->setText(QString::number(countConv));
    ui->countConvolution->resize(ui->countConvolution->sizeHint());
}

void ResultForm::launchChange(int index)
{
    int proteinIndex = ui->protein->currentIndex();
    QVector<Result> results = results_[proteinIndex];

    QVector<SubResult> subResults = results.at(index).results;
    for(int i=0;i<subResults.size();i++)
    {
        ui->subLaunchNumber->addItem(QString::number(i+1)+" Результат: "+QString::number(subResults.at(i).convolution_.result_));
    }
}

void ResultForm::subLaunchChange(int index)
{
    int proteinIndex = ui->protein->currentIndex();
    int launchIndex = ui->launchNumber->currentIndex();
    QVector<Result> results = results_[proteinIndex];
    QVector<SubResult> subResults = results.at(launchIndex).results;
    SubResult subResult = subResults.at(index);

    ui->subAverageResult->setText(QString::number(subResult.averageRating_));
    ui->subAverageResult->resize(ui->subAverageResult->sizeHint());
    ui->subBestResult->setText(QString::number(subResult.convolution_.result_));
    ui->subBestResult->resize(ui->subBestResult->sizeHint());
    ui->subCountConvolution->setText(QString::number(subResult.countConvolution_));
    ui->subCountConvolution->resize(ui->subCountConvolution->sizeHint());
}

void ResultForm::showProtein()
{
    for(int i=0;i<nodes_.size();i++)
        delete nodes_.at(i);
    nodes_.clear();
    int proteinIndex = ui->protein->currentIndex();
    VECTORBYTE protein = proteins_.at(proteinIndex);
    Node *node = nullptr;
    for(int i=0;i<protein.size();i++)
    {
        nodes_.push_back(new Node(rootEntity_,protein.at(i),QVector3D(0, 0, 0),node));
        node = nodes_.last();
    }

    int launchIndex = ui->launchNumber->currentIndex();
    int subLaunchIndex = ui->subLaunchNumber->currentIndex();
    QVector<Result> results = results_[proteinIndex];
    QVector<SubResult> subResults = results.at(launchIndex).results;
    SubResult subResult = subResults.at(subLaunchIndex);

    QVector<QVector3D> vectorCoords = ConvolutionFactory::getFactory()->getVectorCoords(&subResult.convolution_);
    for(int i=1;i<nodes_.size();i++)//0 нод не трогаем
        nodes_.at(i)->changeLocation(vectorCoords.at(i-1));
}

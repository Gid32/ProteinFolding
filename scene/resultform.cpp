#include "resultform.h"
#include "ui_resultform.h"

ResultForm::ResultForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultForm)
{
    ui->setupUi(this);
    view_ = new Qt3DExtras::Qt3DWindow();
    rootEntity_ = ProteinForm::initWidget(ui->widget,view_,QColor(240,240,240),true,15.0f);

    settingsForm_ = new SettingsForm();

    connect(ui->protein,SIGNAL(currentIndexChanged(int)),this,SLOT(proteinChange(int)));
    connect(ui->launchNumber,SIGNAL(currentIndexChanged(int)),this,SLOT(launchChange(int)));
    connect(ui->subLaunchNumber,SIGNAL(currentIndexChanged(int)),this,SLOT(subLaunchChange(int)));
    connect(ui->show,SIGNAL(clicked(bool)),this,SLOT(showProtein()));
    connect(ui->showSettings,SIGNAL(clicked(bool)),this,SLOT(showSettings()));
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

    for(int i=0;i<results.size();i++)
        ui->launchNumber->addItem(QString::number(i+1)+" подзапусков:"+QString::number(results[i].getCountSub()),i);

    int alltime;
    int allBestTime;
    double averageResult;
    int bestResult;
    int countConv;
    getProteinResult(results, alltime, allBestTime, averageResult, bestResult, countConv);

    int msAverageBestTime = allBestTime/results.size();
    QTime averageBestTime = QTime(0,0,0,0).addMSecs(msAverageBestTime);
    int msAverageTime = alltime/results.size();
    QTime averageTime = QTime(0,0,0,0).addMSecs(msAverageTime);

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

    QVector<SubResult> subResults = results[index].getSubResults();
    for(int i=0;i<subResults.size();i++)
    {
        ui->subLaunchNumber->addItem(QString::number(i+1)+" Результат: "+QString::number(subResults[i].getConvolution().result_));
    }

    ui->allTimeR->setText(results[index].getAllTime().toString("hh:mm:ss.zzz"));
    ui->allTimeR->resize(ui->allTimeR->sizeHint());
    ui->averageResultR->setText(QString::number(results[index].getAverageResult()));
    ui->averageResultR->resize(ui->averageResultR->sizeHint());
    ui->averageTimeR->setText(results[index].getAverageTime().toString("hh:mm:ss.zzz"));
    ui->averageTimeR->resize(ui->averageTimeR->sizeHint());
    ui->averageTimeBestR->setText(results[index].getAverageBestTime().toString("hh:mm:ss.zzz"));
    ui->averageTimeBestR->resize(ui->averageTimeBestR->sizeHint());
    ui->bestResultR->setText(QString::number(results[index].getBestResult()));
    ui->bestResultR->resize(ui->bestResultR->sizeHint());
    ui->countConvolutionR->setText(QString::number(results[index].getCountConv()));
    ui->countConvolutionR->resize(ui->countConvolutionR->sizeHint());
}

void ResultForm::subLaunchChange(int index)
{
    int proteinIndex = ui->protein->currentIndex();
    int launchIndex = ui->launchNumber->currentIndex();
    QVector<Result> results = results_[proteinIndex];
    QVector<SubResult> subResults = results[launchIndex].getSubResults();
    SubResult subResult = subResults.at(index);

    ui->subAverageResult->setText(QString::number(subResult.getAverageRating()));
    ui->subAverageResult->resize(ui->subAverageResult->sizeHint());
    ui->subBestResult->setText(QString::number(subResult.getConvolution().result_));
    ui->subBestResult->resize(ui->subBestResult->sizeHint());
    ui->subCountConvolution->setText(QString::number(subResult.getCountConvolution()));
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
    QVector<SubResult> subResults = results[launchIndex].getSubResults();
    SubResult subResult = subResults.at(subLaunchIndex);

    QVector<QVector3D> vectorCoords = ConvolutionFactory::getFactory()->getVectorCoords(&subResult.getConvolution());
    for(int i=1;i<nodes_.size();i++)//0 нод не трогаем
        nodes_.at(i)->changeLocation(vectorCoords.at(i-1));
}

void ResultForm::showSettings()
{
    int proteinIndex = ui->protein->currentIndex();
    int launchIndex = ui->launchNumber->currentIndex();
    QVector<Result> results = results_[proteinIndex];

    settingsForm_->loadSettings(results.at(launchIndex).settings);
    settingsForm_->show();
}

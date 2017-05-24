#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

    double arr[][2] = {0.60,0.20,0.15,0.40,0.25,0.40};
    QString arrName[][2] = {"weightsFOBFOB","weightsFILFOB","weightsFOBFIL","weightsFILFIL","weightsFOBFREE","weightsFILFREE"};
    for(int i=0;i<3;i++)
        for(int j=0;j<2;j++)
        {
            ui->tableWidget->setColumnWidth(i,45);
            ui->tableWidget->setRowHeight(j,25);
            QDoubleSpinBox *box = new QDoubleSpinBox(ui->tableWidget);
            box->setRange(0,1);
            box->setSingleStep(0.1);
            box->setValue(arr[i][j]);
            box->setObjectName(arrName[i][j]);
            ui->tableWidget->setCellWidget(j,i,box);
        }

    ui->netValue->addItems(NetFactory::getInstance()->getStringList());
    ui->netValue->setCurrentIndex(1);
    connect(ui->elitism,SIGNAL(currentIndexChanged(int)),this,SLOT(changeElitizm(int)));
    connect(ui->exit,SIGNAL(currentIndexChanged(int)),this,SLOT(exitVariant(int)));

    ui->countExit->setEnabled(false);
    ui->timeExit->setEnabled(true);
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::exitVariant(int variant)
{
    bool flag = false;
    if(variant==2)
        flag = true;
    ui->countExit->setEnabled(flag);
    ui->timeExit->setEnabled(!flag);
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
    for(int i=0;i<3;i++)
        for(int j=0;j<2;j++)
        {
            QDoubleSpinBox *box = static_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(j,i));
            settings.insert(box->objectName(),box->value());
        }
    settings.insert(ui->elitism->objectName(),ui->elitism->currentIndex());
    settings.insert(ui->exit->objectName(),ui->exit->currentIndex());
    settings.insert(ui->timeExit->objectName(),ui->timeExit->time());
    settings.insert(ui->countExit->objectName(),ui->countExit->value());
    settings.insert(ui->countLaunch->objectName(),ui->countLaunch->value());
    //settings.insert("PROTEIN", QVariant::fromValue(protein_));

    return settings;
}

void SettingsForm::loadSettings(SETTINGS settings)
{
    ui->netValue->setCurrentText(settings.value(ui->netValue->objectName()).toString());
    ui->methodValue->setCurrentIndex(settings.value(ui->methodValue->objectName()).toInt());
    ui->antsCount->setValue(settings.value(ui->antsCount->objectName()).toInt());
    ui->threadsCount->setValue(settings.value(ui->threadsCount->objectName()).toInt());
    ui->traceMin->setValue(settings.value(ui->traceMin->objectName()).toDouble());
    ui->traceMax->setValue(settings.value(ui->traceMax->objectName()).toDouble());
    ui->traceEvoparation->setValue(settings.value(ui->traceEvoparation->objectName()).toDouble());
    ui->traceCoef->setValue(settings.value(ui->traceCoef->objectName()).toDouble());

    for(int i=0;i<3;i++)
        for(int j=0;j<2;j++)
        {
            QDoubleSpinBox *box = static_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(j,i));
            box->setValue(settings.value(box->objectName()).toDouble());
        }

    ui->elitism->setCurrentIndex(settings.value(ui->elitism->objectName()).toInt());
    ui->exit->setCurrentIndex(settings.value(ui->exit->objectName()).toInt());
    ui->timeExit->setTime(settings.value(ui->timeExit->objectName()).toTime());
    ui->countExit->setValue(settings.value(ui->countExit->objectName()).toInt());
    ui->countLaunch->setValue(settings.value(ui->countLaunch->objectName()).toInt());

}

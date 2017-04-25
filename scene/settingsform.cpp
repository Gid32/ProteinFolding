#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    ui->netValue->addItems(NetFactory::getInstance()->getStringList());
    connect(ui->elitism,SIGNAL(currentIndexChanged(int)),this,SLOT(changeElitizm(int)));
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::changeElitizm(int index)
{
    bool enabled = false;
    if(index == 0)
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

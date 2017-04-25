#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QDialog>
#include "settings.h"
#include "netfactory.h"

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = 0);
    ~SettingsForm();
    SETTINGS getSettings();

public slots:
    void changeElitizm(int index);

private:
    Ui::SettingsForm *ui;
};

#endif // SETTINGSFORM_H

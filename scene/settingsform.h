#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QDialog>
#include <QFileDialog>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>
#include <Qt3DInput/QInputAspect>
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>

#include "settings.h"
#include "netfactory.h"
#include "node.h"


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

signals:
    void createdProtein(int,double);
    void loadedProtein(QByteArray);
public slots:
    void changeElitizm(int index);
    void saveToFileProtein();
    void loadFromFileProtein();
    void createProtein();
    void getProtein(VECTORBYTE protein);
private:
    Qt3DCore::QEntity* rootEntity_;
    QVector<Node*> nodes;
    VECTORBYTE protein_;
    Ui::SettingsForm *ui;
};

#endif // SETTINGSFORM_H

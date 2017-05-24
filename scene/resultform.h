#ifndef RESULTFORM_H
#define RESULTFORM_H

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
#include "node.h"
#include "result.h"
#include "convolutionfactory.h"
#include "proteinForm.h"

namespace Ui {
class ResultForm;
}

class ResultForm : public QDialog
{
    Q_OBJECT

public:
    explicit ResultForm(QWidget *parent = 0);
    ~ResultForm();
    void load(QMap<int,QVector<Result>> results,QVector<VECTORBYTE> proteins);
public slots:
    void proteinChange(int proteinN);
    void launchChange(int index);
    void subLaunchChange(int index);
    void showProtein();
private:
    Ui::ResultForm *ui;
    QMap<int,QVector<Result>> results_;
    QVector<VECTORBYTE> proteins_;

    Qt3DCore::QEntity* rootEntity_;
    Qt3DExtras::Qt3DWindow *view_;
    QVector<Node*> nodes_;
};

#endif // RESULTFORM_H

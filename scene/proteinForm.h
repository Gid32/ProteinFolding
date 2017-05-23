#ifndef PROTEIN_H
#define PROTEIN_H

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

namespace Ui {
class ProteinForm;
}

class ProteinForm : public QDialog
{
    Q_OBJECT

public:
    explicit ProteinForm(QWidget *parent = 0);
    ~ProteinForm();
    static Qt3DCore::QEntity *initWidget(QWidget *widget, Qt3DExtras::Qt3DWindow *view, QColor color, bool control, float cameraZ);
    VECTORBYTE protein_;
signals:
    void createdProtein(int,double);
    void loadedProtein(QByteArray);
public slots:
    void saveToFileProtein();
    void loadFromFileProtein();
    void createProtein();
    void getProtein(VECTORBYTE protein);
private:
    Ui::ProteinForm *ui;
    Qt3DCore::QEntity* rootEntity_;
    Qt3DExtras::Qt3DWindow *view_;
    QVector<Node*> nodes;
};

#endif // PROTEIN_H

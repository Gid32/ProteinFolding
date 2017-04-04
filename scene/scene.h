#ifndef Scene_H
#define Scene_H
#include <QtCore/QObject>

#include <QObject>
#include <QGuiApplication>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>

#include <Qt3DInput/QInputAspect>
#include <Qt3DExtras/qt3dwindow.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>


#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>

#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#include <QThread>

#include "node.h"
#include "connection.h"
#include "netfactory.h"

class Scene : public QObject
{
    Q_OBJECT

public:
    explicit Scene(const char *title, int width, int height);
    void show();
    ~Scene();
signals:
    void started(QString,int);
    void stopped();
public slots:
    void update(QVector<QVector3D> vectorCoords, int value);
    void genericNodes(VECTORBYTE protein);
    void reDraw();
    void start();
    void stop();
    void countConvolution(int count);
private:
    const char* title_;
    Qt3DExtras::Qt3DWindow* view_;
    Qt3DCore::QEntity* rootEntity_;
    QWidget* widget_;
    Qt3DRender::QCamera *cameraEntity_;
    QHBoxLayout *mainLayout_;
    QVBoxLayout *settingsLayout_;
    QWidget *mainContainer_;
    int width_;
    int height_;

    QLabel *settingsLabel_;

    QLabel *settingsNetChangesLabel_;
    QLabel *settingsSelectMethodLabel_;
    QLabel *settingsRateLabel_;
    QLabel *settingsRate_;
    QLabel *settingsCountConvolutionLabel_;
    QLabel *settingsCountConvolution_;
    QComboBox *settingsNetChanges_;
    QComboBox *settingsSelectMethod_;
    QPushButton *settingsStartApplication_;
    QPushButton *settingsStopApplication_;

    QTimer *timerUpdate_;

    bool hasVariantToUpdate_;
    QVector<QVector3D> vectorCoords;
    QVector<Node*> nodes;

    void setColor(QColor color);
    void initLight();
    void initCamera();
    void initMainContainer();
    void initSettingsLayout();

    void initSettingsLabel();
    void initSettingsNetChanges();
    void initSettingsSelectMethod();
    void initSettingsRate();
    void initSettingsStartStopApplication();
    void initSettingsCountConvolution();


    void clear();
    void test();
};

#endif // Scene_H


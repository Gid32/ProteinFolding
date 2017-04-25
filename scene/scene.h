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
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QFileDialog>

#include <QThread>

#include "node.h"
#include "connection.h"
#include "settingsform.h"

class Scene : public QObject
{
    Q_OBJECT

public:
    explicit Scene(const char *title, int width, int height);
    void show();
    ~Scene();
signals:
    void started(SETTINGS);
    void createdProtein(int,double);
    void stopped();
    void loadedProtein(QByteArray);
public slots:
    void onlyBetter(bool checked);
    void update(QVector<QVector3D> vectorCoords, int value, QString time);
    void hasVariant(QVector<QVector3D> vectorCoords);
    void genericNodes(VECTORBYTE protein);
    void getError(QString error);
    void reDraw();
    void reDrawBetter();
    void start();
    void stop();
    void countConvolution(int count);
    void createProtein();
    void saveToFileProtein();
    void loadFromFileProtein();
    void setSettings();
private:
    SettingsForm *settingsForm;
    const char* title_;
    Qt3DExtras::Qt3DWindow* view_;
    Qt3DCore::QEntity* rootEntity_;
    QWidget* widget_;
    Qt3DRender::QCamera *cameraEntity_;
    QHBoxLayout *mainLayout_;
    QVBoxLayout *resultLayout_;
    QWidget *mainContainer_;
    QString styleLabel_;
    QString styleLabelValue_;
    QString styleButtonRed_;
    QString styleButtonGreen_;
    QString styleButtonBlue_;
    int width_;
    int height_;

    QTimer *timerUpdate_;

    VECTORBYTE protein_;
    bool hasVariantToUpdate_;
    bool drawAll_;
    bool hasVariant_;
    QVector<QVector3D> betterVariant;
    QVector<QVector3D> variant;
    QVector<Node*> nodes;

    void setColor(QColor color);
    void initLight();
    void initCamera();
    void initMainContainer();
    void initResultLayout();

    void addSettingsLabel(QVBoxLayout *layout, QString name, QString title, QString style);
    void addSettingsComboBox(QVBoxLayout *layout, QString name, QStringList list, int defValue);
    void addSettingsSpinBox(QVBoxLayout *layout, QString name, int min, int max, int defValue);
    void addSettingsDoubleSpinBox(QVBoxLayout *layout, QString name, int min, int max, double defValue);
    QPushButton *addSettingsPushButton(QVBoxLayout *layout, QString name, QString title, QString style);
    QCheckBox *addSettingsCheckBox(QVBoxLayout *layout,QString name, QString title);

    void clear();
};

#endif // Scene_H


#ifndef Scene_H
#define Scene_H
#include <QtCore/QObject>

#include <QObject>
#include <QGuiApplication>

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
    SettingsForm *getSettingsForm();
    ~Scene();
signals:
    void started(SETTINGS);
    void stopped();
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


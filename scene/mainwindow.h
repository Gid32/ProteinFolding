#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsform.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SettingsForm *getSettingsForm();
signals:
    void started(SETTINGS);
    void stopped();
public slots:
    void createProtein(VECTORBYTE protein);
    void setSettings();
    void start();
    void stop();
    void countConvolution(int count);
    void getError(QString error);
    void hasBetterVariant(QVector<QVector3D> vectorCoords, int value, QString time);
    void hasVariant(QVector<QVector3D> vectorCoords);
    void reDraw();
    void onlyBetter(bool checked);
private:
    Ui::MainWindow *ui;
    Qt3DCore::QEntity* rootEntity_;
    SettingsForm *settingsForm_;
    QTimer *timer_;

    bool hasBetterVariant_;
    bool hasVariant_;
    bool drawAll_;
    QVector<QVector3D> betterVariant_;
    QVector<QVector3D> variant_;

    QVector<Node*> nodes_;
    VECTORBYTE protein_;
};

#endif // MAINWINDOW_H

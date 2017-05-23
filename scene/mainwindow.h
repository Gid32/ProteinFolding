#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsform.h"
#include "proteinForm.h"
#include "convolutionfactory.h"
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
    ProteinForm *getProteinForm();
signals:
    void started(SETTINGS);
    void stopped();
public slots:
    void createProtein(VECTORBYTE protein);
    void start();
    void stop();
    void countConvolution(int count,int rating);
    void getError(QString error);
    void hasBetterVariant(Convolution convolution, QString time);
    void hasVariant(Convolution convolution);
    void reDraw();
    void onlyBetter(bool checked);
    void showConnection(bool checked);
    void showGraph(bool checked);
    void cameraBack();
    void changeStatus(int status);
    void addLaunch();
    void addProtein();
private:
    Ui::MainWindow *ui;
    Qt3DCore::QEntity* rootEntity_;
    Qt3DExtras::Qt3DWindow *view_;
    SettingsForm *settingsForm_;
    ProteinForm *proteinForm_;
    QTimer *timer_;

    int currentLaunch_;
    int currentSubLaunch_;
    int countFullLaunches_;
    int currentProtein_;
    QVector<SETTINGS> launches_;
    QVector<VECTORBYTE> proteins_;

    int status_;
    bool hasBetterVariant_;
    bool showConnection_;
    bool hasVariant_;
    bool drawAll_;
    Convolution betterVariant_;
    Convolution variant_;
    QVector<Connection*> currentConnections;

    QVector<Node*> nodes_;
    VECTORBYTE protein_;
};

#endif // MAINWINDOW_H

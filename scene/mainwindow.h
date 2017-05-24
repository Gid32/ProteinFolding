#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsform.h"
#include "proteinForm.h"
#include "convolutionfactory.h"
#include <QTimer>
#include "result.h"
#include "resultform.h"

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
    void hasVariant(Convolution convolution, QString time);
    void reDraw();
    void onlyBetter(bool checked);
    void showConnection(bool checked);
    void showGraph(bool checked);
    void cameraBack();
    void changeStatus(int status);
    void addLaunch();
    void addProtein();
    void showResult();
private:
    QMap<int,QVector<Result>> results_;
    Ui::MainWindow *ui;
    Qt3DCore::QEntity* rootEntity_;
    Qt3DExtras::Qt3DWindow *view_;
    SettingsForm *settingsForm_;
    ProteinForm *proteinForm_;
    ResultForm *resultForm_;
    QTimer *timer_;

    int currentLaunch_;
    int currentSubLaunch_;
    int countFullLaunches_;
    int currentProtein_;
    QVector<SETTINGS> launches_;
    QVector<VECTORBYTE> proteins_;

    double avarageRating_;
    int countConvolution_;
    QString timeWork_;
    QString timeBest_;

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

    void startLaunch();
};

#endif // MAINWINDOW_H

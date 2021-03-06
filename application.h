#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "core.h"
#include "mainwindow.h"

#include <QThread>

class Application : public QObject
{
    Q_OBJECT
private:
    Core *core_;
    MainWindow *mainWindow_;
public:
    explicit Application(QObject *parent = 0);
    void start();
signals:

public slots:
};

#endif // APPLICATION_H

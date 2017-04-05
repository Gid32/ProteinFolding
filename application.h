#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "core.h"
#include "scene.h"

#include <QThread>

class Application : public QObject
{
    Q_OBJECT
private:
    QThread *coreThread_;
    Core *core_;
    Scene *scene_;
    bool isStart;
public:
    explicit Application(QObject *parent = 0);
    void start();
signals:

public slots:
    void coreStart(QString netName, int method, int countAnt);
    void coreStop();
};

#endif // APPLICATION_H

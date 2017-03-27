#ifndef DEBUG_H
#define DEBUG_H

#include <QObject>
#include <QVariant>
#include <QDebug>
#include "settings.h"

class Debug : public QObject
{
    Q_OBJECT
public:
    explicit Debug(QObject *parent = 0);
    void show(QVariant var, bool isShow);
signals:

public slots:
};

#endif // DEBUG_H

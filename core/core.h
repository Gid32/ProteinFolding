#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QVector>
#include "settings.h"

#include "netfactory.h"
#include <QtCore/QObject>

#include "convolution.h"

class Core:public QObject
{
  Q_OBJECT
signals:
    void hasBetterVariant(QVector<QVector3D> vectorCoords);
    void proteinLoaded(VECTORBYTE protein);
private:
    int bestResult_;
    bool isProteinLoaded_;
    bool isBreak_;
    void loadProtein();
public:
    explicit Core();
    void setNet(QString netName);
public slots:
    void start();
    void stop();
};

#endif // CORE_H

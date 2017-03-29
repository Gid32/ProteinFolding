#ifndef NETFACTORY_H
#define NETFACTORY_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "triangle3dnet.h"
#include "trianglenet.h"
#include "quadnet.h"
#include "cubenet.h"


class NetFactory : public QObject
{
    Q_OBJECT
private:
    QMap<QString,Net*> nets;
    explicit NetFactory(QObject *parent = 0);
    static NetFactory* instance_;
public:
    static NetFactory* getInstance();
    Net* getNetByName(QString netName);
    QStringList getStringList();
signals:

public slots:
};

#endif // NETFACTORY_H

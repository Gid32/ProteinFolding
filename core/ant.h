#ifndef ANT_H
#define ANT_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "settings.h"
#include "convolutionfactory.h"

class Ant : public QThread
{
    Q_OBJECT
private:
    int count_;
public:
    void setCount(int);
    explicit Ant();
    void run();
signals:
    void convolutionCreated(Convolution*);
public slots:
};

#endif // ANT_H

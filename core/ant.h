#ifndef ANT_H
#define ANT_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "settings.h"
#include "convolution.h"

class Ant : public QThread
{
    Q_OBJECT
private:
    Convolution *convolution_;
public:
    explicit Ant();
    void run();
    void setConvolution(Convolution *convolution);
signals:
    void convolutionCreated(Convolution*,int);
    void test(int);


public slots:
};

#endif // ANT_H

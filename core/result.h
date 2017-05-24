#ifndef RESULT_H
#define RESULT_H

#include "convolution.h"
#include "settings.h"

struct SubResult
{
    Convolution convolution_;
    double averageRating_;
    QString timeWork_;
    QString timeBest_;
    int countConvolution_;
    void save(Convolution convolution,double averageRating,QString timeWork,QString timeBest,int countConvolution)
    {
       convolution_ =  convolution;
       averageRating_ = averageRating;
       timeWork_ = timeWork;
       timeBest_ = timeBest;
       countConvolution_ = countConvolution;
    }
};

class Result
{
public:
    Result();
    QVector<SubResult> results;
    SETTINGS settings;
private:
};
//резов(бест.свёртка\рейт(средн\макс)\время  работы\количество свёрток
#endif // RESULT_H

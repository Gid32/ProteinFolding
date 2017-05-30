#ifndef RESULT_H
#define RESULT_H

#include "convolution.h"
#include "settings.h"

#include <QFile>
#include <QTextStream>

class SubResult
{
private:
    Convolution convolution_;
    double averageRating_;
    QString timeWork_;
    QString timeBest_;
    int countConvolution_;
public:
    SubResult();
    SubResult(Convolution convolution,double averageRating,QString timeWork,QString timeBest,int countConvolution);
    void saveToLog(int index);
    int getTimeWork();
    int getTimeBest();
    QString getTimeWorkStr();
    QString getTimeBestStr();
    int getCountConvolution();
    double getAverageRating();
    Convolution getConvolution();
};

class Result
{
private:
    int alltime;
    int allBestTime;
    double averageResult;
    int bestResult;
    int countConv;
    QVector<SubResult> results;
public:
    Result();
    SETTINGS settings;
    void addSubResult(SubResult subResult);
    QTime getAverageTime() ;
    QTime getAverageBestTime();
    QTime getAllTime();
    int getAverageTimeInt() ;
    int getAverageBestTimeInt();
    int getAllTimeInt();
    double getAverageResult();
    int getCountSub();
    int getBestResult();
    int getCountConv();
    void saveToLog();
    void saveToLogHeader(int index);
    QVector<SubResult> getSubResults();
};
void saveToLogProteinHeader(int index,int length);
void saveToLogProteinResult(QVector<Result> results);
void getProteinResult(QVector<Result> results, int &alltime, int &allBestTime, double &averageResult, int &bestResult, int &countConv);
void saveSuperLogHeader();
void saveSuperLog(int proteinNumber, Result res, int subResIndex, int resIndex);
#endif // RESULT_H

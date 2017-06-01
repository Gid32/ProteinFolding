#include "result.h"

SubResult::SubResult()
{

}

SubResult::SubResult(Convolution convolution,double averageRating,QString timeWork,QString timeBest,int countConvolution)
{
   convolution_ =  convolution;
   averageRating_ = averageRating;
   timeWork_ = timeWork;
   timeBest_ = timeBest;
   countConvolution_ = countConvolution;
}

int SubResult::getTimeWork()
{
    return QTime(0,0,0,0).msecsTo(QTime::fromString(timeWork_,"hh:mm:ss.zzz"));
}

int SubResult::getTimeBest()
{
    return QTime(0,0,0,0).msecsTo(QTime::fromString(timeBest_,"hh:mm:ss.zzz"));
}

QString SubResult::getTimeWorkStr()
{
    return timeWork_;
}

QString SubResult::getTimeBestStr()
{
    return timeBest_;
}

int SubResult::getCountConvolution()
{
    return countConvolution_;
}

double SubResult::getAverageRating()
{
    return averageRating_;
}

Convolution SubResult::getConvolution()
{
    return convolution_;
}

void saveSuperLogHeader()
{
    QFile file("log.xls");
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << QString("№ протеїну\tMMAS\\EXP\t№ набору параметрів\t№ старту\tкращий результат\tсередній результат\tзагальний час роботи\tчас знаходження кращого результату\tкількість переглянутих результатів")<<"\n";

    file.close();
}

void saveSuperLog(int proteinNumber, Result res, int subResIndex, int resIndex)
{
    QFile file("log.xls");
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    SubResult subRes = res.getSubResults().at(subResIndex);
    int bestRes = subRes.getConvolution().result_;
    int mmasExp = res.settings.value("elitism").toInt();

    QTextStream out(&file);
    out << QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9").arg(proteinNumber+1)
           .arg(mmasExp)
           .arg(resIndex+1)
           .arg(subResIndex+1)
           .arg(bestRes)
           .arg(subRes.getAverageRating())
           .arg(subRes.getTimeWorkStr())
           .arg(subRes.getTimeBestStr())
           .arg(subRes.getCountConvolution())<<"\n";

    file.close();
}


void SubResult::saveToLog(int index)
{
    QFile file("log.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << QString("        Подзапуск № %1").arg(index) << "\n"
        << QString("            средний рейтинг: %1").arg(averageRating_) << "\n"
        << QString("            время работы: %1").arg(timeWork_) << "\n"
        << QString("            время нахождения лучшего: %1").arg(timeBest_) << "\n"
        << QString("            количество сверток: %1").arg(countConvolution_) << "\n"
        << QString("            лучший рейтинг:  %1").arg(convolution_.result_) << "\n";

    file.close();
}

void Result::saveToLog()
{
    QFile file("log.txt");
        if (!file.open(QIODevice::Append | QIODevice::Text))
            return;

    QTextStream out(&file);
    out << QString("        средний рейтинг: %1").arg(getAverageResult()) << "\n"
        << QString("        время работы: %1").arg(getAllTime().toString("hh:mm:ss.zzz")) << "\n"
        << QString("        среднее время нахождения лучшего: %1").arg(getAverageBestTime().toString("hh:mm:ss.zzz")) << "\n"
        << QString("        среднее время работы: %1").arg(getAverageTime().toString("hh:mm:ss.zzz")) << "\n"
        << QString("        количество сверток: %1").arg(countConv) << "\n"
        << QString("        лучший рейтинг:  %1").arg(bestResult) << "\n";

    file.close();
}

void Result::saveToLogHeader(int index)
{
    QFile file("log.txt");
        if (!file.open(QIODevice::Append | QIODevice::Text))
            return;

    QTextStream out(&file);
    out << QString("    Запуск № %1").arg(index) << "\n";

    file.close();
}

void saveToLogProteinHeader(int index,int length)
{
    QFile file("log.txt");
        if (!file.open(QIODevice::Append | QIODevice::Text))
            return;

    QTextStream out(&file);
    out << QString("Претеин № %1 длина: %2").arg(index).arg(length) << "\n";

    file.close();
}

void saveToLogProteinResult(QVector<Result> results)
{
    int alltime;
    int allBestTime;
    double averageResult;
    int bestResult;
    int countConv;
    getProteinResult(results, alltime, allBestTime, averageResult, bestResult, countConv);

    int msAverageBestTime = allBestTime/results.size();
    QTime averageBestTime = QTime(0,0,0,0).addMSecs(msAverageBestTime);
    int msAverageTime = alltime/results.size();
    QTime averageTime = QTime(0,0,0,0).addMSecs(msAverageTime);

    QFile file("log.txt");
        if (!file.open(QIODevice::Append | QIODevice::Text))
            return;

    QTextStream out(&file);
    out << QString("средний рейтинг: %1").arg((double)averageResult/results.size()) << "\n"
        << QString("время работы: %1").arg(QTime(0,0,0,0).addMSecs(alltime).toString("hh:mm:ss.zzz")) << "\n"
        << QString("среднее время нахождения лучшего: %1").arg(averageBestTime.toString("hh:mm:ss.zzz")) << "\n"
        << QString("среднее время работы: %1").arg(averageTime.toString("hh:mm:ss.zzz")) << "\n"
        << QString("количество сверток: %1").arg(countConv) << "\n"
        << QString("лучший рейтинг:  %1").arg(bestResult) << "\n";

    file.close();
}

void getProteinResult(QVector<Result> results, int &alltime, int &allBestTime, double &averageResult, int &bestResult, int &countConv)
{
    alltime = 0;
    allBestTime = 0;
    averageResult = 0;
    bestResult = 0;
    countConv = 0;

    for(int i=0;i<results.size();i++)
    {
        alltime += results[i].getAllTimeInt();
        allBestTime += results[i].getAverageBestTimeInt();

        int res = results[i].getBestResult();
        if(res > bestResult)
            bestResult = res;
        averageResult += results[i].getAverageResult();
        countConv += results[i].getCountConv();
    }

}

Result::Result()
{
    alltime = 0;
    allBestTime = 0;
    averageResult = 0;
    bestResult = 0;
    countConv = 0;
}

void Result::addSubResult(SubResult subResult)
{
    results.push_back(subResult);
    alltime += subResult.getTimeWork();
    allBestTime += subResult.getTimeBest();

    int res = subResult.getConvolution().result_;
    if(res > bestResult)
        bestResult = res;
    averageResult += subResult.getAverageRating();
    countConv += subResult.getCountConvolution();
}

QTime Result::getAverageTime()
{
    int msAverageTime = alltime/results.size();
    return QTime(0,0,0,0).addMSecs(msAverageTime);
}

int Result::getAverageTimeInt()
{
    return alltime/results.size();
}

QTime Result::getAverageBestTime()
{
    int msAverageBestTime = allBestTime/results.size();
    return QTime(0,0,0,0).addMSecs(msAverageBestTime);
}

int Result::getAverageBestTimeInt()
{
    return allBestTime/results.size();
}

QTime Result::getAllTime()
{
    return QTime(0,0,0,0).addMSecs(alltime);
}

int Result::getAllTimeInt()
{
    return alltime;
}

double Result::getAverageResult()
{
    return averageResult/results.size();
}

int Result::getCountSub()
{
    return results.size();
}

int Result::getBestResult()
{
    return bestResult;
}

int Result::getCountConv()
{
    return countConv;
}

QVector<SubResult> Result::getSubResults()
{
    return results;
}


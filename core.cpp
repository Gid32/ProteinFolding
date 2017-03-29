#include "core.h"
#include <ctime>
#include <windows.h>
#include <QtCore/QDebug>
#include <QTimer>
using namespace std;

Core::Core()
{
    isProteinLoaded_ = false;
}

void Core::init()
{
    for(int i=0;i<AREA_SIZE;i++)
        for(int j=0;j<AREA_SIZE;j++)
            for(int k=0;k<AREA_SIZE;k++)
                area[i][j][k] = FILL_AREA;
    history_.clear();
    currentDirection_ = 0;
}

void Core::setValueByCoord(QVector3D coord, BYTE value)
{
    area[(int)coord.x()][(int)coord.y()][(int)coord.z()] = value;
}

BYTE Core::getValueByCoord(QVector3D coord)
{
    return area[(int)coord.x()][(int)coord.y()][(int)coord.z()];
}

bool Core::isHydroFobByCoord(QVector3D coord)
{
    BYTE value = getValueByCoord(coord);
    if(value == H_FOB)
        return true;
    return false;
}

int Core::getCount(QVector3D coord,QVector3D blockCoordPrev,QVector3D blockCoordNext)
{
    int count = 0;
    for(int i=0;i<=net->getMaxTurn();i++)
    {
        QVector3D testCoord = net->getDirectionCoord(i,coord);
        if(testCoord != blockCoordPrev && testCoord != blockCoordNext)
            count += isHydroFobByCoord(testCoord);
    }
    return count;
}

void Core::debugCoord(QVector3D coord)
{
    qDebug()<<coord.x()<<" "<<coord.y()<<" "<<coord.z();
}

void Core::debugHistoryCoord()
{
    for (int i = 0; i < history_.size(); ++i)
    {
        debugCoord(history_[i].coords);
    }
}

int Core::getResult()
{
   int result = 0;
   QVector3D blockCoordPrev(START_POSITION,START_POSITION,START_POSITION);
   QVector3D blockCoordNext = history_[0].coords;
   if(isHydroFobByCoord(blockCoordPrev))
       result += getCount(blockCoordPrev, blockCoordNext, blockCoordNext);
   for (int i = 0; i < history_.size(); ++i)
   {
        if(i!=history_.size()-1)
            blockCoordNext = history_[i+1].coords;
        QVector3D coords = history_[i].coords;
        if(isHydroFobByCoord(coords))
            result += getCount(coords,blockCoordPrev,blockCoordNext);
        blockCoordPrev = coords;
   }
   return result;
}

bool Core::getNextCoordsByCurrentDirection(QVector3D &coords)
{
    QVector<QVector3D> possible;
    for(int i=net->getMinTurn();i<=net->getMaxTurn();i++)
    {
        currentDirection_ = net->turnToDirection(currentDirection_,i);
        QVector3D newCoords = net->getDirectionCoord(currentDirection_,currentCoords_);
        if(getValueByCoord(newCoords)==FILL_AREA)
            possible.push_back(newCoords);
    }
    if(possible.empty())
        return false;
    int n = rand()%possible.size();
    coords = possible.at(n);
    possible.clear();
    return true;
}

void Core::createConvolution()
{
    init();
    currentCoords_ = QVector3D(START_POSITION,START_POSITION,START_POSITION);
    setValueByCoord(currentCoords_, protein_.at(0));
    QVector3D newCoords = currentCoords_;
    int direction = currentDirection_;
    for(int i=1;i<protein_.size();i++)
    {
        if(!getNextCoordsByCurrentDirection(newCoords))//нельзя ходить
        {
            i-=2;//-2 потому что не только этот шаг не удалось построить но и еще нужно старый удалить
            History history = history_.last();
            setValueByCoord(history.coords,BLOCK_AREA);
            currentDirection_ = history.direction;
            history_.removeLast();
            currentCoords_ = history_.last().coords;
            continue;
        }
        setValueByCoord(newCoords,protein_.at(i));
        history_.push_back(History(newCoords,direction));
        currentCoords_ = newCoords;
    }
}

QVector3D Core::getCoordsRelationZeroPosition(QVector3D coords)
{
    return QVector3D(coords.x()-START_POSITION,coords.y()-START_POSITION,coords.z()-START_POSITION);
}

void Core::loadProtein()
{
    srand(time(NULL));
    for(int i=0;i<COUNT;i++)
    {
        if(rand()%2)
            protein_.push_back(H_FOB);
        else
            protein_.push_back(H_FILL);
    }
    isProteinLoaded_ = true;
    emit proteinLoaded(protein_);
}

void Core::setNet(QString netName)
{
    net = NetFactory::getInstance()->getNetByName(netName);
}

void Core::start()
{
    if(!isProteinLoaded_)
        loadProtein();
    qDebug()<<"aaa";
    if(!net)
        return;
    bestResult_ = 0;
    int i = 0;
    while(true)
    {
        qDebug()<<i;
        i++;
        createConvolution();
        int result = getResult();
        if(result > bestResult_)
        {
            bestResult_ = result;
            qDebug()<<bestResult_;
            QVector<QVector3D> vectorCoords;
            for (int i = 0; i < history_.size(); ++i)
                vectorCoords.push_back(net->getCoords(getCoordsRelationZeroPosition(history_[i].coords)));
            emit hasBetterVariant(vectorCoords);
        }
        //break;
    }
}

void Core::stop()
{
}

int Core::getElementNumByCoords(QVector3D coord)
{
    if (coord == QVector3D(COUNT,COUNT,COUNT)) return 0;
    for(int i = 0; i < COUNT-1; i++)
    {
        if(history_[i].coords == coord)
            return i+1;
    }
    return -1;//smth wrong with this shit!
}

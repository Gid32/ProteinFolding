#include "core.h"
#include <ctime>
#include <windows.h>
#include <QtCore/QDebug>
#include <QTimer>
using namespace std;

Core::Core()
{
    net = NET_TYPE;
    debug = false;
}

void Core::init()
{
    for(int i=0;i<COUNT * 2 + 1;i++)
        for(int j=0;j<COUNT * 2 + 1;j++)
            for(int k=0;k<COUNT * 2 + 1;k++)
                area[i][j][k] = FILL_AREA;
    //========================
//    for(int i=0;i<COUNT;i++)
//        numberOfLinks[i] = 0;
    //========================
    history.clear();
    currentDirection = 0;
}

bool Core::isHydroFobByCoord(QVector3D coord)
{
    int value = area[(int)coord.x()][(int)coord.y()][(int)coord.z()];
    if(value == 1)
    {
        //========================
//        int num = getElementNumByCoords(coord);

//        if(num>=0)numberOfLinks[num]++;
//        else
//        {
//            qDebug()<<"SMTH TERRIBLE WAS HAPPENED!!";
//            debugCoord(coord);
//            qDebug()<<"coordinates are not in history nor start";
//        }
//        //numberOfLinks[getElementNumByCoords(coord)]--;
        //========================
        return value;
    }
    return false;
}

int Core::getCount(QVector3D coord,QVector3D blockCoordPrev,QVector3D blockCoordNext)
{
    int count = 0;
    for(int i=0;i<=net->getMaxTurn();i++)
    {
        QVector3D testCoord = net->getDirectionCoord(i,coord);
        if(testCoord != blockCoordPrev && testCoord != blockCoordNext)
        {
            count += isHydroFobByCoord(testCoord);
            //========================
//            if (isHydroFobByCoord(testCoord))
//            {
//                int num = getElementNumByCoords(coord);

//                if(num>=0)numberOfLinks[num]--;
//                else
//                {
//                    qDebug()<<"SMTH TERRIBLE WAS HAPPENED!!";
//                    debugCoord(coord);
//                    qDebug()<<"coordinates are not in history nor start(wut? O.o)";
//                }
//            }
            //========================
            if(isHydroFobByCoord(testCoord))
            {
                if(debug)
                {
                    qDebug()<<"+1";
                    debugCoord(testCoord);
                }
            }
        }
    }
    return count;
}

void Core::debugCoord(QVector3D coord)
{
    qDebug()<<coord.x()<<" "<<coord.y()<<" "<<coord.z();
}

void Core::debugHistoryCoord()
{
    for (int i = 0; i < history.size(); ++i)
    {
        debugCoord(history[i].coord);
    }
}

int Core::getResult()
{
   int result = 0;
   int x,y,z;
   x = y = z = COUNT;
   QVector3D blockCoordPrev(x,y,z);
   QVector3D blockCoordNext = history[0].coord;
   if(isHydroFobByCoord(blockCoordPrev))
       result += getCount(blockCoordPrev, blockCoordNext, blockCoordNext);
   for (int i = 0; i < history.size(); ++i)
   {
        if(i!=history.size()-1)
            blockCoordNext = history[i+1].coord;
        QVector3D coord = history[i].coord;
        if(isHydroFobByCoord(coord))
            result += getCount(coord,blockCoordPrev,blockCoordNext);

        if(debug)
        {
            qDebug()<<i<<")";
            debugCoord(blockCoordPrev);
            debugCoord(coord);
            debugCoord(blockCoordNext);
        }

        blockCoordPrev = coord;
   }
   return result;
}

QVector<int> Core::canTurn()
{
    QVector<int> possible;
    for(int i=net->getMinTurn();i<=net->getMaxTurn();i++)
    {
        int direction = net->turnToDirection(currentDirection,i);
        QVector3D newCoord = net->getDirectionCoord(direction,currentCoords);
        if(area[(int)newCoord.x()][(int)newCoord.y()][(int)newCoord.z()]==FILL_AREA)
        {
            possible.push_back(i);
        }
    }
    return possible;
}

void Core::createConvolution()
{
    init();
    currentCoords.setX(COUNT);
    currentCoords.setY(COUNT);
    currentCoords.setZ(COUNT);
    QVector3D newCoords = currentCoords;
    area[(int)currentCoords.x()][(int)currentCoords.y()][(int)currentCoords.z()] = protein.at(0);
    int turn,direction;
    for(int i=1;i<protein.size();i++)
    {
        QVector<int> possible = canTurn();
        if(possible.empty())//нельзя ходить
        {
            //isBreak = true;
            i-=2;//-2 потому что не только этот шаг не удалось построить но и еще нужно старый удалить

            History a = history.last();
            area[(int)a.coord.x()][(int)a.coord.y()][(int)a.coord.z()] = BLOCK_AREA;
            currentDirection = a.direction;

            history.removeLast();
            currentCoords = history.last().coord;
            continue;
        }
        else
        {
           turn = rand()%possible.size();
           turn = possible.at(turn);
           direction = net->turnToDirection(currentDirection,turn);
           newCoords = net->getDirectionCoord(direction,currentCoords);
        }
        area[(int)newCoords.x()][(int)newCoords.y()][(int)newCoords.z()] = protein.at(i);
        history.push_back(History(newCoords,turn,currentDirection));
        currentDirection = direction;
        currentCoords = newCoords;
    }
}

void Core::start()
{
    srand(time(NULL));
    for(int i=0;i<COUNT;i++)
        protein.push_back(rand()%2);
    bestResult = 0;
    isBreak = false;
    emit proteinLoaded(protein);
    int test = 0;
    while(true)
    {
        if(isBreak)
            break;
        //qDebug()<<"start conv";
        createConvolution();

        int result = getResult();
        if(result%2==1)
        {
            qDebug()<<"hernya"<<result;
            debug = true;
            result = getResult();
            qDebug()<<result;
            qDebug()<<"----------------------------";
            debugHistoryCoord();
            QVector<QVector3D> coords;
            for (int i = 0; i < history.size(); ++i)
            {
                QVector3D coord(history[i].coord.x()-COUNT,history[i].coord.y()-COUNT,history[i].coord.z()-COUNT);
                coords.push_back(net->getCoords(coord));
            }
            emit hasBetterVariant(coords);
            break;
        }
        if(result > bestResult)
        {
            bestResult = result;
            qDebug()<<bestResult;
            QVector<QVector3D> coords;
            for (int i = 0; i < history.size(); ++i)
            {
                QVector3D coord(history[i].coord.x()-COUNT,history[i].coord.y()-COUNT,history[i].coord.z()-COUNT);
                coords.push_back(net->getCoords(coord));
            }
            emit hasBetterVariant(coords);
        }
        //break;
    }
}


int Core::getElementNumByCoords(QVector3D coord)
{
    if (coord == QVector3D(COUNT,COUNT,COUNT)) return 0;
    for(int i = 0; i < COUNT-1; i++)
    {
        if(history[i].coord == coord) return i+1;
    }
    return -1;//smth wrong with this shit!
}

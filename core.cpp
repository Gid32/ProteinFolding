#include "core.h"
#include <ctime>
#include <windows.h>
#include <QtCore/QDebug>
#include <QTimer>
#include "net.cpp"
#include "trianglenet.cpp"
using namespace std;

Core::Core()
{
    net = new TriangleNet();
}

void Core::init()
{
    for(int i=0;i<COUNT * 2 - 1;i++)
        for(int j=0;j<COUNT * 2 - 1;j++)
            area[i][j] = FILL_AREA;
    vectorTurn.clear();
    history.clear();
    currentDirection = 0;
    blockTurn = BLOCK_AREA;
}

int Core::getResult()
{
   bestResult = 0;
   return bestResult+1;
}

QVector<int> Core::canTurn()
{
    QVector<int> possible;
    for(int i=net->getMinTurn();i<=net->getMaxTurn();i++)
    {
        int direction = net->turnToDirection(currentDirection,i);//turnToDirection(currentDirection,i);
        Coord newCoord = net->getDirectionCoord(direction,currentCoords);//getDirectionCoord(direction,currentCoords);
        if(area[newCoord.x][newCoord.y]==FILL_AREA && blockTurn != i)
        {
            qDebug()<<i<<" "<<blockTurn;
            possible.push_back(i);
        }
    }
    QString str;
    str.clear();
    if (DEBUG_CORE)
    {
//      posible turns
        for(int i=0;i<possible.size();i++)
            str.push_back(QString::number(possible.at(i))+" ");
        qDebug()<< "posible turns: " + str;
    }
    return possible;
}

QVector<int> Core::createConvolution()
{
    init();
    Coord newCoord;
    currentCoords.x = COUNT/2;
    currentCoords.y = COUNT/2;
    area[currentCoords.x][currentCoords.y] = protein.at(0);
    int turn,direction;
    QVector<int> currentVectorTurn;
    for(int i=1;i<protein.size();i++)
    {
        QVector<int> possible = canTurn();
        if(possible.empty())//нельзя ходить
        {
            //isBreak = true;
            i-=2;//-2 потому что не только этот шаг не удалось построить но и еще нужно старый удалить
            area[history.last().coord.x][history.last().coord.y] = BLOCK_AREA;
            blockTurn = history.last().turn;
            currentDirection = history.last().direction;
            //qDebug()<<"pzdc block "<<blockTurn<<" direction "<<direction;
            history.removeLast();
            currentVectorTurn.removeLast();
            currentCoords = history.last().coord;
            continue;
        }
        else
        {
           turn = rand()%possible.size();
           turn = possible.at(turn);
           direction = net->turnToDirection(currentDirection,turn);//turnToDirection(currentDirection,turn);
           newCoord = net->getDirectionCoord(direction,currentCoords);//getDirectionCoord(direction,currentCoords);
           //qDebug()<<direction;
           blockTurn = BLOCK_AREA;
        }
        area[newCoord.x][newCoord.y] = protein.at(i);
        currentVectorTurn.push_back(turn);
        history.push_back(History(newCoord,turn,currentDirection));
        currentDirection = direction;
        currentCoords = newCoord;
    }
    if (DEBUG_CORE)
    {
        qDebug()<<"area";
        for(int i=0;i<COUNT;i++)
        {
            QString str;
            str.clear();
            for(int j=0;j<COUNT;j++)
                str.push_back(QString::number(area[i][j]));
            qDebug()<<str;
        }
        qDebug()<<"turn vector";
        for(int i=0;i<currentVectorTurn.size();i++)
        {
            qDebug()<<"turn "<<i<<": "<<currentVectorTurn.at(i);
        }
    }
    return currentVectorTurn;
}

void Core::start()
{
    srand(time(NULL));
    for(int i=0;i<COUNT;i++)
        protein.push_back(rand()%2);
    bestResult = 0;
    isBreak = false;
    emit proteinLoaded(protein);
    while(true)
    {
        if(isBreak)
            break;
        QVector<int> currentVectorTurn = createConvolution();

        int result = getResult();
        if(result > bestResult)
        {
            bestResult = result;
            vectorTurn = currentVectorTurn;
            emit hasBetterVariant(getvectorDirection(vectorTurn));
        }
    }
}


QVector<int> Core::getvectorDirection(QVector<int >vectorTurn)
{
    QVector<int> vectorDirection;
    if(vectorTurn.size()==0)
        return vectorDirection;
    vectorDirection.push_back(net->turnToDirection(0,vectorTurn.at(0)));
    for(int i=1;i<vectorTurn.size();i++)
        vectorDirection.push_back(net->turnToDirection(vectorDirection.last(),vectorTurn.at(i)));
    return vectorDirection;
}

#include "core.h"
#include <ctime>
#include <windows.h>
#include <QtCore/QDebug>
#include <QTimer>
using namespace std;

Coord Core::getDirectionCoord(int direction, Coord c)
{
    switch(direction)
    {
    case 0:c.x--;
        break;
    case 1:c.x++;
        break;
    case 2:c.y--;
        break;
    case 3:c.y++;
        break;
    case 4:c.x++;c.y--;
        break;
    case 5:c.x--;c.y++;
        break;
    }
    return c;
}

Core::Core()
{

}

void Core::init()
{
    for(int i=0;i<COUNT;i++)
        for(int j=0;j<COUNT;j++)
            area[i][j] = 5;
    vectorTurn.clear();
    history.clear();
    currentDirection = 0;
    blockTurn = -100;
}

int Core::getResult()
{
   bestResult = 0;
   return bestResult+1;
}

QVector<int> Core::canTurn()
{
    QVector<int> possible;
    for(int i=MIN_TURN;i<=MAX_TURN;i++)
    {
        int direction = turnToDirection(currentDirection,i);
        Coord newCoord = getDirectionCoord(direction,currentCoords);
        if(area[newCoord.x][newCoord.y]==5 && blockTurn != i)
            possible.push_back(i);
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
            area[history.last().coord.x][history.last().coord.y] = 7;
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
           direction = turnToDirection(currentDirection,turn);
           newCoord = getDirectionCoord(direction,currentCoords);
           //qDebug()<<direction;
           blockTurn = -100;
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
            qDebug()<<currentVectorTurn.at(i);
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

int Core::turnToDirection(int currentDirection,int turn)
{
    int direction = 0;

    direction = (currentDirection+turn)%COUNT_DIRECTION;
    if(direction<0)
        direction += COUNT_DIRECTION;
    return direction;
}

QVector<int> Core::getvectorDirection(QVector<int >vectorTurn)
{
    QVector<int> vectorDirection;
    if(vectorTurn.size()==0)
        return vectorDirection;
    vectorDirection.push_back(turnToDirection(0,vectorTurn.at(0)));
    for(int i=1;i<vectorTurn.size();i++)
        vectorDirection.push_back(turnToDirection(vectorDirection.last(),vectorTurn.at(i)));
    return vectorDirection;
}

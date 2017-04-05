#include "convolution.h"
Net* Convolution::net = nullptr;
QVector<BYTE> Convolution::protein = QVector<BYTE>();
int Convolution::method = 0;

Convolution::Convolution(QObject *parent) : QObject(parent)
{
    history_.clear();
    currentDirection_ = 0;

    currentCoords_ = QVector3D(START_POSITION,START_POSITION,START_POSITION);//начальная координата в массиве откуда начинается свертка
    setValueByCoord(currentCoords_, protein.at(0));//записываем в массив первую аминокислоту
    currentTurnNumber_ = 1;
    create();
}

Convolution::Convolution(Convolution *convolution, int turnNumber) : QObject(0)
{
    //turnNumber начинается с 0
    history_.clear();
    historyPrev_ = convolution->getHistory();
    currentDirection_ = historyPrev_.at(turnNumber).direction;
    if(turnNumber==0)
        currentCoords_ = QVector3D(START_POSITION,START_POSITION,START_POSITION);
    else
        currentCoords_ = historyPrev_.at(turnNumber-1).coords;

    setValueByCoord(QVector3D(START_POSITION,START_POSITION,START_POSITION), protein.at(0));//записываем в массив первую аминокислоту
    for(int i = 0;i < turnNumber;i++)
    {
        history_.append(historyPrev_[i]);
        setValueByCoord(historyPrev_[i].coords, protein.at(0));
    }
    setValueByCoord(historyPrev_[turnNumber].coords, BLOCK_AREA);//делаем невозможными повтор свертки
    currentTurnNumber_ = turnNumber+1;//+1 потому что currentTurnNumber_ начинается с 1
    create();
}

QVector<History> Convolution::getHistory()
{
    return history_;
}

Convolution::~Convolution()
{

}

void Convolution::create()
{
    QVector3D newCoords;
    int direction;
    int turn;
    while(currentTurnNumber_!=protein.size())
    {
        if(!getNext(newCoords,direction,turn))//нельзя ходить
        {
            deadEnd();
            continue;
        }
        setValueByCoord(newCoords,protein.at(currentTurnNumber_));
        history_.push_back(History(newCoords,currentDirection_,turn));//в историю добавляем координату и дайрекшин который был перед этим
        currentDirection_ = direction;
        currentCoords_ = newCoords;
        currentTurnNumber_++;
    }
}

int Convolution::randomMethod(QVector<QVector3D> possible)
{
    int n = qrand()%possible.size();
    return n;
}

int Convolution::withoutProbabilistic(QVector<QVector3D> possible)
{
    int indexMax = 0;
    int count = 0;
    if(history_.size()==0 || possible.size()==1)
       return 0;
    for(int i=0;i<possible.size();i++)
    {
        int localCount = getCount(possible.at(i),history_.last().coords,history_.last().coords);
        if(localCount > count)
        {
            count = localCount;
            indexMax = i;
        }
    }
    return indexMax;
}

int Convolution::probabilistic(QVector<QVector3D> possible)
{
    QVector<int> chances;
    int count = 0;
    if(history_.size()==0 || possible.size()==1)
       return  0;
    for(int i=0;i<possible.size();i++)
    {
        int localCount = getCount(possible.at(i),history_.last().coords,history_.last().coords);
        chances.push_back(count);
        count+=localCount;
    }
    int n = qrand()%(count+1);
    for(int i=0;i<chances.size();i++)
    {
        if(n<=chances.at(i))
            return i;
    }
    return 0;
}

void Convolution::deadEnd()
{
    currentTurnNumber_-=2;//-2 потому что не только этот шаг не удалось построить но и еще нужно старый удалить
    History history = history_.last();
    setValueByCoord(history.coords,BLOCK_AREA);
    currentDirection_ = history.direction;
    history_.removeLast();
    currentCoords_ = history_.last().coords;
}

bool Convolution::addNewTurn(int turn,QVector3D &coords, int &direction)
{
    direction = net->turnToDirection(currentDirection_,turn);
    coords = net->getDirectionCoord(direction,currentCoords_);
    if(getValueByCoord(coords)==FILL_AREA)
        return true;
    return false;
}

bool Convolution::getNext(QVector3D &coords, int &direction,int &turn)
{
    QVector<QVector3D> possible;
    QVector<int> turns;
    QVector<int> directions;
    bool f = false;
//    if(!historyPrev_.empty())
//    {
//        if(addNewTurn(historyPrev_.at(currentTurnNumber_).turn,coords,direction))
//        {
//            possible.push_back(coords);
//            turns.push_back(historyPrev_.at(currentTurnNumber_).turn);
//            directions.push_back(direction);
//            f = true;
//            qDebug()<<coords;
//        }
//    }
    if(!f)
    {
        for(int i=net->getMinTurn();i<=net->getMaxTurn();i++)
        {
            if(addNewTurn(i,coords,direction))
            {
                possible.push_back(coords);
                turns.push_back(i);
                directions.push_back(direction);
            }
        }
    }

    if(possible.empty())
        return false;

    int n;
    if(method == 0)
        n = randomMethod(possible);
    else if(method == 1)
        n = withoutProbabilistic(possible);
    else
        n = probabilistic(possible);

    coords = possible.at(n);
    turn = turns.at(n);
    direction = directions.at(n);

    possible.clear();
    turns.clear();
    return true;
}

void Convolution::setValueByCoord(QVector3D coord, BYTE value)
{
    area.insert(coordToQString(coord),value);
}

BYTE Convolution::getValueByCoord(QVector3D coord)
{
    QString str = coordToQString(coord);
    if(area.contains(str))
        return area[str];
    return FILL_AREA;
}

QString Convolution::coordToQString(QVector3D coord)
{
    QString str = "";
    str.append(QString::number(coord.x()));
    str.append(" ");
    str.append(QString::number(coord.y()));
    str.append(" ");
    str.append(QString::number(coord.z()));
    return str;
}

QVector3D Convolution::getCoordsRelationZeroPosition(QVector3D coords)
{
    return QVector3D(coords.x()-START_POSITION,coords.y()-START_POSITION,coords.z()-START_POSITION);
}

bool Convolution::isHydroFobByCoord(QVector3D coord)
{
    BYTE value = getValueByCoord(coord);
    if(value == H_FOB)
        return true;
    return false;
}

int Convolution::getCount(QVector3D coord,QVector3D blockCoordPrev,QVector3D blockCoordNext)
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

int Convolution::getResult()
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

QVector<QVector3D> Convolution::getVectorCoords()
{
    QVector<QVector3D> vectorCoords;
    for (int i = 0; i < history_.size(); ++i)
        vectorCoords.push_back(net->getCoords(getCoordsRelationZeroPosition(history_[i].coords)));
    return vectorCoords;
}




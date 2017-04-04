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
    qDebug()<<"--"<<turnNumber;
    history_.clear();
    QVector<History> historyVect = convolution->getHistory();
    currentDirection_ = historyVect.at(turnNumber).direction;
    //qDebug()<<"size:"<<historyVect.size();
    if(turnNumber==0)
        currentCoords_ = QVector3D(START_POSITION,START_POSITION,START_POSITION);
    else
        currentCoords_ = historyVect.at(turnNumber-1).coords;

    setValueByCoord(QVector3D(START_POSITION,START_POSITION,START_POSITION), protein.at(0));//записываем в массив первую аминокислоту
    for(int i = 0;i < turnNumber-1;i++)
    {
        history_.append(historyVect[i]);
        setValueByCoord(historyVect[i].coords, protein.at(0));
    }
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
    QVector3D newCoords = currentCoords_;
    int direction = currentDirection_;
    for(int i=currentTurnNumber_;i<protein.size();i++)
    {
        if(!getNext(newCoords,direction))//нельзя ходить
        {
            i-=2;//-2 потому что не только этот шаг не удалось построить но и еще нужно старый удалить
            History history = history_.last();
            setValueByCoord(history.coords,BLOCK_AREA);
            currentDirection_ = history.direction;
            history_.removeLast();
            currentCoords_ = history_.last().coords;
            continue;
        }
        setValueByCoord(newCoords,protein.at(i));
        history_.push_back(History(newCoords,currentDirection_));//в историю добавляем координату и дайрекшин который был перед этим
        currentDirection_ = direction;
        currentCoords_ = newCoords;
    }
}

QVector3D Convolution::randomMethod(QVector<QVector3D> possible)
{
    int n = qrand()%possible.size();
    return  possible.at(n);
}

QVector3D Convolution::withoutProbabilistic(QVector<QVector3D> possible)
{
    int indexMax = 0;
    int count = 0;
    if(history_.size()==0)
       return  possible.at(indexMax);
    for(int i=0;i<possible.size();i++)
    {
        int localCount = getCount(possible.at(i),history_.last().coords,history_.last().coords);
        if(localCount > count)
        {
            count = localCount;
            indexMax = i;
        }
    }
    return  possible.at(indexMax);
}

QVector3D Convolution::probabilistic(QVector<QVector3D> possible)
{
    return randomMethod(possible);
    //to do
//    int indexMax = 0;
//    int count = 0;
//    if(history_.size()==0)
//       return  possible.at(indexMax);
//    for(int i=0;i<possible.size();i++)
//    {
//        int localCount = getCount(possible.at(i),history_.last().coords,history_.last().coords);
//        if(localCount > count)
//        {
//            count = localCount;
//            indexMax = i;
//        }
//    }
//    return  possible.at(indexMax);
}


bool Convolution::getNext(QVector3D &coords, int &direction)
{
    QVector<QVector3D> possible;
    for(int i=net->getMinTurn();i<=net->getMaxTurn();i++)
    {
        direction = net->turnToDirection(direction,i);
        QVector3D newCoords = net->getDirectionCoord(direction,currentCoords_);
        if(getValueByCoord(newCoords)==FILL_AREA)
            possible.push_back(newCoords);
    }
    if(possible.empty())
        return false;
    if(method == 0)
        coords = randomMethod(possible);
    else if(method == 1)
        coords = withoutProbabilistic(possible);
    else
        coords = probabilistic(possible);
    possible.clear();
    return true;
}

void Convolution::setValueByCoord(QVector3D coord, BYTE value)
{
    area.insert(coordToQString(coord),value);
    //area[(int)coord.x()][(int)coord.y()][(int)coord.z()] = value;
}

BYTE Convolution::getValueByCoord(QVector3D coord)
{
    QString str = coordToQString(coord);
    if(area.contains(str))
    {
        return area[str];
    }
    return FILL_AREA;
    //return area[(int)coord.x()][(int)coord.y()][(int)coord.z()];
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




#include "convolution.h"
Net* Convolution::net = nullptr;
QVector<BYTE> Convolution::protein = QVector<BYTE>();
Convolution::Convolution(QObject *parent) : QObject(parent)
{
    history_.clear();
    currentDirection_ = 0;

    currentCoords_ = QVector3D(START_POSITION,START_POSITION,START_POSITION);//начальная координата в массиве откуда начинается свертка
    setValueByCoord(currentCoords_, protein.at(0));//записываем в массив первую аминокислоту

    create();
}

Convolution::~Convolution()
{

}

void Convolution::create()
{
    QVector3D newCoords = currentCoords_;
    int direction = currentDirection_;
    for(int i=1;i<protein.size();i++)
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
    int n = rand()%possible.size();
    coords = possible.at(n);
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




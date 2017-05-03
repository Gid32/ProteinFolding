#include "convolution.h"

Convolution::Convolution()
{
    result_ = -1;
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

void Convolution::setValueByCoord(QVector3D coord, BYTE value)
{
    area_.insert(coordToQString(coord),value);
}

BYTE Convolution::getValueByCoord(QVector3D coord)
{
    QString str = coordToQString(coord);
    if(area_.contains(str))
        return area_[str];
    return FILL_AREA;
}

bool Convolution::isHydroFobByCoord(QVector3D coord)
{
    BYTE value = getValueByCoord(coord);
    if(value == H_FOB)
        return true;
    return false;
}



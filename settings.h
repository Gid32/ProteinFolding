#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVector3D>
#include <QDebug>
#include <QMap>

const float RADIUS_NODE 0.2;
const float RADIUS_CONNECTION 0.05;
const float LENGTH_CONNECTION 0.9;
const double DISTANCE_NODE 1.0;

const double PI 3.14159265;

#define H_FILL 0x0
#define H_FOB 0x1
#define FILL_AREA 0x2
#define BLOCK_AREA 0x3

#define H_FILL_COLOR QColor(0,0,255,255)
#define H_FOB_COLOR QColor(255,0,0,255)
#define H_FOB_COLOR_FIRST QColor(255,0,100,255)
#define H_FILL_COLOR_FIRST QColor(0,200,255,255)
#define DEFAULT_COLOR_CONNECTION QColor(QRgb(0x928327))

typedef unsigned char BYTE;
typedef QVector<BYTE> VECTORBYTE;
typedef QMap<QString,QVariant> SETTINGS;

struct CONNECTION
{
    QVector3D first;
    QVector3D second;
    CONNECTION(QVector3D first,QVector3D second):first(first),second(second){}
    CONNECTION(){}
};

typedef QVector<CONNECTION> CONNECTIONS;

#endif // SETTINGS_H


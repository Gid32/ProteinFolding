#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVector>
#include <QDebug>
#include <QMap>

#define RADIUS_NODE 0.2f
#define RADIUS_CONNECTION 0.05f
#define LENGTH_CONNECTION 0.9f
#define DISTANCE_NODE 1.0

#define PI 3.14159265

#define MAX_ANT 200

#define H_FILL 0x0
#define H_FOB 0x1
#define FILL_AREA 0x2
#define BLOCK_AREA 0x3


typedef unsigned char BYTE;
typedef QVector<BYTE> VECTORBYTE;
typedef QMap<QString,QVariant> SETTINGS;

#endif // SETTINGS_H


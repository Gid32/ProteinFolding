#ifndef SETTINGS_H
#define SETTINGS_H

#include <QVector>

#define RADIUS_NODE 0.2f
#define RADIUS_CONNECTION 0.05f
#define LENGTH_CONNECTION 0.9f
#define DISTANCE_NODE 1.0

#define PI 3.14159265

#define COUNT 10
#define COUNT_ANT 1

#define H_FILL 0x0
#define H_FOB 0x1
#define FILL_AREA 0x2
#define BLOCK_AREA 0x3

#define AREA_SIZE COUNT*2+1
#define START_POSITION COUNT

typedef unsigned char BYTE;
typedef QVector<BYTE> VECTORBYTE;


#endif // SETTINGS_H


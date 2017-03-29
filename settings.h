#ifndef SETTINGS_H
#define SETTINGS_H

#define RADIUS_NODE 0.2f
#define RADIUS_CONNECTION 0.05f
#define LENGTH_CONNECTION 0.9f
#define DISTANCE_NODE 1.0

#define PI 3.14159265

#define COUNT 20
#define DEBUG_NODE false
#define DEBUG_CORE false
#define DEBUG_SM false
#define DEBUG_GLOBAL false

#define H_FILL 0x0
#define H_FOB 0x1
#define FILL_AREA 0x2
#define BLOCK_AREA 0x3


#define NET_TYPE Triangle2D
#define Square2D new QuadNet()
#define Square3D new CubeNet()
#define Triangle2D new TriangleNet()
#define Triangle3D new Triangle3DNet()

#endif // SETTINGS_H


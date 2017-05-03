#ifndef Connection_H
#define Connection_H

#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongAlphaMaterial>
#include <Qt3DExtras/QPhongMaterial>
#include <cmath>
#include "settings.h"

class Connection:public QObject
{
    Q_OBJECT
private:
    Qt3DExtras::QCylinderMesh *mesh;
    Qt3DCore::QTransform *transform;
    Qt3DExtras::QPhongAlphaMaterial *material;
    Qt3DCore::QEntity *obj;
public:
    explicit Connection(Qt3DCore::QEntity *parent, QVector3D vect, float radius = RADIUS_CONNECTION, float length = LENGTH_CONNECTION);
    ~Connection();
    void changeLocation(QVector3D,QVector3D);
    void changeColor(QColor color);
};

#endif // Connection_H

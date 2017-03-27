#ifndef NODE_H
#define NODE_H

#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include "connection.h"


class Node:public QObject
{
    Q_OBJECT
private:
    Qt3DExtras::QSphereMesh *mesh;
    Qt3DCore::QEntity *sphera;
    Qt3DExtras::QPhongMaterial *material;
    Qt3DCore::QEntity *parent;
    Connection *connection;
public:
    Node *prevNode;
    Qt3DCore::QTransform *transform;
    explicit Node(Qt3DCore::QEntity *parent, bool hFob, QVector3D vect, Node *prevNode);
    ~Node();
    void changeLocation(QVector3D coords);
};

#endif // NODE_H

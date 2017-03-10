#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H
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

#include "node.h"
#include "connection.h"
#include "core.h"

class SceneModifier : public QObject
{
    Q_OBJECT

public:
    explicit SceneModifier(Qt3DCore::QEntity *rootEntity);
    ~SceneModifier();
public slots:
    void update(QVector<int> vectorDirection);
    void genericNodes(QVector<bool> protein);
    void reDraw();
private:
    bool hasVar;
    void clear();
    QVector<int> vectorDirection;
    QVector<Node*> nodes;
    Qt3DCore::QEntity *rootEntity;
};

#endif // SCENEMODIFIER_H


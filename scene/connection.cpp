#include "connection.h"


Connection::Connection(Qt3DCore::QEntity *parent, QVector3D vect, float radius, float length)
{
    mesh = new Qt3DExtras::QCylinderMesh();
    mesh->setRadius(radius);
    mesh->setLength(length);
    mesh->setRings(5);
    mesh->setSlices(5);

    transform= new Qt3DCore::QTransform();
    transform->setScale(1.0f);
    transform->setTranslation(vect);

    material = new Qt3DExtras::QPhongAlphaMaterial();
    material->setDiffuse(DEFAULT_COLOR_CONNECTION);
    material->setAlpha(1.0f);

    obj = new Qt3DCore::QEntity(parent);
    obj->addComponent(mesh);
    obj->addComponent(material);
    obj->addComponent(transform);
}

Connection::~Connection()
{
    delete material;
    delete transform;
    delete mesh;
    delete obj;
}

void Connection::changeColor(QColor color)
{
    obj->removeComponent(material);
    delete material;
    material = new Qt3DExtras::QPhongAlphaMaterial();
    material->setDiffuse(color);
    material->setAlpha(0.3f);
    obj->addComponent(material);
}

void Connection::changeLocation(QVector3D vect1,QVector3D vect2)
{
    obj->removeComponent(transform);
    delete transform;
    transform = new Qt3DCore::QTransform();
    transform->setScale(1.0f);
    transform->setRotation(QQuaternion::rotationTo(QVector3D(0,1,0),vect2 - vect1));
    transform->setTranslation((vect1 + vect2)/2);

    obj->addComponent(transform);
}

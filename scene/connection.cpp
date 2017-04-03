#include "connection.h"


Connection::Connection(Qt3DCore::QEntity *parent, QVector3D vect)
{
    mesh = new Qt3DExtras::QCylinderMesh();
    mesh->setRadius(RADIUS_CONNECTION);
    mesh->setLength(LENGTH_CONNECTION);
    mesh->setRings(10);
    mesh->setSlices(10);

    transform= new Qt3DCore::QTransform();
    transform->setScale(1.0f);
    transform->setTranslation(vect);

    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0x928327)));

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

#include "connection.h"


Connection::Connection(Qt3DCore::QEntity *parent, QVector3D vect)
{
    mesh = new Qt3DExtras::QCylinderMesh();
    mesh->setRadius(RADIUS_CONNECTION);
    mesh->setLength(LENGTH_CONNECTION);
    mesh->setRings(2);
    mesh->setSlices(4);

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
    QVector3D vect = vect2;
    obj->removeComponent(transform);
    delete transform;
    transform = new Qt3DCore::QTransform();
    transform->setScale(1.0f);
    if(vect1.x()!=vect2.x())
    {
        vect.setX((vect1.x()+vect2.x())/2);
        transform->setRotationZ(90.0f);

    }
    else if(vect1.y()!=vect2.y())
    {
        vect.setY((vect1.y()+vect2.y())/2);
        transform->setRotationY(90.0f);
    }
    transform->setTranslation(vect);
    obj->addComponent(transform);
}

#include "node.h"
#include <QtCore/QDebug>

Node::Node(Qt3DCore::QEntity *parent, BYTE hFob, QVector3D vect, Node *prevNode, float radius, float connectionRadius, float connectionLength)
{

    this->parent = parent;
    this->hFob = hFob;
    QColor color;

    if(!prevNode && hFob == H_FILL)
        color = H_FILL_COLOR_FIRST;
    else if(!prevNode && hFob == H_FOB)
        color = H_FOB_COLOR_FIRST;
    else if(hFob == H_FOB)
        color = H_FOB_COLOR;
    else
        color = H_FILL_COLOR;


    mesh = new Qt3DExtras::QSphereMesh();
    mesh->setRings(8);
    mesh->setSlices(8);
    mesh->setRadius(radius);

    transform = new Qt3DCore::QTransform();
    transform->setScale(1.0f);
    transform->setTranslation(vect);

    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(color);

    sphera = new Qt3DCore::QEntity(parent);
    sphera->addComponent(mesh);
    sphera->addComponent(material);
    sphera->addComponent(transform);

    this->prevNode = prevNode;
    if(prevNode)
        connection = new Connection(parent,vect,connectionRadius,connectionLength);
    else
        connection = nullptr;

}

Node::~Node()
{
    delete transform;
    delete material;
    delete mesh;
    delete sphera;
    if(connection)
        delete connection;
}

void Node::changeLocation(QVector3D coords)
{
    transform->setTranslation(coords);
    if(connection)
        connection->changeLocation(prevNode->transform->translation(),coords);
}


#include "node.h"
#include <QtCore/QDebug>

Node::Node(Qt3DCore::QEntity *parent, BYTE hFob, QVector3D vect, Node *prevNode, float radius)
{

    this->parent = parent;
    QColor color = QColor(0,0,255,255);
    if(hFob == H_FOB)
        color = QColor(255,0,0,255);
    if(!prevNode && hFob == H_FILL)
        color = QColor(0,200,255,255);
    if(!prevNode && hFob == H_FOB)
        color = QColor(255,0,100,255);

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
        connection = new Connection(parent,vect);
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
    this->transform->setTranslation(coords);
    if(this->connection)
        this->connection->changeLocation(prevNode->transform->translation(),coords);
}


#include "node.h"
#include <QtCore/QDebug>

Node::Node(Qt3DCore::QEntity *parent, bool hFob, QVector3D vect, Node *prevNode)
{

    this->parent = parent;
    QColor color = QColor(0,0,255,255);
    if(hFob)
        color = QColor(255,0,0,255);
    if(!prevNode)
        color = QColor(255,255,0,255);
    mesh = new Qt3DExtras::QSphereMesh();
    mesh->setRings(2);
    mesh->setSlices(4);
    mesh->setRadius(RADIUS_NODE);

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

void Node::changeLocation(int direction)
{
    QVector3D coords = prevNode->transform->translation();
    switch(direction)
    {
    case 0:coords.setX(coords.x()-DISTANCE_NODE);
        break;
    case 1:coords.setX(coords.x()+DISTANCE_NODE);
        break;
    case 2:coords.setY(coords.y()+DISTANCE_NODE);
        break;
    case 3:coords.setY(coords.y()-DISTANCE_NODE);
        break;
//    case 4:coords.setY(coords.y()-DISTANCE_NODE);coords.setX(coords.x()+DISTANCE_NODE);
//        break;
//    case 5:coords.setY(coords.y()+DISTANCE_NODE);coords.setX(coords.x()-DISTANCE_NODE);
//        break;
    default:
        break;
    }
//    qDebug()<<coords.x();
//    coords.setX(coords.x()*0.8660254037844);
//    coords.setY(coords.y()+coords.x()/2);

    this->transform->setTranslation(coords);

    if(this->connection)
        this->connection->changeLocation(prevNode->transform->translation(),coords);
}


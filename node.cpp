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
    mesh->setRings(10);
    mesh->setSlices(10);
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
//  back to int
    float y = coords.y()/0.8660254037844;
    float x = coords.x() - y/2;

    if (DEBUG_NODE)
    {
        qDebug() << direction;
        qDebug() << " (" <<
                    (prevNode->transform->translation().x()) << ", " <<
                    (prevNode->transform->translation().y()) << ")";
        qDebug() << " (" << x << ", " << y << ")";
    }

    switch(direction)
    {
    case 0:x-=DISTANCE_NODE;
        break;
    case 1:x+=DISTANCE_NODE;
        break;
    case 2:y-=DISTANCE_NODE;
        break;
    case 3:y+=DISTANCE_NODE;
        break;
    case 4:
    {
        x+=DISTANCE_NODE;
        y-=DISTANCE_NODE;
    }
        break;
    case 5:
    {
        x-=DISTANCE_NODE;
        y+=DISTANCE_NODE;
    }
        break;
    default:
        break;
    }

//  go to float
    coords.setX(x + y/2.0);
    coords.setY(y * 0.8660254037844);

    if (DEBUG_NODE)
    {
        qDebug() << " (" << x << ", " << y << ")";
        qDebug() << " (" << coords.x() << ", " << coords.y() << ")";
    }
    this->transform->setTranslation(coords);


    if(this->connection)
        this->connection->changeLocation(prevNode->transform->translation(),coords);
}


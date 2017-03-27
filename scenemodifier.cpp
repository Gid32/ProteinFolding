#include "scenemodifier.h"
#include <QtCore/QDebug>
#include <QTimer>
#include <windows.h>

void SceneModifier::clear()
{
    for(int i=0;i<nodes.size();i++)
    {
        delete nodes.at(i);
    }
    nodes.clear();
}

void SceneModifier::genericNodes(QVector<bool> protein)
{
    Node *node = nullptr;
    for(int i=0;i<protein.size();i++)
    {
        nodes.push_back(new Node(rootEntity,protein.at(i),QVector3D(0.0f, 0.0f, 0.0f),node));
        node = nodes.last();
    }
}

void SceneModifier::reDraw()
{
    if(!hasVar)
        return;
    if (DEBUG_SM) qDebug() << "--------------";
    for(int i=1;i<nodes.size();i++)//0 нод не трогаем (повороты начинаются с первого)
    {
        if(vectorCoords.size()==COUNT-1)
            nodes.at(i)->changeLocation(vectorCoords.at(i-1));
    }
    hasVar = false;
}

SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity)
    : rootEntity(rootEntity)
{
    hasVar = false;

    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(reDraw()));
    timer->start(1000);
}

void SceneModifier::update(QVector<QVector3D> vectorCoords)
{
    this->vectorCoords = vectorCoords;
    hasVar = true;
}

SceneModifier::~SceneModifier()
{
}




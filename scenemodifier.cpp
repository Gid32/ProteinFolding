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
        nodes.push_back(new Node(rootEntity,protein.at(i),QVector3D(2.0f, 2.0f, 0.0f),node));
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
        if(vectorDirection.size()==COUNT-1)
            nodes.at(i)->changeLocation(vectorDirection.at(i-1));
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

void SceneModifier::update(QVector<int> vectorDirection)
{
    this->vectorDirection = vectorDirection;
    hasVar = true;
}

SceneModifier::~SceneModifier()
{
}




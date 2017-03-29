#include "netfactory.h"

NetFactory* NetFactory::instance_ = nullptr;

NetFactory::NetFactory(QObject *parent) : QObject(parent)
{
    nets.insert("Triangle3DNet", new Triangle3DNet());
    nets.insert("TriangleNet", new TriangleNet());
    nets.insert("QuadNet", new QuadNet());
    nets.insert("CubeNet", new CubeNet());
}

Net* NetFactory::getNetByName(QString netName)
{
    QMap<QString,Net*>::iterator netIt = nets.find(netName);
    if(netIt != nets.end())
        return *netIt;
    return nullptr;
}

NetFactory* NetFactory::getInstance()
{
    if(!instance_)
        instance_ = new NetFactory();
    return instance_;
}

QStringList NetFactory::getStringList()
{
    QStringList list;
    list.append(nets.keys());
    return list;
}

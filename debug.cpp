#include "debug.h"

Debug::Debug(QObject *parent) : QObject(parent)
{

}

void Debug::show(QVariant var, bool isShow)
{
    if(!DEBUG_GLOBAL || !isShow)
        return;
    qDebug()<<var;
}

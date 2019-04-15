#include "abstracttask.h"
AbstractTask::AbstractTask(QObject *parent) : QObject(parent)
{

}

AbstractTask::~AbstractTask()
{

}

AbstractTask::Ptr AbstractTask::Null()
{
    return AbstractTask::Ptr(new NullTask());
}

NullTask::NullTask(QObject *parent): AbstractTask(parent){}

void NullTask::run(){ return;/*ничего не деаем*/}

QString NullTask::getName() {return tr("Null");}



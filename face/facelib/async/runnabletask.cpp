#include "runnabletask.h"



RunnableTask::RunnableTask(const AbstractTask::Ptr &task/*= AbstractTask::Null()*/, QObject *parent /*=0*/) : QObject(parent),
    m_taskPtr(task)
{
    QObject::connect(m_taskPtr.data(),
                     &AbstractTask::changed,
                     this,
                     &RunnableTask::changed);
}

RunnableTask::~RunnableTask()
{
    qDebug().nospace() << "[dtor " << this << "] disconnect changed signal from task: " << m_taskPtr.data();
    QObject::disconnect(m_taskPtr.data(),
                         &AbstractTask::changed,
                         this,
                         &RunnableTask::changed);
}

void RunnableTask::run()
{
    // сигнал о запуске задачи
    emit started(m_taskPtr);
    // выполняем
    m_taskPtr->run();
    // сигнал об останове задачи
    emit finished(m_taskPtr);
}


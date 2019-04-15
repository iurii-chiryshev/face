#include "backgroundworker.h"

/***
 *
 * BackgroundWorker
 *
 * */
BackgroundWorker::BackgroundWorker(int maxThreadCount, QObject *parent) : QObject(parent)
{
    // регистрируем тип, который ходит в сигналах\слотах
    qRegisterMetaType<AbstractTask::Ptr>("AbstractTask::Ptr");
    // запускаем по одной задаче за раз
    m_threadPool.setMaxThreadCount(maxThreadCount);
}

BackgroundWorker::~BackgroundWorker()
{
    // ждем завершения задач в пуле
    m_threadPool.waitForDone();
}


void BackgroundWorker::start(const AbstractTask::Ptr &task /*= AbstractTask::Null()*/)
{
#ifdef NO_BACKGROUND
    QSharedPointer<QRunnable> rPtr(createRunnable(task));
    rPtr->run();
#else
    m_threadPool.start(createRunnable(task));
#endif
}

bool BackgroundWorker::tryStart(const AbstractTask::Ptr &task /*= AbstractTask::Null()*/)
{
#ifdef NO_BACKGROUND
    QSharedPointer<QRunnable> rPtr(createRunnable(task));
    rPtr->run();
    return true;
#else
    return m_threadPool.tryStart(createRunnable(task));
#endif

}

QRunnable *BackgroundWorker::createRunnable(const AbstractTask::Ptr &task /*= AbstractTask::Null()*/)
{
    RunnableTask* proxy = new RunnableTask(task);
    proxy->setAutoDelete(true);

    // started
    QObject::connect(proxy,
                     &RunnableTask::started,
                     this,
                     &BackgroundWorker::started);
    //changed
    QObject::connect(proxy,
                     &RunnableTask::changed,
                     this,
                     &BackgroundWorker::changed);

    // finished
    QObject::connect(proxy,
                     &RunnableTask::finished,
                     this,
                     &BackgroundWorker::finished);

    return proxy;
}







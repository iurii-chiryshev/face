/**
 * @file
 * @brief Заголовочный файл с классом выполнителя операций в отдельном потоке
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * */

#ifndef FLIB_BACKGROUNDWORKER_H
#define FLIB_BACKGROUNDWORKER_H



#include <QObject>
#include <QThreadPool>
#include <QDebug>

#include "runnabletask.h"
#include "facelib_global.h"


/**
 * @brief The BackgroundWorker class
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * Позволяет выполнять операции в отдельном, выделенном потоке.
 */

class FACELIBSHARED_EXPORT BackgroundWorker : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief BackgroundWorker
     * @param parent
     * ctor
     */
    explicit BackgroundWorker(int maxThreadCount = 1,QObject *parent = 0);

    ~BackgroundWorker();

    /**
     * @brief start
     * @param[in] task указатель на задачу
     * Положить задачу в очередь на исполнение.
     * Притом неважно есть свободный поток или нет.
     * Задача запустится как только кто-нибудь освободится.
     */
    void start(const AbstractTask::Ptr &task = AbstractTask::Null());
    /**
     * @brief tryStart
     * @param task
     * @return true - если есть свободный поток для выполнения
     * Попытаться положить и выполнить задачу в очередь.
     */
    bool tryStart(const AbstractTask::Ptr &task = AbstractTask::Null());

signals:
    /**
     * @brief started
     * @param[in] name имя задачи
     * Сигнал о начале выполнения задачи
     */
    void started(const AbstractTask::Ptr &task);
    /**
     * @brief changed
     * @param[in] name
     * @param[in] percent
     */
    void changed(QString name, int percent = -1);
    /**
     * @brief finished
     * @param[in] task умный указатель на задачу, которая была выполнена
     * Сигнал о завершении задачи
     */
    void finished(const AbstractTask::Ptr &task);

private slots:

private:
    /**
     * @brief m_threadPool
     * пул потоков - исполнителей фоновых зачач
     */
    QThreadPool m_threadPool;
    /**
     * @brief createRunnable
     * @param[in] task умный указатель на задачу, которая была выполнена
     * @return runnable object
     * Обернуть задачу в QRunnable интерфейс, т.к. QThreadPool работает только с ним.
     */
    QRunnable* createRunnable(const AbstractTask::Ptr &task = AbstractTask::Null());

};



#endif // FLIB_BACKGROUNDWORKER_H

#ifndef FLIB_RUNNABLETASK_H
#define FLIB_RUNNABLETASK_H

#include <QObject>
#include <QDebug>
#include <QSharedPointer>
#include <QRunnable>

#include "abstracttask.h"
#include "facelib_global.h"

/**
 * @brief The TaskWrap class
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * @note Его использует только BackgroundWoker. Для своих задач реализуйте AbstractTask
 * Обертка над задачей - он непосредственно выполняет задачу в qt потоке (qthreadpool).
 */
class FACELIBSHARED_EXPORT RunnableTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    /**
     * @brief RunnableTask
     * @param[in] task
     * @param[in] parent
     * ctor
     */
    explicit RunnableTask(const AbstractTask::Ptr &task = AbstractTask::Null(),QObject *parent = 0);
    /**
     * @brief ~RunnableTask
     * dtor
     */
    virtual ~RunnableTask();

    /**
     * @brief run
     * implementation QRunnable
     */
    void run();

signals:
    /**
     * @brief started
     * @param name имя задачи
     * Задача началась
     */
    void started(const AbstractTask::Ptr &task);
    /**
     * @brief changed
     * @param[in] name
     * @param[in] percent
     * Изменилась подзадача
     */
    void changed(QString name, int percent = -1);
    /**
     * @brief finished
     * @param[in] task задача
     * Задача завершилась
     */
    void finished(const AbstractTask::Ptr& task);

private slots:

private:
    /**
     * @brief m_task
     * сама задача
     */
    AbstractTask::Ptr m_taskPtr;


public:
};

#endif // FLIB_RUNNABLETASK_H

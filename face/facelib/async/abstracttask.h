/**
 * @file
 * @brief Заголовочный файл интерфейса "фоновой" задачи
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * */

#ifndef FLIB_ABSTRACTTASK_H
#define FLIB_ABSTRACTTASK_H

#include "facelib_global.h"

#include <QObject>
#include <QSharedPointer>

/**
 * @brief The AbstractTask class
 * @author Chiryshev Iurii <iurii.chiryshev@mail.ru>
 * Абстрактный класс задачи для выполнения в background-е
 */
class FACELIBSHARED_EXPORT AbstractTask : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractTask)
public:
    /**
     * @brief Ptr
     * Синоним для умного указателя
     */
    typedef QSharedPointer<AbstractTask> Ptr;

    /**
     * @brief AbstractTask
     * @param parent
     * ctor
     */
    explicit AbstractTask(QObject *parent = 0);
    /**
     * @brief ~Task
     * dtor
     */
    virtual ~AbstractTask();
    /**
     * @brief run
     * @code
     * // example
     * void run(){
     *  emit changed("subtask 1...",50);
     *  //do subtask1
     *  ...
     *  emit changed("subtask 2...",100);
     *  //do subtask2
     *  ...
     *  return;
     * }
     * @endcode
     * main executor
     */
    virtual void run() = 0;
    /**
     * @brief getName
     * @return
     * Получить имя задачи.
     * Это имя будет светиться возле progressbar-а для пользователя
     */
    virtual QString getName() = 0;

    /**
     * @brief Null
     * @return
     */
    static AbstractTask::Ptr Null();



signals:
    /**
     * @brief changed
     * @param[in] name имя текущей подзадачи в рамках всей задачи
     * @param[in] percent процент выполненого
     * Вполне возможно, что задача состоит из нескольких подзадач.
     * Этот сигнал о смене подзадачи, если есть необходимость уведомить пользователя.
     */
    void changed(QString name, int percent = -1 /*< 0 not used*/);

public slots:

};

/**
 * @brief The NullTask class
 * Пустая задача
 */
class FACELIBSHARED_EXPORT NullTask: public AbstractTask{
public:
    typedef QSharedPointer<NullTask> Ptr;
    /**
     * @brief NullTask
     * @param parent
     * ctor
     */
    explicit NullTask(QObject *parent = 0);
    /**
     * @brief run
     */
    void run();
    /**
     * @brief getName
     * @return
     */
    QString getName();
};


#endif // FLIB_ABSTRACTTASK_H

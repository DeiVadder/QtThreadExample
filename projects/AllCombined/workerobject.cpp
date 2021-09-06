#include "workerobject.h"

#include <QElapsedTimer>

WorkerObject::WorkerObject(QObject *parent) : QObject(parent)
{

}

void WorkerObject::heavyOperation()
{
    QElapsedTimer t;
    t.start();

    try {
        quint64 result = fibonacci(FibonacciNumber);
        double time = t.elapsed()/1000.0;

        emit operationDone(result, time);

    }  catch (int) {
        emit operationCanceled();
    }
}

quint64 WorkerObject::fibonacci(quint64 value)
{
    if(m_cancel) //Keep in mind, that a cancel option like this increases run time. In this particualy case, by a factor of 4
        throw -1;

    if (value == 0)
        return 0;
    if (value == 1)
        return 1;
    return fibonacci(value - 1) + fibonacci(value - 2);
}

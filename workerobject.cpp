#include "workerobject.h"

#include <QElapsedTimer>

WorkerObject::WorkerObject(QObject *parent) : QObject(parent)
{

}

void WorkerObject::heavyOperation()
{
    QElapsedTimer t;
    t.start();

    quint64 result = fibonacci(FibonacciNumber);
    double time = t.elapsed()/1000.0;

    emit operationDone(result, time);
}

quint64 WorkerObject::fibonacci(quint64 value)
{
    if (value == 0)
        return 0;
    if (value == 1)
        return 1;
    return fibonacci(value - 1) + fibonacci(value - 2);
}

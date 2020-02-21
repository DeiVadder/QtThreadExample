#include "workerthread.h"
#include <QElapsedTimer>

WorkerThread::WorkerThread(QObject *parent) : QThread(parent)
{

}

void WorkerThread::heavyOperation()
{
    QElapsedTimer t;
    t.start();

    quint64 result = fibonacci(FibonacciNumber);
    double time = t.elapsed()/1000.0;

    emit operationDone(result, time);
}

quint64 WorkerThread::fibonacci(quint64 value)
{
    if (value == 0)
        return 0;
    if (value == 1)
        return 1;
    return fibonacci(value - 1) + fibonacci(value - 2);
}

void WorkerThread::run()
{
    heavyOperation();

/*uncomment this, when you want an event loop running,
otherwise the finished signal will be emitted as soon as heavyOperation() returns*/
//    exec();
}

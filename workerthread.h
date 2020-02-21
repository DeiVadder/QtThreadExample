#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = nullptr);

private:
    void heavyOperation();
    quint64 fibonacci(quint64 value);

protected:

    virtual void run() override;

signals:
    void operationDone(quint64 result, double time);

};

#endif // WORKERTHREAD_H

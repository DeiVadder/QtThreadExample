#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = nullptr);

public slots:
    void cancel(){m_cancel = true;}

private:
    void heavyOperation();
    quint64 fibonacci(quint64 value);

protected:

    virtual void run() override;

signals:
    void operationDone(quint64 result, double time);
    void operationCanceled();

private:
    std::atomic<bool> m_cancel{false};
};

#endif // WORKERTHREAD_H

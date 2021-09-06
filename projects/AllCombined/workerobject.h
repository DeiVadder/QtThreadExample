#ifndef WORKEROBJECT_H
#define WORKEROBJECT_H

#include <QObject>

class WorkerObject : public QObject
{
    Q_OBJECT
public:
    explicit WorkerObject(QObject *parent = nullptr);

public slots:
    void heavyOperation();
    void cancel(){m_cancel = true;}

signals:
    void operationDone(quint64 result, double time);
    void operationCanceled();

private:
    quint64 fibonacci(quint64 value);
    std::atomic<bool> m_cancel{false};

};

#endif // WORKEROBJECT_H

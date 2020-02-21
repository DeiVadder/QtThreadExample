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

signals:
    void operationDone(quint64 result, double time);

private:
    quint64 fibonacci(quint64 value);

};

#endif // WORKEROBJECT_H

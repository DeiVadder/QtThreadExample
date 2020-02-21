#include "mainwindow.h"
#include "ui_mainwindow.h"

//![1]
#include "QMessageBox"
#include <QElapsedTimer>

//![2]
#include "workerobject.h"
#include <QThread>

//![3]
#include "workerthread.h"

//![4]
#include <QtConcurrentRun>


/*
[1] = No Thread
[2] = Worker Object approach
[3] = Subclass QThread
[4] = QtConcurrent
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//![1]
    connect(ui->btn1, &QPushButton::clicked, this, &MainWindow::heavyOperation);

//![2]
    connect(ui->btn2, &QPushButton::clicked, this, &MainWindow::heavyOperationWorkerApproach);

//![3]
    connect(ui->btn3, &QPushButton::clicked, this, &MainWindow::heavyOperationQThreadSubclass);

//![4]
    connect(this, &MainWindow::operationDone, this, &MainWindow::onOperationDone);

//![4]
    connect(ui->btn4, &QPushButton::clicked, this, &MainWindow::heavyOperationQtConcurrent);
    connect(&m_futureWatcher, &QFutureWatcher<QPair<quint64,double>>::finished, this, &MainWindow::onWatchedFutureFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//![1]
void MainWindow::heavyOperation()
{
    QElapsedTimer t;
    t.start();

    quint64 result = fibonacci(FibonacciNumber);

//![1]W
    QMessageBox::information(this,QString("Fibonacci of %1").arg(FibonacciNumber),QString("Result: %1 \n in %2 sec").arg(result).arg(t.elapsed()/1000., 0,'f',1));
}

//![1]
quint64 MainWindow::fibonacci(quint64 value) {
    if (value == 0)
        return 0;
    if (value == 1)
        return 1;
    return fibonacci(value - 1) + fibonacci(value - 2);
}

//![2]
void MainWindow::heavyOperationWorkerApproach()
{
    WorkerObject *worker = new WorkerObject();
    QThread *normalThread = new QThread(this);

    worker->moveToThread(normalThread);

    connect(normalThread, &QThread::started, worker, &WorkerObject::heavyOperation);
    connect(worker, &WorkerObject::operationDone, normalThread, &QThread::quit);
    connect(normalThread, &QThread::finished, normalThread, &QThread::deleteLater);
    connect(normalThread, &QThread::finished, worker, &WorkerObject::deleteLater);

    connect(worker, &WorkerObject::operationDone, this, &MainWindow::onOperationDone);
    normalThread->start();
}

//![3]
void MainWindow::heavyOperationQThreadSubclass()
{
    WorkerThread *workerThread = new WorkerThread(this);

    connect(workerThread, &WorkerThread::operationDone, this, &MainWindow::onOperationDone);
    connect(workerThread, &WorkerThread::operationDone, workerThread, &WorkerThread::deleteLater);

    workerThread->start();
}

//![4]
void MainWindow::heavyOperationQtConcurrent()
{
    QFuture<QPair<quint64, double>> future = QtConcurrent::run(this,&MainWindow::heavyOperationNoGui);
    m_futureWatcher.setFuture(future);
}

//![4]
QPair<quint64, double> MainWindow::heavyOperationNoGui()
{
    QElapsedTimer t;
    t.start();

    quint64 result = fibonacci(FibonacciNumber);
    return QPair<quint64, double>{result, t.elapsed()/1000.};
}

//![4]
void MainWindow::onWatchedFutureFinished()
{
    emit operationDone(m_futureWatcher.result().first, m_futureWatcher.result().second);
}

//![2,3,4]
void MainWindow::onOperationDone(quint64 result, double time)
{
    QMessageBox::information(this,QString("Fibonacci of %1").arg(FibonacciNumber),QString("Result (threaded): %1 \n in %2 sec").arg(result).arg(time, 0,'f',1));
}






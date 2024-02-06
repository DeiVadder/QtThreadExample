#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QMessageBox"
#include <QMovie>

//![1,4]
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
[5] = QThread::Create
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto mov = new QMovie(QStringLiteral(":/giphy.gif"));//https://giphy.com/gifs/PeninsulaHumaneSocietySPCA-working-hard-hardly-phsspca-gishiA5celsz6ASgOO
    connect(this, &MainWindow::stopAnimation, mov, &QMovie::stop);
    ui->busyIndicator->setMovie(mov);

    ui->btn1->setText(ui->btn1->text().arg(FibonacciNumber));
    ui->btn2->setText(ui->btn2->text().arg(FibonacciNumber));
    ui->btn3->setText(ui->btn3->text().arg(FibonacciNumber));
    ui->btn4->setText(ui->btn4->text().arg(FibonacciNumber));
    ui->btn5->setText(ui->btn5->text().arg(FibonacciNumber));

    connect(ui->btn1, &QPushButton::clicked, mov, &QMovie::start);
    connect(ui->btn2, &QPushButton::clicked, mov, &QMovie::start);
    connect(ui->btn3, &QPushButton::clicked, mov, &QMovie::start);
    connect(ui->btn4, &QPushButton::clicked, mov, &QMovie::start);
    connect(ui->btn5, &QPushButton::clicked, mov, &QMovie::start);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::stopAnimation);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::cancelSignal);

//![1]
    connect(ui->btn1, &QPushButton::clicked, this, &MainWindow::heavyOperation);

//![2]
    connect(ui->btn2, &QPushButton::clicked, this, &MainWindow::heavyOperationWorkerApproach);

//![3]
    connect(ui->btn3, &QPushButton::clicked, this, &MainWindow::heavyOperationQThreadSubclass);

//![4]
    connect(ui->btn4, &QPushButton::clicked, this, &MainWindow::heavyOperationQtConcurrent);
    connect(&m_futureWatcher, &QFutureWatcher<QPair<quint64,double>>::finished, this, &MainWindow::onWatchedFutureFinished);

//![5]
    connect(ui->btn5, &QPushButton::clicked, this, &MainWindow::heavyOperationQThreadCreate);
    connect(this, &MainWindow::operationDone, this, &MainWindow::onOperationDone);

//![4,5]
    connect(this, &MainWindow::cancelSignal, this, [&]()->void{m_cancel = true;});
}

MainWindow::~MainWindow()
{
    emit cancelSignal();
    delete ui;
}

//![1]
void MainWindow::heavyOperation()
{
    QElapsedTimer t;
    t.start();

    quint64 result = fibonacci(FibonacciNumber);

//![1]W
    emit stopAnimation();
    QMessageBox::information(this,QString("Fibonacci of %1").arg(FibonacciNumber),QString("Result: %1 \n in %2 sec").arg(result).arg(t.elapsed()/1000., 0,'f',1));
}

//![1,4]
quint64 MainWindow::fibonacci(quint64 value) {
    if(m_cancel) //Keep in mind, that a cancel option like this increases run time. In this particualy case, by a factor of 4
        throw -1;
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

    connect(this, &MainWindow::cancelSignal, worker, &WorkerObject::cancel, Qt::DirectConnection);
    connect(this, &MainWindow::cancelSignal, normalThread, &QThread::quit);

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
    //WorkerThread has no event loop -> calling quit has no effect -> we need to actually terminate
    connect(this, &MainWindow::cancelSignal, workerThread, &WorkerThread::cancel, Qt::DirectConnection);

    workerThread->start();
}

//![4]
void MainWindow::heavyOperationQtConcurrent()
{
    if(m_futureWatcher.isFinished()){
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
        QFuture<QPair<quint64, double>> future = QtConcurrent::run(&MainWindow::heavyOperationNoGui, this);
#else
        QFuture<QPair<quint64, double>> future = QtConcurrent::run(this,&MainWindow::heavyOperationNoGui);
#endif
        //QFutures resulting from a QtConcurrent::run call can not be canceld via  QFutureWatcher::cancel call,
        //We'll have to use an atomic bool to check and modify.
        m_futureWatcher.setFuture(future);
    } else {
        QMessageBox::warning(this, QString("!"), QString("The futur is not done yet!"));
    }
}

//![4]
QPair<quint64, double> MainWindow::heavyOperationNoGui()
{
    QElapsedTimer t;
    t.start();

    m_cancel = false; //reset cancel
    try {
        quint64 result = fibonacci(FibonacciNumber);
        return QPair<quint64, double>{result, t.elapsed()/1000.};
    }  catch (int) {
        return QPair<quint64, double>{0,0};
    }
}

//![4]
void MainWindow::onWatchedFutureFinished()
{
    auto result = m_futureWatcher.result();
    if(result.first != 0)
        onOperationDone(result.first, result.second);
}

//![5]
void MainWindow::heavyOperationQThreadCreate()
{
    //Guard is needed, because operationDone is a MainWindow signal that could also be emitted by other QThreadCreate::instances
    //Technically, the call to quit, should keep the thread alive, until its finished.
    static bool guard(false);
    if(!guard || m_cancel){
        guard = true;
        QThread *thread = QThread::create([=]()->void{onHeavyOperationQThreadCreate();});
        connect(this, &MainWindow::operationDone, thread, &QThread::quit);
        connect(this, &MainWindow::cancelSignal, thread, &QThread::quit);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        connect(thread, &QThread:: finished, thread, [=]()->void{
            thread->deleteLater();
            guard = false;
        });

        thread->start();
    } else {
        QMessageBox::warning(this, QString("Guard Warning!"), QString("Thread not yet finished"));
    }
}

//![5]
void MainWindow::onHeavyOperationQThreadCreate()
{
    QElapsedTimer t;
    t.start();

    m_cancel = false;
    try {
        quint64 result = fibonacci(FibonacciNumber);

        emit operationDone(result, t.elapsed()/1000.);
    }  catch (int) {
        //
    }
}

//![2,3,4,5]
void MainWindow::onOperationDone(quint64 result, double time)
{
    emit stopAnimation();
    QMessageBox::information(this,QString("Fibonacci of %1").arg(FibonacciNumber),QString("Result (threaded): %1 \n in %2 sec").arg(result).arg(time, 0,'f',1));
}






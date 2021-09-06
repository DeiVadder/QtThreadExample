#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QMessageBox"
#include <QMovie>

#include "workerobject.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto mov = new QMovie(QStringLiteral(":/giphy.gif"));//https://giphy.com/gifs/PeninsulaHumaneSocietySPCA-working-hard-hardly-phsspca-gishiA5celsz6ASgOO
    connect(this, &MainWindow::stopAnimation, mov, &QMovie::stop);
    ui->busyIndicator->setMovie(mov);

    ui->btn2->setText(ui->btn2->text().arg(FibonacciNumber));

    connect(ui->btn2, &QPushButton::clicked, mov, &QMovie::start);

    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::stopAnimation);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::cancelSignal);

//![2]
    connect(ui->btn2, &QPushButton::clicked, this, &MainWindow::heavyOperationWorkerApproach);
}

MainWindow::~MainWindow()
{
    emit cancelSignal();
    delete ui;
}

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

void MainWindow::onOperationDone(quint64 result, double time)
{
    emit stopAnimation();
    QMessageBox::information(this,QString("Fibonacci of %1").arg(FibonacciNumber),QString("Result (threaded): %1 \n in %2 sec").arg(result).arg(time, 0,'f',1));
}






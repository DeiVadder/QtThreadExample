#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QMessageBox"
#include <QMovie>

#include "workerthread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto mov = new QMovie(QStringLiteral(":/giphy.gif"));//https://giphy.com/gifs/PeninsulaHumaneSocietySPCA-working-hard-hardly-phsspca-gishiA5celsz6ASgOO
    connect(this, &MainWindow::stopAnimation, mov, &QMovie::stop);
    ui->busyIndicator->setMovie(mov);

    ui->btn3->setText(ui->btn3->text().arg(FibonacciNumber));


    connect(ui->btn3, &QPushButton::clicked, mov, &QMovie::start);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::stopAnimation);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::cancelSignal);

    connect(ui->btn3, &QPushButton::clicked, this, &MainWindow::heavyOperationQThreadSubclass);
}

MainWindow::~MainWindow()
{
    emit cancelSignal();
    delete ui;
}

void MainWindow::heavyOperationQThreadSubclass()
{
    WorkerThread *workerThread = new WorkerThread(this);

    connect(workerThread, &WorkerThread::operationDone, this, &MainWindow::onOperationDone);
    connect(workerThread, &WorkerThread::operationDone, workerThread, &WorkerThread::deleteLater);
    //WorkerThread has no event loop -> calling quit has no effect -> we need to actually terminate
    connect(this, &MainWindow::cancelSignal, workerThread, &WorkerThread::cancel, Qt::DirectConnection);

    workerThread->start();
}

void MainWindow::onOperationDone(quint64 result, double time)
{
    emit stopAnimation();
    QMessageBox::information(this,QString("Fibonacci of %1").arg(FibonacciNumber),QString("Result (threaded): %1 \n in %2 sec").arg(result).arg(time, 0,'f',1));
}






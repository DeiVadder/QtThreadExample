#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QMessageBox"
#include <QMovie>

//![1,4]
#include <QElapsedTimer>

#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto mov = new QMovie(QStringLiteral(":/giphy.gif"));//https://giphy.com/gifs/PeninsulaHumaneSocietySPCA-working-hard-hardly-phsspca-gishiA5celsz6ASgOO
    connect(this, &MainWindow::stopAnimation, mov, &QMovie::stop);
    ui->busyIndicator->setMovie(mov);

    ui->btn5->setText(ui->btn5->text().arg(FibonacciNumber));

    connect(ui->btn5, &QPushButton::clicked, mov, &QMovie::start);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::stopAnimation);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::cancelSignal);

    connect(ui->btn5, &QPushButton::clicked, this, &MainWindow::heavyOperationQThreadCreate);
    connect(this, &MainWindow::operationDone, this, &MainWindow::onOperationDone);

    connect(this, &MainWindow::cancelSignal, this, [&]()->void{m_cancel = true;});
}

MainWindow::~MainWindow()
{
    emit cancelSignal();
    delete ui;
}

quint64 MainWindow::fibonacci(quint64 value) {
    if(m_cancel)//Keep in mind, that a cancel option like this increases run time. In this particualy case, by a factor of 4
        throw -1;
    if (value == 0)
        return 0;
    if (value == 1)
        return 1;
    return fibonacci(value - 1) + fibonacci(value - 2);
}

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

void MainWindow::onOperationDone(quint64 result, double time)
{
    emit stopAnimation();
    QMessageBox::information(this,QString("Fibonacci of %1").arg(FibonacciNumber),QString("Result (threaded): %1 \n in %2 sec").arg(result).arg(time, 0,'f',1));
}






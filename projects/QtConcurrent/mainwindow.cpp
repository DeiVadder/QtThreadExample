#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QMessageBox"
#include <QMovie>

#include <QElapsedTimer>

#include <QtConcurrentRun>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto mov = new QMovie(QStringLiteral(":/giphy.gif"));
    connect(this, &MainWindow::stopAnimation, mov, &QMovie::stop);
    ui->busyIndicator->setMovie(mov);

    ui->btn4->setText(ui->btn4->text().arg(FibonacciNumber));

    connect(ui->btn4, &QPushButton::clicked, mov, &QMovie::start);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::stopAnimation);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::cancelSignal);

    connect(ui->btn4, &QPushButton::clicked, this, &MainWindow::heavyOperationQtConcurrent);
    connect(&m_futureWatcher, &QFutureWatcher<QPair<quint64,double>>::finished, this, &MainWindow::onWatchedFutureFinished);

    connect(this, &MainWindow::cancelSignal, this, [&]()->void{m_cancel = true;});
}

MainWindow::~MainWindow()
{
    emit cancelSignal();
    delete ui;
}

quint64 MainWindow::fibonacci(quint64 value) {
    if(m_cancel) //Keep in mind, that a cancel option like this increases run time. In this particualy case, by a factor of 4
        throw -1;
    if (value == 0)
        return 0;
    if (value == 1)
        return 1;
    return fibonacci(value - 1) + fibonacci(value - 2);
}

void MainWindow::heavyOperationQtConcurrent()
{
    if(m_futureWatcher.isFinished()){
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
        QFuture<QPair<quint64, double>> future = QtConcurrent::run(&MainWindow::heavyOperationNoGui, this);
#else \
        QFuture<QPair<quint64, double>> future = QtConcurrent::run(this,&MainWindow::heavyOperationNoGui);
#endif
        //QFutures resulting from a QtConcurrent::run call can not be canceld via  QFutureWatcher::cancel call,
        //We'll have to use an atomic bool to check and modify.
        m_futureWatcher.setFuture(future);
    } else {
        QMessageBox::warning(this, QString("!"), QString("The futur is not done yet!"));
    }
}

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

void MainWindow::onWatchedFutureFinished()
{
    auto result = m_futureWatcher.result();
    if(result.first != 0)
        onOperationDone(result.first, result.second);
}

void MainWindow::onOperationDone(quint64 result, double time)
{
    emit stopAnimation();
    QMessageBox::information(this,QString("Fibonacci of %1").arg(FibonacciNumber),QString("Result (threaded): %1 \n in %2 sec").arg(result).arg(time, 0,'f',1));
}






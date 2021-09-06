#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QElapsedTimer>
#include "QMessageBox"
#include <QMovie>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto mov = new QMovie(QStringLiteral(":/giphy.gif"));//https://giphy.com/gifs/PeninsulaHumaneSocietySPCA-working-hard-hardly-phsspca-gishiA5celsz6ASgOO
    connect(this, &MainWindow::stopAnimation, mov, &QMovie::stop);
    ui->busyIndicator->setMovie(mov);

    ui->btn1->setText(ui->btn1->text().arg(FibonacciNumber));

    connect(ui->btn1, &QPushButton::clicked, mov, &QMovie::start);

    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::stopAnimation);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::cancelSignal);

    connect(ui->btn1, &QPushButton::clicked, this, &MainWindow::heavyOperation);
}

MainWindow::~MainWindow()
{
    emit cancelSignal();
    delete ui;
}

void MainWindow::heavyOperation()
{
    QElapsedTimer t;
    t.start();

    quint64 result = fibonacci(FibonacciNumber);

    emit stopAnimation();
    QMessageBox::information(this,QString("Fibonacci of %1").arg(FibonacciNumber),QString("Result: %1 \n in %2 sec").arg(result).arg(t.elapsed()/1000., 0,'f',1));
}

quint64 MainWindow::fibonacci(quint64 value) {
    if (value == 0)
        return 0;
    if (value == 1)
        return 1;
    return fibonacci(value - 1) + fibonacci(value - 2);
}






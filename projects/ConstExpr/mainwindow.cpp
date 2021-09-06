#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QMessageBox"
#include <QMovie>

#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto mov = new QMovie(QStringLiteral(":/giphy.gif"));//https://giphy.com/gifs/PeninsulaHumaneSocietySPCA-working-hard-hardly-phsspca-gishiA5celsz6ASgOO
    connect(this, &MainWindow::stopAnimation, mov, &QMovie::stop);
    ui->busyIndicator->setMovie(mov);

    ui->btn1->setText(ui->btn1->text().arg(25));

    connect(ui->btn1, &QPushButton::clicked, mov, &QMovie::start);

    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::stopAnimation);
    connect(ui->btn6, &QPushButton::clicked, this, &MainWindow::cancelSignal);

//![1]
    connect(ui->btn1, &QPushButton::clicked, this, &MainWindow::fibonacciByTheCompiler);
}

MainWindow::~MainWindow()
{
    emit cancelSignal();
    delete ui;
}

constexpr quint64 fibonacci(quint64 value) {
    if (value == 0)
        return 0;
    if (value == 1)
        return 1;
    return fibonacci(value - 1) + fibonacci(value - 2);
}

void MainWindow::fibonacciByTheCompiler()
{
    QElapsedTimer t;
    t.start();

    //Any value higher than 25 may break on compilers like clang because they don't do any memoization of constexpr evaluation
    constexpr auto result = fibonacci(25);

    emit stopAnimation();
    QMessageBox::information(this,QString("Fibonacci of %1").arg(FibonacciNumber),QString("Result: %1 \n in %2 sec").arg(result).arg(t.elapsed()/1000., 0,'f',1));
}








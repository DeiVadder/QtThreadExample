#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//!4
#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void heavyOperation();

    void heavyOperationWorkerApproach();

    void heavyOperationQThreadSubclass();

    void heavyOperationQtConcurrent();

    void heavyOperationQThreadCreate();

signals:
    void operationDone(quint64 result, double time);
    void stopAnimation();
    void cancelSignal();

private slots:
    void onOperationDone(quint64 result, double time);

    QPair<quint64, double> heavyOperationNoGui();
    void onWatchedFutureFinished();

    void onHeavyOperationQThreadCreate();

private:
    quint64 fibonacci(quint64 value);
    std::atomic<bool> m_cancel{false};
private:
    Ui::MainWindow *ui;

    QFutureWatcher<QPair<quint64,double>> m_futureWatcher;
};
#endif // MAINWINDOW_H

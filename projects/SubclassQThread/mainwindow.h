#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void heavyOperationQThreadSubclass();

signals:
    void operationDone(quint64 result, double time);
    void stopAnimation();
    void cancelSignal();

private slots:
    void onOperationDone(quint64 result, double time);

private:
    quint64 fibonacci(quint64 value);
    std::atomic<bool> m_cancel{false};
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

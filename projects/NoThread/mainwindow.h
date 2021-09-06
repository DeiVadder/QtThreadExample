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
    void heavyOperation();

signals:
    void operationDone(quint64 result, double time);
    void stopAnimation();
    void cancelSignal();

private:
    quint64 fibonacci(quint64 value);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H

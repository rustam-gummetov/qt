#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *tmr;

private slots:
    void updateTime();

private slots:
    void m_reduce();

private slots:
    void m_increase();

private slots:
    void n_reduce();

private slots:
    void n_increase();

private slots:
    void ok();

private slots:
    void begin();

private slots:
    void restart();

private:
    void testP(int i, int j, int count, int *field[]);

private:
    void find_only_way(int i, int j, int search, int *field[], int *way[], int current[]);

};
#endif // MAINWINDOW_H

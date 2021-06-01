#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <string>
#include <queue>
#include <QTimer>
#include <QTime>

using namespace std;

int m = 3, n = 3;
int side_square = 30;
queue <vector<int>> numbers;
int C;              //количество ходов
int way[2][10];
int second = 0;

QBrush br(Qt::green);
QPen pen(Qt::black);
//QTimer *tmr;

QGraphicsScene *PalletScene;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pb_begin->setVisible(false);
    ui->pb_restart->setVisible(false);
    ui->label_counter->setVisible(false);
    ui->label_way->setVisible(false);
    ui->label_motion->setVisible(false);
    ui->label_min_motion->setVisible(false);
    ui->label_mincount->setVisible(false);
    ui->graphicsView->setVisible(false);

    connect(ui->pb_mleft, SIGNAL(clicked()), this, SLOT(m_reduce()));
    connect(ui->pb_mright, SIGNAL(clicked()), this, SLOT(m_increase()));
    connect(ui->pb_nleft, SIGNAL(clicked()), this, SLOT(n_reduce()));
    connect(ui->pb_nright, SIGNAL(clicked()), this, SLOT(n_increase()));
    connect(ui->pb_ok, SIGNAL(clicked()), this, SLOT(ok()));
    connect(ui->pb_begin, SIGNAL(clicked()), this, SLOT(begin()));
    connect(ui->pb_restart, SIGNAL(clicked()), this, SLOT(restart()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::m_reduce() {
    QString label;
    if (m > 3)
    {
        m--;
        label = QString::number(m);
        ui->label_mdigit->setText(label);
    }
}

void MainWindow::m_increase() {
    QString label;
    if (m < 10)
    {
        m++;
        label = QString::number(m);
        ui->label_mdigit->setText(label);
    }
}

void MainWindow::n_reduce() {
    QString label;
    if (n > 3)
    {
        n--;
        label = QString::number(n);
        ui->label_ndigit->setText(label);
    }
}

void MainWindow::n_increase() {
    QString label;
    if (n < 10)
    {
        n++;
        label = QString::number(n);
        ui->label_ndigit->setText(label);
    }
}

void MainWindow::ok() {
    ui->pb_mleft->setEnabled(false);
    ui->pb_mright->setEnabled(false);
    ui->pb_nleft->setEnabled(false);
    ui->pb_nright->setEnabled(false);
    ui->pb_ok->setEnabled(false);
    ui->pb_begin->setVisible(true);
    ui->graphicsView->setVisible(true);

    ui->graphicsView->resize(n * side_square + 2, m * side_square + 2);

    PalletScene = new QGraphicsScene(0, 0, n * side_square, m * side_square, this);
    ui->graphicsView->setScene(PalletScene);

    for (int i = 0; i <= n * side_square + 2; i += side_square)
    {
        PalletScene->addLine(i, 0, i, m * side_square + 2);
    }
    for (int i = 0; i <= m * side_square + 2; i += side_square)
    {
        PalletScene->addLine(0, i, n * side_square + 2, i);
    }

    int field[m][n];
    for (int i = 0; i < m; i++)
        {
        for (int j = 0; j < n; j++)
            {
                field[i][j] = 0;                    //заполняем поле нулями
            }
        }
    field[0][0] = -1;                           //стартовая точка = -1

    vector<int> mas = { 0, 0 };            //массив в индексами стартовой точки
    numbers.push(mas);                     //добавление стартовой точки в очередь

    int *p[m];
    for (int i = 0; i < m; i++)
        p[i] = field[i];

    while (field[m - 1][n - 1] == 0)
    {
        vector<int> point = numbers.front();       //извлекли из очереди точку
        numbers.pop();

        if (point[0] == 0 && point[1] == 0)
            testP(point[0], point[1], 1, p);
        else
            testP(point[0], point[1], field[point[0]][point[1]] + 1, p);
    }
    int count = field[m - 1][n - 1];  //кол-во ходов сделано
    C = count;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (field[i][j] == 0)
                field[i][j] = -1;
        }
    }
    field[0][0] = 0;

    int *pWay[2];
    for (int i = 0; i < 2; i++) {
        pWay[i] = way[i];
    }

    way[0][count] = m - 1;
    way[1][count] = n - 1;

    int current[2];
    current[0] = m - 1;
    current[1] = n - 1;
    while (count != 0)
    {
        find_only_way(current[0], current[1], count - 1, p, pWay, current);
        count--;
    }

}



void MainWindow::begin() {
    ui->label_motion->setVisible(true);
    ui->label_counter->setVisible(true);
    ui->label_way->setVisible(true);

    tmr = new QTimer();
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime()));
    tmr->setInterval(1000);
    tmr->start();
}



void MainWindow::updateTime()
{
    if (second > C)
    {
        ui->label_min_motion->setVisible(true);
        ui->label_mincount->setVisible(true);
        ui->label_mincount->setText(QString::number(C));
        ui->pb_restart->setVisible(true);
        tmr->stop();
    }
    else
    {
        PalletScene->addRect(way[1][second] * 30, way[0][second] * 30, 30, 30, pen, br);
        ui->label_counter->setText(QString::number(second));
        second++;
    }


}

void MainWindow::restart() {
    ui->pb_mleft->setEnabled(true);
    ui->pb_mright->setEnabled(true);
    ui->pb_nleft->setEnabled(true);
    ui->pb_nright->setEnabled(true);
    ui->pb_ok->setEnabled(true);
    ui->pb_begin->setVisible(false);
    ui->graphicsView->setVisible(false);
    ui->label_motion->setVisible(false);
    ui->label_min_motion->setVisible(false);
    ui->pb_restart->setVisible(false);
    ui->label_way->setVisible(false);

    second = 0;
    m = 3;
    n = 3;
    C = 0;
    ui->graphicsView->setEnabled(false);
    ui->label_mdigit->setText("" + QString::number(m));
    ui->label_ndigit->setText("" + QString::number(n));

    ui->label_counter->setText("");
    ui->label_mincount->setText("");

    while(numbers.size() > 0) {
        numbers.pop();
    }
}

void MainWindow::testP(int i, int j, int count, int *field[]) {
    if ((i - 2) >= 0)  //на 2 вверх
                {
                    if ((j - 1) >= 0)   //на 1 влево
                    {
                        if (field[i - 2][j - 1] == 0)
                        {
                            field[i - 2][j - 1] = count;
                            vector<int> mas = { i - 2, j - 1 };
                            numbers.push(mas);
                        }
                    }
                    if ((j + 1) < n)   //на 1 вправо
                    {
                        if (field[i - 2][j + 1] == 0)
                        {
                            field[i - 2][j + 1] = count;
                            vector<int> mas = { i - 2, j + 1 };
                            numbers.push(mas);
                        }
                    }
                }
                if ((i + 2) < m)    //на 2 вниз
                {
                    if ((j - 1) >= 0)    //на 1 влево
                    {
                        if (field[i + 2][j - 1] == 0)
                        {
                            field[i + 2][j - 1] = count;
                            vector<int> mas = { i + 2, j - 1 };
                            numbers.push(mas);
                        }
                    }
                    if ((j + 1) < n)   //на 1 вправо
                    {
                        if (field[i + 2][j + 1] == 0)
                        {
                            field[i + 2][j + 1] = count;
                            vector<int> mas = { i + 2, j + 1 };
                            numbers.push(mas);
                        }
                    }
                }
                if ((j - 2) >= 0)  //на 2 влево
                {
                    if ((i - 1) >= 0)     //на 1 вверх
                    {
                        if (field[i - 1][j - 2] == 0)
                        {
                            field[i - 1][j - 2] = count;
                            vector<int> mas = { i - 1, j - 2 };
                            numbers.push(mas);
                        }
                    }
                    if ((i + 1) < m)   //на 1 вниз
                    {
                        if (field[i + 1][j - 2] == 0)
                        {
                            field[i + 1][j - 2] = count;
                            vector<int> mas = { i + 1, j - 2 };
                            numbers.push(mas);
                        }
                    }
                }
                if ((j + 2) < n)    //на 2 вправо
                {
                    if ((i - 1) >= 0)    //на 1 вверх
                    {
                        if (field[i - 1][j + 2] == 0)
                        {
                            field[i - 1][j + 2] = count;
                            vector<int> mas = { i - 1, j + 2 };
                            numbers.push(mas);
                        }
                    }
                    if ((i + 1) < m)    //на 1 вниз
                    {
                        if (field[i + 1][j + 2] == 0)
                        {
                            field[i + 1][j + 2] = count;
                            vector<int> mas = { i + 1, j + 2 };
                            numbers.push(mas);
                        }
                    }
                }
}

void MainWindow::find_only_way(int i, int j, int search, int *field[], int *way[], int current[]) {
    bool f = false;
                while (f == false)
                {
                    if ((i - 2) >= 0)  //на 2 вверх
                    {
                        if ((j - 1) >= 0)   //на 1 влево
                        {
                            if (field[i - 2][j - 1] == search)
                            {
                                current[0] = i - 2;
                                current[1] = j - 1;
                                way[0][search] = i - 2;
                                way[1][search] = j - 1;
                                f = true;
                                break;
                            }
                        }
                        if ((j + 1) < n)   //на 1 вправо
                        {
                            if (field[i - 2][j + 1] == search)
                            {
                                current[0] = i - 2;
                                current[1] = j + 1;
                                way[0][search] = i - 2;
                                way[1][search] = j + 1;
                                f = true;
                                break;
                            }
                        }
                    }
                    if ((i + 2) < m)    //на 2 вниз
                    {
                        if ((j - 1) >= 0)    //на 1 влево
                        {
                            if (field[i + 2][j - 1] == search)
                            {
                                current[0] = i + 2;
                                current[1] = j - 1;
                                way[0][search] = i + 2;
                                way[1][search] = j - 1;
                                f = true;
                                break;
                            }
                        }
                        if ((j + 1) < n)   //на 1 вправо
                        {
                            if (field[i + 2][j + 1] == search)
                            {
                                current[0] = i + 2;
                                current[1] = j + 1;
                                way[0][search] = i + 2;
                                way[1][search] = j + 1;
                                f = true;
                                break;
                            }
                        }
                    }
                    if ((j - 2) >= 0)  //на 2 влево
                    {
                        if ((i - 1) >= 0)     //на 1 вверх
                        {
                            if (field[i - 1][j - 2] == search)
                            {
                                current[0] = i - 1;
                                current[1] = j - 2;
                                way[0][search] = i - 1;
                                way[1][search] = j - 2;
                                f = true;
                                break;
                            }
                        }
                        if ((i + 1) < m)   //на 1 вниз
                        {
                            if (field[i + 1][j - 2] == search)
                            {
                                current[0] = i + 1;
                                current[1] = j - 2;
                                way[0][search] = i + 1;
                                way[1][search] = j - 2;
                                f = true;
                                break;
                            }
                        }
                    }
                    if ((j + 2) < n)    //на 2 вправо
                    {
                        if ((i - 1) >= 0)    //на 1 вверх
                        {
                            if (field[i - 1][j + 2] == search)
                            {
                                current[0] = i - 1;
                                current[1] = j + 2;
                                way[0][search] = i - 1;
                                way[1][search] = j + 2;
                                f = true;
                                break;
                            }
                        }
                        if ((i + 1) < m)    //на 1 вниз
                        {
                            if (field[i + 1][j + 2] == search)
                            {
                                current[0] = i + 1;
                                current[1] = j + 2;
                                way[0][search] = i + 1;
                                way[1][search] = j + 2;
                                f = true;
                                break;
                            }
                        }
                    }
                }
}



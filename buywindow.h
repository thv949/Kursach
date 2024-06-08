#ifndef BUYWINDOW_H
#define BUYWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include "database.h"
#include "qsqlrecord.h"
#include <Qstring>

namespace Ui {
class BuyWindow;
}

class BuyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BuyWindow(QWidget *parent = nullptr);
    ~BuyWindow();
    void showEvent(QShowEvent *event);
    void update();

public slots:
    void updateSum();

signals:
    void Return();

private slots:
    void on_Return_button_clicked();
    void on_buy_window_clicked();

private:
    Ui::BuyWindow *ui;
    DataBase db;

};

#endif // BUYWINDOW_H

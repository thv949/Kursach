#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "homewindowadministrator.h"
#include "homewindowaccountant.h"
#include "homewindowemployee.h"
#include "buywindow.h"
#include "database.h"
#include "QTimer"
#include "QDate"

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
    void updateMonthlyBudget();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_enter_button_Clicked_clicked();


signals:
    void openHomeWindowAdministrator();
    void openHomeWindowEmployee();
    void openHomeWindowAccountant();

private:
    Ui::MainWindow *ui;

    HomeWindowAdministrator *_HomeWindowAdministrator;
    HomeWindowAccountant * _HomeWindowAccountant;
    HomeWindowEmployee * _HomeWindowEmployee;
    DataBase db;
    int employeeId;
    QTimer* timer;

};
#endif // MAINWINDOW_H

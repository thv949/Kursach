#include "homewindowemployee.h"
#include "ui_homewindowemployee.h"

HomeWindowEmployee::HomeWindowEmployee(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWindowEmployee)
{
    ui->setupUi(this);

    setWindowTitle("Домашнее окно");

    _PersonAccount = new PersonAccount();
    _PersonsWindow = new PersonsWindow();
    _BuyWindow = new BuyWindow();
    _HistoryWindow = new HistoryWindow();

    connect(this, &HomeWindowEmployee::openPersonAccount, this, &HomeWindowEmployee::close);
    connect(this, &HomeWindowEmployee::openPersonAccount, _PersonAccount, &PersonAccount::show);
    connect(_PersonAccount, &PersonAccount::Return, this, &HomeWindowEmployee::show);

    connect(this, &HomeWindowEmployee::openPersonsWindow, this, &HomeWindowEmployee::close);
    connect(this, &HomeWindowEmployee::openPersonsWindow, _PersonsWindow, &PersonsWindow::show);
    connect(_PersonsWindow, &PersonsWindow::Return, this, &HomeWindowEmployee::show);

    connect(this, &HomeWindowEmployee::openBuyWindow, this, &HomeWindowEmployee::close);
    connect(this, &HomeWindowEmployee::openBuyWindow, _BuyWindow, &BuyWindow::show);
    connect(_BuyWindow, &BuyWindow::Return, this, &HomeWindowEmployee::show);

    connect(this, &HomeWindowEmployee::openHistoryWindow, this, &HomeWindowEmployee::close);
    connect(this, &HomeWindowEmployee::openHistoryWindow, _HistoryWindow, &HistoryWindow::show);
    connect(_HistoryWindow, &HistoryWindow::Return, this, &HomeWindowEmployee::show);

}

HomeWindowEmployee::~HomeWindowEmployee()
{
    delete ui;
}

void HomeWindowEmployee::on_person_account_emp_clicked()
{
    emit openPersonAccount();
}

void HomeWindowEmployee::on_buy_emp_clicked()
{
    emit openBuyWindow();
}

void HomeWindowEmployee::on_persons_emp_clicked()
{
    emit openPersonsWindow();
}

void HomeWindowEmployee::on_history_emp_clicked()
{
    emit openHistoryWindow();
}

void HomeWindowEmployee::on_return_emp_clicked()
{
    emit Return();
    this->close();
}



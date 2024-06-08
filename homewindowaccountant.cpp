#include "homewindowaccountant.h"
#include "ui_homewindowaccountant.h"

HomeWindowAccountant::HomeWindowAccountant(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWindowAccountant)
{
    ui->setupUi(this);
    setWindowTitle("Домашнее окно");

    _PersonAccount = new PersonAccount();
    _PersonsWindow = new PersonsWindow();
    _HistoryWindow = new HistoryWindow();
    _accountantbudgetwindow = new accountantbudgetwindow();

    connect(this, &HomeWindowAccountant::openPersonAccount, this, &HomeWindowAccountant::close);
    connect(this, &HomeWindowAccountant::openPersonAccount, _PersonAccount, &PersonAccount::show);
    connect(_PersonAccount, &PersonAccount::Return, this, &HomeWindowAccountant::show);

    connect(this, &HomeWindowAccountant::openPersonsWindow, this, &HomeWindowAccountant::close);
    connect(this, &HomeWindowAccountant::openPersonsWindow, _PersonsWindow, &PersonsWindow::show);
    connect(_PersonsWindow, &PersonsWindow::Return, this, &HomeWindowAccountant::show);

    connect(this, &HomeWindowAccountant::openHistoryWindow, this, &HomeWindowAccountant::close);
    connect(this, &HomeWindowAccountant::openHistoryWindow, _HistoryWindow, &HistoryWindow::show);
    connect(_HistoryWindow, &HistoryWindow::Return, this, &HomeWindowAccountant::show);

    connect(this, &HomeWindowAccountant::openaccountantbudgetwindow, this, &HomeWindowAccountant::close);
    connect(this, &HomeWindowAccountant::openaccountantbudgetwindow, _accountantbudgetwindow, &ListUnitAccountant::show);
    connect(_accountantbudgetwindow, &accountantbudgetwindow::Return, this, &HomeWindowAccountant::show);
}

HomeWindowAccountant::~HomeWindowAccountant()
{
    delete ui;
}

void HomeWindowAccountant::on_person_account_acc_clicked()
{
    emit openPersonAccount();
}

void HomeWindowAccountant::on_persons_acc_clicked()
{
    emit openPersonsWindow();
}

void HomeWindowAccountant::on_history_acc_clicked()
{
       emit openHistoryWindow();
}

void HomeWindowAccountant::on_unit_acc_clicked()
{
    emit openaccountantbudgetwindow();
}

void HomeWindowAccountant::on_return_accountant_clicked()
{
    emit Return();
    this->close();
}

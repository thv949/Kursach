#include "homewindowadministrator.h"
#include "ui_homewindowadministrator.h"
#include "mainwindow.h"
#include "personaccount.h"
#include "personwindowadministrator.h"
#include "listunitadministrator.h"

HomeWindowAdministrator::HomeWindowAdministrator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWindowAdministrator)
{
    ui->setupUi(this);

    setWindowTitle("Домашнее окно");

    _PersonAccount = new PersonAccount();
    _PersonWindowAdministrator = new PersonWindowAdministrator();
    _ListUnitAdministrator = new ListUnitAdministrator();

    connect(this, &HomeWindowAdministrator::openPersonAccount, this, &HomeWindowAdministrator::close);
    connect(this, &HomeWindowAdministrator::openPersonAccount, _PersonAccount, &PersonAccount::show);
    connect(_PersonAccount, &PersonAccount::Return, this, &HomeWindowAdministrator::show);

    connect(this, &HomeWindowAdministrator::openPersonWindowAdministrator, this, &HomeWindowAdministrator::close);
    connect(this, &HomeWindowAdministrator::openPersonWindowAdministrator, _PersonWindowAdministrator, &PersonWindowAdministrator::show);
    connect(_PersonWindowAdministrator, &PersonWindowAdministrator::Return, this, &HomeWindowAdministrator::show);

    connect(this, &HomeWindowAdministrator::openListUnitAdministrator, this, &HomeWindowAdministrator::close);
    connect(this, &HomeWindowAdministrator::openListUnitAdministrator, _ListUnitAdministrator, &ListUnitAdministrator::show);
    connect(_ListUnitAdministrator, &ListUnitAdministrator::Return, this, &HomeWindowAdministrator::show);
}

HomeWindowAdministrator::~HomeWindowAdministrator()
{
    delete ui;
}


void HomeWindowAdministrator::on_person_account_clicked()
{
    emit openPersonAccount();
}

void HomeWindowAdministrator::on_persons_admin_clicked()
{
    emit openPersonWindowAdministrator();
}

void HomeWindowAdministrator::on_unit_admin_clicked()
{
    emit openListUnitAdministrator();
}


void HomeWindowAdministrator::on_return_ad_clicked()
{
    emit Return();
    this->close();
}

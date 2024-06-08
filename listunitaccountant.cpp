#include "listunitaccountant.h"
#include "ui_listunitaccountant.h"
#include "QMessageBox"
#include "qsqlrecord.h"
#include "QRegularExpressionValidator"

ListUnitAccountant::ListUnitAccountant(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListUnitAccountant)
{
    ui->setupUi(this);
    QRegularExpression rx("[0-9]*");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->budget->setValidator(validator);

    setWindowTitle("Изменить бюджет");
}

ListUnitAccountant::~ListUnitAccountant()
{
    delete ui;
}

void ListUnitAccountant::takeID(int a)
{
    id = a;
    QSqlRecord data = db.getUnitByID(a);
    ui->unit_name->setText(data.value(1).toString());
    ui->all_budget->setText("Общий бюджет отдела: " + data.value(2).toString());
    ui->inf->setText("");
}

void ListUnitAccountant::on_return_acc_clicked()
{
    emit Return();
    this->close();
}


void ListUnitAccountant::on_highlight_clicked()
{
    QString a = ui->budget->text();
    if (a != "" && a!= "0")
    {
        if (db.updateMonthlyBudgetById(id, a.toInt()))
        {
            emit update();
            ui->inf->setText("Бюджет отдела успешно изменен!");
            ui->inf->setStyleSheet("color: green;");
        }
    }
    else {
        ui->inf->setText("Поле не должно быть пустым!");
        ui->inf->setStyleSheet("color: red;");
    }
}

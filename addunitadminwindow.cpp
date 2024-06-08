#include "addunitadminwindow.h"
#include "ui_addunitadminwindow.h"
#include "QMessageBox"

AddUnitAdminWindow::AddUnitAdminWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUnitAdminWindow)
{
    ui->setupUi(this);

    setWindowTitle("Добавить отдел");
}

AddUnitAdminWindow::~AddUnitAdminWindow()
{
    delete ui;
}

void AddUnitAdminWindow::on_return_adm_clicked()
{
    this->close();
}

void AddUnitAdminWindow::applyDefaultStyles()
{
    ui->lineEdit->setText("");
    ui->label_3->setText("");
}

void AddUnitAdminWindow::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    applyDefaultStyles();

}

void AddUnitAdminWindow::on_add_unit_clicked()
{
    if(!ui->lineEdit->text().trimmed().isEmpty())
    {
        int a = db.insertUnit(ui->lineEdit->text());
        if(a == -1)
        {
            QMessageBox::information(nullptr, "Ошибка", "Ошибка при вставке в бд");
        }
        if(a == -2)
        {
            ui->label_3->setText("Данное название уже занято");
            ui->label_3->setStyleSheet("color: red");
        }
        if(a == 1)
        {
            ui->label_3->setText("Отдел успешно добавлен!");
            ui->label_3->setStyleSheet("color: green");
            emit update();
        }
    }
    else
    {
        ui->label_3->setText("Введите название отдела!");
        ui->label_3->setStyleSheet("color: red");
    }

}

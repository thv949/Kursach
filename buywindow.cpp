#include "buywindow.h"
#include "ui_buywindow.h"
#include <QMessageBox>
#include <QIntValidator>


BuyWindow::BuyWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuyWindow)
{
    ui->setupUi(this);

    setWindowTitle("Совершить покупку");

    connect(ui->count, &QLineEdit::textChanged, this, &BuyWindow::updateSum);
    connect(ui->sum, &QLineEdit::textChanged, this, &BuyWindow::updateSum);

    QRegularExpression rx("[0-9]*");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->count->setValidator(validator);
    ui->sum->setValidator(validator);
}

BuyWindow::~BuyWindow()
{
    delete ui;
}

void BuyWindow::on_Return_button_clicked()
{
    emit Return();
    this->close();
}

void BuyWindow::update()
{
    ui->buy_name->setText("");
    ui->description->setText("");
    ui->sum->setText("");
    User* session = User::getInstance();
    QSqlRecord data = db.getUnitByID(db.getUnitId(session->getUnit()));
    ui->all_budget->setText(data.value(2).toString());
    ui->count->setText("1");
}

void BuyWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    ui->inform->setText("");
    update();
}

void BuyWindow::updateSum()
{
    if(ui->sum->text() != "" && ui->count->text() != "")
    {
        if(ui->count->text() == "0") ui->aii_sum->setText(ui->sum->text());
        else {
            int a = ui->sum->text().toInt();
            int b = ui->count->text().toInt();
            int c = a * b;
            ui->aii_sum->setText(QString::number(c));
        }
    }
    else {
        ui->aii_sum->setText("");
    }
}

void BuyWindow::on_buy_window_clicked()
{
    QString purchaseName = ui->buy_name->text();
    QString description = ui->description->toPlainText();
    QString costText = ui->sum->text();
    QString count = ui->count->text();


    if (purchaseName.isEmpty() || description.isEmpty() || costText.isEmpty() || count.isEmpty() || count == "0") {
        ui->inform->setText("Все поля должны быть заполнены!");
        ui->inform->setStyleSheet("color: red;");
        return;
    }

    bool ok;
    int cost = costText.toInt(&ok);

    if (!ok) {
        ui->inform->setText("Стоимость должна быть числом!");
        ui->inform->setStyleSheet("color: red;");
        return;
    }

    if (ui->aii_sum->text().toInt() <= ui->all_budget->text().toInt())
    {
        bool success = db.insertPurchase(purchaseName, description, cost, count.toInt(), ui->aii_sum->text().toInt());

        if (success) {
            ui->inform->setText( "Покупка успешно добавлена!");
            ui->inform->setStyleSheet("color: green;");
            update();
            return;
        }
        else {
            ui->inform->setText("Не удалось добавить покупку!");
            ui->inform->setStyleSheet("color: red;");
        }
    }
    else {
        ui->inform->setText("Цена не должна превышать общий бюжет!");
        ui->inform->setStyleSheet("color: red;");
    }
}




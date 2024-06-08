#include "personaccount.h"
#include "ui_personaccount.h"
#include "QSqlRecord"
#include "QRegExpValidator"
#include "QMessageBox"

PersonAccount::PersonAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonAccount)
{
    ui->setupUi(this);

    setWindowTitle("Личный кабинет");

    ui->lineEdit->setEchoMode(QLineEdit::Password);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    //Валидатор на номер
    QRegularExpression rx("[0-9+]*");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->lineEdit_4->setValidator(validator);
}

PersonAccount::~PersonAccount()
{
    delete ui;
}

void PersonAccount::updateInfo()
{
    User* session = User::getInstance();
    QSqlRecord data = db.getPersonByID(session->getPersonID());
    ui->label_2->setText("Фио: " + data.value("Фамилия").toString() + " " + data.value("Имя").toString() + " " + data.value("Отчество").toString());
    ui->label_3->setText("Логин: " + data.value("Логин").toString());
    ui->label_5->setText("Номер телефона: +7" + data.value("Телефон").toString());
    _Phone = data.value("Телефон").toString();
    _Password = data.value("Пароль").toString();
    ui->label_12->setText("Роль: " + session->getRole());
    ui->label_11->setText("Отдел: " + data.value("Название_Отдела").toString());
}

void PersonAccount::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateInfo();
}


void PersonAccount::on_return_pa_clicked()
{
    emit Return();
    this->close();
    clearMessages();
}

void PersonAccount::clearMessages()
{
    ui->num_luck->clear();
    ui->psw_luck->clear();
}

void PersonAccount::on_change_psw_clicked()
{
    clearMessages();

    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty() || ui->lineEdit_3->text().isEmpty())
    {
        return;
    }

    if (ui->lineEdit->text() == _Password)
    {
        if (ui->lineEdit_2->text() == ui->lineEdit_3->text())
        {
            User* session = User::getInstance();
            if (db.changePassword(session->getPersonID(), ui->lineEdit_2->text()))
            {
                ui->psw_luck->setText("Пароль успешно изменён!");
                ui->psw_luck->setStyleSheet("color: green;");
                _Password = ui->lineEdit_2->text();
            }
            else
            {
                QMessageBox::warning(nullptr, "Ошибка", "Ошибка при обновлении пароля в базе данных");
            }
        }
        else
        {
            ui->psw_luck->setText("Новые пароли не совпадают");
            ui->psw_luck->setStyleSheet("color: red;");
        }
    }
    else
    {
        ui->psw_luck->setText("Неправильный текущий пароль");
        ui->psw_luck->setStyleSheet("color: red;");
    }

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();

    updateInfo();
}

void PersonAccount::on_change_num_clicked()
{
    clearMessages();

    QString newPhone = ui->lineEdit_4->text().trimmed();

    if (!newPhone.isEmpty() && newPhone != _Phone)
    {
        if (newPhone.length() < 10)
        {
            ui->num_luck->setText("Новый номер телефона слишком короткий!");
            ui->num_luck->setStyleSheet("color: red;");
            ui->lineEdit_4->clear();
            return;
        }

        User* session = User::getInstance();
        QStringList errors = db.checkUnique("", newPhone);

        if (errors.isEmpty())
        {
            if (db.changePhone(session->getPersonID(), newPhone))
            {
                ui->num_luck->setText("Номер телефона успешно изменён!");
                ui->num_luck->setStyleSheet("color: green;");
                _Phone = newPhone;
                updateInfo();
            }
            else
            {
                QMessageBox::warning(nullptr, "Ошибка", "Ошибка при обновлении номера телефона в базе данных");
            }
        }
        else
        {
            ui->num_luck->setText("Номер телефона уже используется");
            ui->num_luck->setStyleSheet("color: red;");
        }
    }
    else if (newPhone == _Phone)
    {
        ui->num_luck->setText("Номер телефона совпадает с текущим");
        ui->num_luck->setStyleSheet("color: red;");
    }
    else
    {
        return;
    }

    ui->lineEdit_4->clear();
}

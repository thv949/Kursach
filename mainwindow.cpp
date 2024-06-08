#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);

    setWindowTitle("Окно входа в систему");

    _HomeWindowAdministrator = new HomeWindowAdministrator();
    _HomeWindowEmployee = new HomeWindowEmployee();
    _HomeWindowAccountant = new HomeWindowAccountant();

    connect(this, &MainWindow::openHomeWindowAdministrator, this, &MainWindow::close);
    connect(this, &MainWindow::openHomeWindowAdministrator, _HomeWindowAdministrator, &HomeWindowAdministrator::show);
    connect(_HomeWindowAdministrator, &HomeWindowAdministrator::Return, this, &MainWindow::show);

    connect(this, &MainWindow::openHomeWindowEmployee, this, &MainWindow::close);
    connect(this, &MainWindow::openHomeWindowEmployee, _HomeWindowEmployee, &HomeWindowEmployee::show);
    connect(_HomeWindowEmployee, &HomeWindowEmployee::Return, this, &MainWindow::show);

    connect(this, &MainWindow::openHomeWindowAccountant, this, &MainWindow::close);
    connect(this, &MainWindow::openHomeWindowAccountant, _HomeWindowAccountant, &HomeWindowAccountant::show);
    connect(_HomeWindowAccountant, &HomeWindowAccountant::Return, this, &MainWindow::show);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateMonthlyBudget);
    timer->start(60);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        on_enter_button_Clicked_clicked();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::on_enter_button_Clicked_clicked()
{
    QString gg = db.checkLoginAndPassword(ui->lineEdit->text(), ui->lineEdit_2->text());
    if(gg == "Администратор")
    {
        emit openHomeWindowAdministrator();
    }

    if(gg == "Сотрудник")
    {
        emit openHomeWindowEmployee();
    }

    if(gg == "Бухгалтерия")
    {
        emit openHomeWindowAccountant();
    }

    if(gg == "Ошибка")
    {
        QMessageBox::warning(nullptr, "Ошибка", "Неправильно введён логин или пароль!");
    }
}

void MainWindow::updateMonthlyBudget() {
    QDate currentDate = QDate::currentDate();
    // TODO: бюджет начислен!
    if (currentDate.day() == 1) {
        QSqlQuery query;
        if (!query.exec("SELECT ID, Общий_Бюджет, Бюджет_на_месяц FROM Отдел")) {
            qDebug() << "Error: query execution failed -" << query.lastError();
            return;
        }

        while (query.next()) {
            int id = query.value("ID").toInt();
            int totalBudget = query.value("Общий_Бюджет").toInt();
            int monthlyBudget = query.value("Бюджет_на_месяц").toInt();

            totalBudget += monthlyBudget;

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE Отдел SET Общий_Бюджет = :totalBudget WHERE ID = :id");
            updateQuery.bindValue(":totalBudget", totalBudget);
            updateQuery.bindValue(":id", id);

            if (!updateQuery.exec()) {
                qDebug() << "Error: update query execution failed -" << updateQuery.lastError();
            }
        }
        qDebug() << "Monthly budget update completed.";
    }
}

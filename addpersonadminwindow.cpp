#include "addpersonadminwindow.h"
#include "ui_addpersonadminwindow.h"
#include <QSortFilterProxyModel>
#include "QMessageBox"

AddPersonAdminWindow::AddPersonAdminWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPersonAdminWindow)
{
    ui->setupUi(this);

    setWindowTitle("Добавить сотрудника");

    ui->password->setEchoMode(QLineEdit::Password); // Валидатор на пароль
    // Валидатор на номер
    QRegularExpression rx("[0-9+]*");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->phone->setValidator(validator);
    // Валидатор на имя отчество и т.п.
    QRegularExpression rxx("[А-Яа-яЁё ]*");
    QValidator *validatorr = new QRegularExpressionValidator(rxx, this);
    ui->LName->setValidator(validatorr);
    ui->FName->setValidator(validatorr);
    ui->MName->setValidator(validatorr);

}

void AddPersonAdminWindow::setupSearchableComboBox(QComboBox *comboBox) {
    comboBox->setEditable(true);

    QLineEdit *lineEdit = comboBox->lineEdit();
    lineEdit->setPlaceholderText("Поиск...");

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(comboBox);
    proxyModel->setSourceModel(comboBox->model());
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(0);

    QCompleter *completer = new QCompleter(proxyModel, comboBox);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);

    lineEdit->setCompleter(completer);

    connect(lineEdit, &QLineEdit::textEdited, proxyModel, [proxyModel](const QString &text) {
        proxyModel->setFilterFixedString(text);
    });

    connect(completer, QOverload<const QModelIndex &>::of(&QCompleter::activated), comboBox, [comboBox, proxyModel](const QModelIndex &index) {
        QModelIndex sourceIndex = proxyModel->mapToSource(index);
        if (sourceIndex.isValid()) {
            comboBox->setCurrentIndex(sourceIndex.row());
        }
    });
}

QStringList AddPersonAdminWindow::checkInput(const QString& FName, const QString& LName,
                                             const QString& MName, const QString& phone, const QString& unit,
                                             const QString& login, const QString& password, const QString& role) {
    QStringList errors;
    if (FName.trimmed().isEmpty()) {
        errors << "Name";
    }
    if (LName.trimmed().isEmpty()) {
        errors << "LastName";
    }
    if (MName.trimmed().isEmpty()) {
        errors << "MiddleName";
    }
    if (phone.trimmed().isEmpty() || phone.length() != 10) {
        errors << "NumberPhone";
    }
    if (login.trimmed().isEmpty()) {
        errors << "login";
    }
    if (password.trimmed().isEmpty()) {
        errors << "password";
    }
    if (unit.trimmed().isEmpty()) {
        errors << "unit";
    }
    if (role.trimmed().isEmpty()) {
        errors << "role";
    }
    return errors;
}

void AddPersonAdminWindow::applyEmptyStyles()
{
    ui->FName->setText("");
    ui->LName->setText("");
    ui->MName->setText("");
    ui->phone->setText("");
    ui->login->setText("");
    ui->password->setText("");
}

void AddPersonAdminWindow::applyErrorStyles(const QStringList &errors) {
    QString errorStyle = "border: 1px solid red; color:white;";
    for (const QString &error : errors) {
        if (error == "Name") {
            ui->FName->setStyleSheet(errorStyle);
        } else if (error == "LastName") {
            ui->LName->setStyleSheet(errorStyle);
        } else if (error == "MiddleName") {
            ui->MName->setStyleSheet(errorStyle);
        } else if (error == "NumberPhone" || error == "phoneNotUnique") {
            ui->phone->setStyleSheet(errorStyle);
        } else if (error == "login" || error == "loginNotUnique") {
            ui->login->setStyleSheet(errorStyle);
        } else if (error == "password") {
            ui->password->setStyleSheet(errorStyle);
        } else if (error == "unit")
        {
            ui->role->setStyleSheet(errorStyle);
        }
    }
}

void AddPersonAdminWindow::applyDefStyles()
{
    ui->FName->setStyleSheet("border-bottom-color: rgb(190, 233, 116);");
    ui->LName->setStyleSheet("border-bottom-color: rgb(190, 233, 116);");
    ui->MName->setStyleSheet("border-bottom-color: rgb(190, 233, 116);");
    ui->phone->setStyleSheet("border-bottom-color: rgb(190, 233, 116);");
    ui->login->setStyleSheet("border-bottom-color: rgb(190, 233, 116);");
    ui->password->setStyleSheet("border-bottom-color: rgb(190, 233, 116);");
    ui->label_10->setText("");
}

void AddPersonAdminWindow::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    applyEmptyStyles();
    applyDefStyles();
    ui->Unit->addItems(db.fetchDepartmentNames());
    setupSearchableComboBox(ui->Unit);

}


AddPersonAdminWindow::~AddPersonAdminWindow()
{
    delete ui;
}


void AddPersonAdminWindow::on_return_adm_clicked()
{
    this->close();
}


void AddPersonAdminWindow::on_add_adm_clicked()
{
    QString Name = ui->FName->text();
    QString LastName = ui->LName->text();
    QString MiddleName = ui->MName->text();
    QString NumberPhone = ui->phone->text();
    if(NumberPhone.length() == 12)
    {
        NumberPhone.remove(0, 2);
    }
    QString role = ui->role->currentText();
    QString unit = ui->Unit->currentText();
    QString login = ui->login->text();
    QString password = ui->password->text();

    // Проверка ошибок
    QStringList errors = checkInput(Name, LastName, MiddleName, NumberPhone, unit, login, password, role);
    QStringList uniqueErrors = db.checkUnique(login, NumberPhone);
    errors.append(uniqueErrors);

    applyDefStyles();

    if (errors.isEmpty())
    {
        int ID_Department = db.getUnitId(unit);
        if(ID_Department != -1)
        {
            if(db.insertEmployee(Name, LastName, MiddleName, NumberPhone, role, ID_Department, login, password))
            {
                ui->label_10->setText("Пользователь успешно создан!");
                ui->label_10->setStyleSheet("color: green;");
                emit update();
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Ошибка при вставке в базу данных");
            }
        }
        else
        {
            ui->Unit->setStyleSheet("color: red;"); // Устанавливаем красный цвет
        }
    }
    else
    {
        applyErrorStyles(errors);
    }
}



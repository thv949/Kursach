#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include "user.h"
#include <QString>
#include <QDate>
#include <QCoreApplication>
#include <QFile>

DataBase::DataBase()
{
    //    db = QSqlDatabase::addDatabase("QSQLITE");
    //    db.setDatabaseName("D:\\KurosanDB\\SQL\\Kurosan.db");
    //    if(!db.open())
    //    {
    //        qDebug() << " Ошибка при подключении к базе данных" << db.lastError().text();
    //    }
    //    qDebug() << "Соединение с базой данных установлено.";


    QString dbPath = QCoreApplication::applicationDirPath() + "/Kurs.db";
    bool dbExists = QFile::exists(dbPath);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Ошибка при подключении к базе данных:" << db.lastError().text();
        throw std::runtime_error("Не удалось открыть базу данных.");
    }

    if (!dbExists) {
        qDebug() << "База данных не существует. Создание новой базы данных.";
        initializeDatabase();
    } else {
        qDebug() << "База данных уже существует.";
    }
}


void DataBase::initializeDatabase()
{
    QSqlQuery query(db);

    if (!query.exec(R"(
                    CREATE TABLE "Отдел" (
                    "ID"	INTEGER NOT NULL UNIQUE,
                    "Название_Отдела"	TEXT UNIQUE,
                    "Общий_Бюджет"	INT,
                    "Бюджет_на_месяц"	INT,
                    PRIMARY KEY("ID" AUTOINCREMENT)
                    );
                    )")) {
        qDebug() << "Ошибка при создании таблицы 'Люди':" << query.lastError().text();
    }

    if (!query.exec(R"(
                    CREATE TABLE "Покупка" (
                    "ID_Покупки"	INTEGER NOT NULL UNIQUE,
                    "ID_Сотрудника"	INTEGER NOT NULL,
                    "Название"	TEXT NOT NULL,
                    "Описание"	TEXT NOT NULL,
                    "Стоимость"	INTEGER NOT NULL,
                    "Дата"	TEXT NOT NULL,
                    "Количество"	INTEGER NOT NULL,
                    "Общая_стоимость"	INTEGER NOT NULL,
                    FOREIGN KEY("ID_Сотрудника") REFERENCES "Сотрудник"("ID"),
                    PRIMARY KEY("ID_Покупки" AUTOINCREMENT)
                    );
            )")) {
        qDebug() << "Ошибка при создании таблицы 'Доставка':" << query.lastError().text();
}


if (!query.exec(R"(
                CREATE TABLE "Сотрудник" (
                "ID"	INTEGER NOT NULL UNIQUE,
                "Имя"	TEXT NOT NULL,
                "Фамилия"	TEXT NOT NULL,
                "Отчество"	TEXT,
                "Телефон"	TEXT NOT NULL UNIQUE,
                "Роль"	TEXT NOT NULL,
                "ID_Отдела"	INTEGER NOT NULL,
                "Логин"	TEXT NOT NULL UNIQUE,
                "Пароль"	TEXT NOT NULL,
                FOREIGN KEY("ID_Отдела") REFERENCES "Отдел"("ID"),
                PRIMARY KEY("ID" AUTOINCREMENT)
                );
)")) {
qDebug() << "Ошибка при создании таблицы 'Заказ':" << query.lastError().text();
}

if (!query.exec(R"(
                INSERT INTO "Отдел" ("Название_Отдела", "Общий_Бюджет", "Бюджет_на_месяц")
                VALUES ('По умолчанию', NULL, NULL);
                )")) {
    qDebug() << "Ошибка при добавлении отдела:" << query.lastError().text();
}

// Добавление администратора
if (!query.exec(R"(
                INSERT INTO "Сотрудник" ("Имя", "Фамилия", "Отчество", "Телефон", "Роль", "ID_Отдела", "Логин", "Пароль")
                VALUES ('Админ', 'Администратор', 'Отчество', '1234567890', 'Администратор', '1', 'Админ', '1');
                )")) {
    qDebug() << "Ошибка при добавлении администратора:" << query.lastError().text();
}
}

QString DataBase::checkLoginAndPassword(const QString& login, const QString& password)
{
    QSqlQuery query(db);
    query.prepare(R"(
                  SELECT s.ID, s.Роль, o.Название_Отдела
                  FROM Сотрудник s
                  JOIN Отдел o ON s.ID_Отдела = o.ID
                  WHERE s.Логин = :login AND s.Пароль = :password
                  )");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Ошибка: не удалось выполнить запрос - " << query.lastError();
        return "Error";
    }

    if (query.next()) {
        User* session = User::getInstance();
        session->setPersonID(query.value("ID").toInt());
        session->setRole(query.value("Роль").toString());
        session->setUnit(query.value("Название_Отдела").toString());
        return query.value("Роль").toString();
    }

    return "Ошибка";
}

void DataBase::getUnit()
{
    model = new QSqlTableModel(nullptr, db);
    model->setTable("Отдел");
    model->select();
}

QSqlTableModel* DataBase::getModel()
{
    return model;
}

int DataBase::insertUnit(const QString &unit)
{
    if(isUnitNameAvailable(unit))
    {
        QSqlQuery query(db);
        query.prepare(R"(
                      INSERT INTO Отдел (Название_Отдела, Общий_Бюджет)
                      VALUES (:departmentName, 0)
                      )");
        query.bindValue(":departmentName", unit);

        if (!query.exec()) {
            qDebug() << "Error: failed to insert department -" << query.lastError();
            return -1;
        }

        return 1;
    }
    else return -2;
}

bool DataBase::isUnitNameAvailable(const QString& unit)
{
    QSqlQuery query(db);
    query.prepare(R"(
                  SELECT COUNT(*) FROM Отдел WHERE Название_Отдела = :departmentName
                  )");
    query.bindValue(":departmentName", unit);

    if (!query.exec()) {
        qDebug() << "Error: failed to execute query -" << query.lastError();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return count == 0;
    }

    return false;
}

bool DataBase::deleteUnitById(int unitId)
{
    QSqlQuery query;

    query.prepare(R"(
                  UPDATE "Сотрудник"
                  SET "ID_Отдела" = 1
                  WHERE "ID_Отдела" = :departmentId;
                  )");
    query.bindValue(":departmentId", unitId);

    if (!query.exec()) {
        qDebug() << "Ошибка при обновлении сотрудников:" << query.lastError().text();
        return false;
    }

    query.prepare(R"(
                  DELETE FROM "Отдел"
                  WHERE "ID" = :departmentId;
                  )");
    query.bindValue(":departmentId", unitId);

    if (!query.exec()) {
        qDebug() << "Ошибка при удалении отдела:" << query.lastError().text();
        return false;
    }

    qDebug() << "Отдел и его сотрудники успешно обновлены и удалены.";
    return true;
}


QStringList DataBase::fetchDepartmentNames()
{
    QStringList unitNames;
    QSqlQuery query(db);
    if (query.exec("SELECT Название_Отдела FROM Отдел")) {
        while (query.next()) {
            QString unitName = query.value(0).toString();
            unitNames.append(unitName);
        }
    }
    else
    {
        qWarning() << "Ошибка при выполнении запроса:" << query.lastError().text();
    }
    return unitNames;
}

QStringList DataBase::checkUnique(const QString& login, const QString& phoneNumber)
{
    QStringList errors;
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM Сотрудник WHERE Логин = :login OR Телефон = :phoneNumber");
    query.bindValue(":login", login);
    query.bindValue(":phoneNumber", phoneNumber);

    qDebug() << "Проверка уникальности. Логин:" << login.trimmed() << "Телефон:" << phoneNumber.trimmed();

    if (!query.exec()) {
        qDebug() << "Ошибка проверки уникальности: " << query.lastError().text();
        errors.append("Ошибка проверки уникальности");
        return errors;
    }

    if (query.next() && query.value(0).toInt() > 0) {
        errors.append("Логин или номер телефона уже существует");
    }

    return errors;
}

bool DataBase::insertEmployee(const QString& firstName, const QString& lastName, const QString& MName,
                              const QString& phoneNumber, const QString& role, int departmentId,
                              const QString& login, const QString& password)
{
    QSqlQuery query(db);
    query.prepare(R"(
                  INSERT INTO Сотрудник (Имя, Фамилия, Отчество, Телефон, Роль, ID_Отдела, Логин, Пароль)
                  VALUES (:firstName, :lastName, :MName, :phoneNumber, :role, :departmentId, :login, :password)
                  )");
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":MName", MName);
    query.bindValue(":phoneNumber", phoneNumber);
    query.bindValue(":role", role);
    query.bindValue(":departmentId", departmentId);
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    qDebug() << "Подготовленный запрос: " << query.lastQuery();
    qDebug() << "Параметры:";
    qDebug() << "Имя:" << firstName;
    qDebug() << "Фамилия:" << lastName;
    qDebug() << "Отчество:" << MName;
    qDebug() << "Телефон:" << phoneNumber;
    qDebug() << "Роль:" << role;
    qDebug() << "ID Отдела:" << departmentId;
    qDebug() << "Логин:" << login;
    qDebug() << "Пароль:" << password;

    if (!query.exec()) {
        qDebug() << "Ошибка при вставке сотрудника: " << query.lastError().text();
        return false;
    }

    qDebug() << "Сотрудник успешно добавлен!";
    return true;
}


int DataBase::getUnitId(const QString& unitName) {
    QSqlQuery query(db);
    query.prepare("SELECT ID FROM Отдел WHERE Название_Отдела = :departmentName");
    query.bindValue(":departmentName", unitName);

    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса: " << query.lastError();
        return -1;
    }

    if (!query.next()) {
        qDebug() << "Ошибка: Отдел не найден";
        return -1;
    }

    int departmentId = query.value(0).toInt();
    qDebug() << "ID Отдела:" << departmentId;
    return departmentId;
}


bool DataBase::deleteEmployeeById(int employeeId) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM Сотрудник WHERE ID = :employeeId");
    query.bindValue(":employeeId", employeeId);

    if (!query.exec()) {
        qDebug() << "Error: failed to delete employee -" << query.lastError();
        return false;
    }

    return true;
}

QStringList DataBase::findEmployeeById(int employeeId) {
    QStringList employeeData;
    QSqlQuery query(db);
    query.prepare("SELECT Имя, Фамилия, Отчество, Номер_телефона, Роль, ID_Отдела, Логин FROM Сотрудник WHERE ID = :employeeId");
    query.bindValue(":employeeId", employeeId);

    if (!query.exec()) {
        qDebug() << "Error: failed to find employee -" << query.lastError();
        return employeeData;
    }

    if (query.next()) {
        employeeData << query.value("Имя").toString()
                     << query.value("Фамилия").toString()
                     << query.value("Отчество").toString()
                     << query.value("Номер_телефона").toString()
                     << query.value("Роль").toString()
                     << QString::number(query.value("ID_Отдела").toInt())
                     << query.value("Логин").toString();
    }

    return employeeData;
}


QSqlRecord DataBase::getPersonByID(int personID)
{
    QSqlQuery query(db);
    query.prepare(R"(
                  SELECT Сотрудник.*, Отдел.Название_Отдела
                  FROM Сотрудник
                  JOIN Отдел ON Сотрудник.ID_Отдела = Отдел.ID
                  WHERE Сотрудник.ID = :ID
                  )");
    query.bindValue(":ID", personID);
    if (query.exec() && query.next())
    {
        return query.record();
    }
    qDebug() << "Ошибка при запросе сотрудника:" << query.lastError().text();
    return QSqlRecord();
}

QStringList DataBase::fetchEmployeeNames() {
    QStringList employeeNames;
    QSqlQuery query(db);
    if (query.exec("SELECT Имя, Фамилия FROM Сотрудник")) {
        while (query.next()) {
            QString fullName = query.value("Имя").toString() + " " + query.value("Фамилия").toString();
            employeeNames.append(fullName);
        }
    } else {
        qWarning() << "Ошибка при выполнении запроса:" << query.lastError().text();
    }
    return employeeNames;
}

QSqlTableModel* DataBase::getPeopleTable() {
    QSqlTableModel* model = new QSqlTableModel;
    model->setTable("Сотрудник");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->removeColumn(model->fieldIndex("Логин"));
    model->removeColumn(model->fieldIndex("Пароль"));

    return model;
}

bool DataBase::changePassword(int IDperson, QString password)
{
    QSqlQuery query;
    query.prepare("UPDATE \"Сотрудник\" SET \"Пароль\" = :Password WHERE \"ID\" = :personId");
    query.bindValue(":Password", password);
    query.bindValue(":personId", IDperson);

    if (!query.exec()) {
        qDebug() << "Ошибка при обновлении данных пользователя:" << query.lastError().text();
        return false;
    }

    return true;
}


bool DataBase::changePhone(int IDperson, QString phone)
{
    QSqlQuery query;
    query.prepare("UPDATE \"Сотрудник\" SET \"Телефон\" = :phone WHERE \"ID\" = :personId");
    query.bindValue(":phone", phone);
    query.bindValue(":personId", IDperson);

    if (!query.exec()) {
        qDebug() << "Ошибка при обновлении данных пользователя:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel* DataBase::fetchEmployees() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query(db);

    query.prepare(R"(
                  SELECT Имя, Фамилия, Отчество, Роль, ID_Отдела
                  FROM Сотрудник
                  )");

    if (!query.exec()) {
        qDebug() << "Ошибка при выполнении запроса: " << query.lastError().text();
        delete model;
        return nullptr;
    }

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, "Имя");
    model->setHeaderData(1, Qt::Horizontal, "Фамилия");
    model->setHeaderData(2, Qt::Horizontal, "Отчество");
    model->setHeaderData(3, Qt::Horizontal, "Роль");
    model->setHeaderData(4, Qt::Horizontal, "ID Отдела");

    return model;
}

bool DataBase::insertPurchase(const QString &purchaseName, const QString &description, int cost, int count, int all_sum)
{
    QDate currentDate = QDate::currentDate();

    QSqlQuery query;
    User* session = User::getInstance();

    query.prepare(R"(
                  INSERT INTO Покупка (ID_Сотрудника, Название, Описание, Стоимость, Дата, Количество, Общая_стоимость)
                  VALUES (:employeeId, :name, :description, :cost, :date, :quantity, :totalCost)
                  )");

    query.bindValue(":employeeId", session->getPersonID());
    query.bindValue(":name", purchaseName);
    query.bindValue(":description", description);
    query.bindValue(":cost", cost);
    query.bindValue(":date", currentDate.toString("dd.MM.yyyy"));
    query.bindValue(":quantity", count);
    query.bindValue(":totalCost", all_sum);

    if (!query.exec()) {
        qDebug() << "Ошибка вставки в базу данных: " << query.lastError().text();
        return false;
    }

    updateDepartmentBudget(all_sum);

    return true;
}

QSqlRecord DataBase::getUnitByID(int unitID)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM Отдел WHERE ID = :ID");
    query.bindValue(":ID", unitID);
    if (query.exec() && query.next())
    {
        return query.record();
    }
    qDebug() << "Ошибка при запросе сотрудника:" << query.lastError().text();
    return QSqlRecord();
}

QSqlTableModel* DataBase::getUnitTable() {
    QSqlTableModel* model = new QSqlTableModel;
    model->setTable("Отдел");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    return model;
}

QSqlQueryModel* DataBase::getPurchasesByDepartmentName() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryString;

    User* session = User::getInstance();

    if (session->getRole() == "Сотрудник") queryString = R"(
                                                         SELECT p.ID_Покупки AS 'ID', p.Название AS 'Название', p.Описание AS 'Описание', p.Стоимость AS 'Сумма', p.Дата AS 'Дата', p.Количество AS 'Количество', p.Общая_стоимость AS 'Общая_стоимость'
                                                         FROM Покупка p
                                                         JOIN Сотрудник s ON p.ID_Сотрудника = s.ID
                                                         JOIN Отдел o ON s.ID_Отдела = o.ID
                                                         WHERE o.Название_Отдела = :departmentName
                                                         )";
    else queryString = R"(
                       SELECT p.ID_Покупки AS 'ID',
                       p.Название AS 'Название',
                       p.Описание AS 'Описание',
                       p.Стоимость AS 'Сумма',
                       p.Дата AS 'Дата',
                       o.Название_Отдела AS 'Отдел'
                       FROM Покупка p
                       JOIN Сотрудник s ON p.ID_Сотрудника = s.ID
                       JOIN Отдел o ON s.ID_Отдела = o.ID
                       )";

    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":departmentName", session->getUnit());

    if (!query.exec()) {
        qDebug() << "Error: query execution failed -" << query.lastError();
        delete model;
        return nullptr;
    }

    model->setQuery(query);

    if (model->lastError().isValid()) {
        qDebug() << "Error: model set query failed -" << model->lastError();
        delete model;
        return nullptr;
    }

    return model;
}

bool DataBase::updateMonthlyBudgetById(int departmentId, int newMonthlyBudget) {
    QSqlQuery query;
    query.prepare("UPDATE Отдел SET Бюджет_на_месяц = :newMonthlyBudget WHERE ID = :departmentId");
    query.bindValue(":newMonthlyBudget", newMonthlyBudget);
    query.bindValue(":departmentId", departmentId);

    if (!query.exec()) {
        qDebug() << "Error: update query execution failed -" << query.lastError();
        return false;
    }
    unitUpdatebb(departmentId);
    return true;
}


bool DataBase::unitUpdatebb(int departmentId)
{
    QSqlQuery query;
    query.prepare(R"(
                  SELECT "Общий_Бюджет", "Бюджет_на_месяц"
                  FROM "Отдел"
                  WHERE "ID" = :departmentId;
                  )");
    query.bindValue(":departmentId", departmentId);

    if (!query.exec()) {
        qDebug() << "Ошибка при получении данных бюджета отдела:" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        qDebug() << "Отдел с указанным ID не найден.";
        return false;
    }

    int all_budget = query.value(0).toInt();
    int all_budget_mounth = query.value(1).toInt();

    int new_all_budget = all_budget + all_budget_mounth;

    query.prepare(R"(
                  UPDATE "Отдел"
                  SET "Общий_Бюджет" = :new_all_budget
                  WHERE "ID" = :departmentId;
                  )");
    query.bindValue(":new_all_budget", new_all_budget);
    query.bindValue(":departmentId", departmentId);

    if (!query.exec()) {
        qDebug() << "Ошибка при обновлении общего бюджета отдела:" << query.lastError().text();
        return  false;
    } else {
        qDebug() << "Общий бюджет отдела успешно обновлен.";
        return true;
    }

}



bool DataBase::updateDepartmentBudget(int cost) {
    QSqlQuery query;
    User* session = User::getInstance();
    query.prepare(R"(
                  UPDATE Отдел SET Общий_Бюджет = Общий_Бюджет - :cost WHERE ID = :departmentId
                  )");
    query.bindValue(":cost", cost);
    query.bindValue(":departmentId", getUnitId(session->getUnit()));

    if (!query.exec()) {
        qDebug() << "Error: Update department budget failed -" << query.lastError();
        return false;
    }

    return true;
}


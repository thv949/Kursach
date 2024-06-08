#ifndef DATABASE_H
#define DATABASE_H

#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QSqlTableModel>
#include<QtDebug>
#include "QSqlRecord"
#include "user.h"


class DataBase
{
public:
    DataBase();
    QString checkLoginAndPassword(const QString &login, const QString &password);
    void getUnit();
    QSqlTableModel* getModel();
    int insertUnit(const QString &unit);
    bool isUnitNameAvailable(const QString &unit);
    bool deleteUnitById(int unitId);
    QStringList fetchDepartmentNames();
    QStringList checkUnique(const QString& login, const QString& phone);
    bool insertEmployee(const QString& firstName, const QString& lastName, const QString& MName,
                        const QString& phoneNumber, const QString& role, int departmentId,
                        const QString& login, const QString& password);
    int getUnitId(const QString& unitName);
    bool deleteEmployeeById(int employeeId);
    QStringList findEmployeeById(int employeeId);
    QStringList fetchEmployeeNames();
    QSqlTableModel* getPeopleTable();
    QSqlRecord getPersonByID(int personID);
    bool changePassword(int IDperson, QString password);
    bool changePhone(int IDperson, QString phone);
    QSqlQueryModel* fetchEmployees();
    bool insertPurchase(const QString &purchaseName, const QString &description, int cost, int count, int all_sum);
    QSqlRecord getUnitByID(int unitID);
    QSqlTableModel* getUnitTable();
    QSqlQueryModel* getPurchasesByDepartmentName();
    bool updateMonthlyBudgetById(int departmentId, int newMonthlyBudget);
    bool updateDepartmentBudget(int cost);
    void initializeDatabase();
    bool unitUpdatebb(int departmentId);


private:
    QSqlDatabase db;
    QSqlTableModel* model;
};



#endif // DATABASE_H

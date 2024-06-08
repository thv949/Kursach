#ifndef ACCOUNTANTBUDGETWINDOW_H
#define ACCOUNTANTBUDGETWINDOW_H

//#include <QDialog>
#include <QWidget>
#include "listunitaccountant.h"
#include "database.h"
#include "qsortfilterproxymodel.h"

namespace Ui {
class accountantbudgetwindow;
}

class accountantbudgetwindow : public QWidget
{
    Q_OBJECT

public:
    explicit accountantbudgetwindow(QWidget *parent = nullptr);
    ~accountantbudgetwindow();
    int takeID();
    void showEvent(QShowEvent *event);

private slots:
    void on_change_budget_clicked();

    void on_return_acccc_clicked();

public slots:
    void updatetables();

signals:
    void Return();
    void openaddbudget();
    void giveInformationChange(int);

private:
    Ui::accountantbudgetwindow *ui;
    ListUnitAccountant* _ListUnitAccountant;
    DataBase db;
    QSortFilterProxyModel *proxyModel;

};

#endif // ACCOUNTANTBUDGETWINDOW_H

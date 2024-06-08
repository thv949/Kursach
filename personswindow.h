#ifndef PERSONSWINDOW_H
#define PERSONSWINDOW_H

#include <QWidget>
#include "database.h"
#include <QSqlQueryModel>
#include "qsortfilterproxymodel.h"

namespace Ui {
class PersonsWindow;
}

class PersonsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PersonsWindow(QWidget *parent = nullptr);
    ~PersonsWindow();
    void updateTable();
    void showEvent(QShowEvent *event);


signals:
    void Return();
private slots:
    void on_return_emp_clicked();

private:
    Ui::PersonsWindow *ui;
    void displayEmployees();
    void setupTableView();
    DataBase db;
    QSortFilterProxyModel *proxyModel;
};

#endif // PERSONSWINDOW_H

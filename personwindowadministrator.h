#ifndef PERSONWINDOWADMINISTRATOR_H
#define PERSONWINDOWADMINISTRATOR_H

#include <QWidget>
#include <addpersonadminwindow.h>
#include "qsortfilterproxymodel.h"

namespace Ui {
class PersonWindowAdministrator;
}

class PersonWindowAdministrator : public QWidget
{
    Q_OBJECT

public:
    explicit PersonWindowAdministrator(QWidget *parent = nullptr);
    ~PersonWindowAdministrator();
    void showEvent(QShowEvent *event);
    int takeID();

public slots:
        void updateTable();

signals:
    void Return();
    void openAddPerson();

private slots:
    void on_add_admin_clicked();
    void on_return_admin_clicked();

    void on_del_admin_clicked();

private:
    Ui::PersonWindowAdministrator *ui;
    AddPersonAdminWindow *_AddPersonAdminWindow;
    DataBase db;
    QSortFilterProxyModel *proxyModel;
};

#endif // PERSONWINDOWADMINISTRATOR_H

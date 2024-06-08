#ifndef LISTUNITADMINISTRATOR_H
#define LISTUNITADMINISTRATOR_H

#include <QWidget>
#include "addunitadminwindow.h"
#include "database.h"
#include "qsortfilterproxymodel.h"

namespace Ui {
class ListUnitAdministrator;
}

class ListUnitAdministrator : public QWidget
{
    Q_OBJECT

public:
    explicit ListUnitAdministrator(QWidget *parent = nullptr);
    ~ListUnitAdministrator();

    void showEvent(QShowEvent *event);
    void UpdateTable();
    int takeID();

signals:
    void Return();
    void openAddUnit();

private slots:
    void on_return_adm_clicked();
    void on_add_unit_admin_clicked();
    void on_del_unit_admin_clicked();

public slots:
    void update();

private:
    Ui::ListUnitAdministrator *ui;
    AddUnitAdminWindow *_AddUnitAdminWindow;
    DataBase db;
    QSortFilterProxyModel *proxyModel;
};

#endif // LISTUNITADMINISTRATOR_H

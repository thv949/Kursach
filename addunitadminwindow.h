#ifndef ADDUNITADMINWINDOW_H
#define ADDUNITADMINWINDOW_H

#include <QDialog>
#include "database.h"

namespace Ui {
class AddUnitAdminWindow;
}

class AddUnitAdminWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddUnitAdminWindow(QWidget *parent = nullptr);
    ~AddUnitAdminWindow();

    void applyDefaultStyles();
    void showEvent(QShowEvent *event);

signals:
    void update();

private slots:
    void on_return_adm_clicked();

    void on_add_unit_clicked();

private:
    Ui::AddUnitAdminWindow *ui;
    DataBase db;
};

#endif // ADDUNITADMINWINDOW_H

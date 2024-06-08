#ifndef LISTUNITACCOUNTANT_H
#define LISTUNITACCOUNTANT_H

#include <QDialog>
#include "database.h"

namespace Ui {
class ListUnitAccountant;
}

class ListUnitAccountant : public QDialog
{
    Q_OBJECT

public:
    explicit ListUnitAccountant(QWidget *parent = nullptr);
    ~ListUnitAccountant();

public slots:
    void takeID(int a);

signals:
    void Return();
    void update();

private slots:
    void on_return_acc_clicked();
    void on_highlight_clicked();

private:
    Ui::ListUnitAccountant *ui;
    DataBase db;
    int id;
};

#endif // LISTUNITACCOUNTANT_H

#ifndef PERSONACCOUNT_H
#define PERSONACCOUNT_H

#include <QWidget>
#include "user.h"
#include "database.h"

namespace Ui {
class PersonAccount;
}

class PersonAccount : public QWidget
{
    Q_OBJECT

public:
    explicit PersonAccount(QWidget *parent = nullptr);
    ~PersonAccount();
    void updateInfo();
    void showEvent(QShowEvent *event);
    void clearMessages();

signals:
    void Return();
private slots:
    void on_return_pa_clicked();
    void on_change_psw_clicked();
    void on_change_num_clicked();

private:
    Ui::PersonAccount *ui;
    DataBase db;
    QString _Password;
    QString _Phone;

};

#endif // PERSONACCOUNT_H

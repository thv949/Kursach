#ifndef HOMEWINDOWEMPLOYEE_H
#define HOMEWINDOWEMPLOYEE_H

#include <QWidget>
#include "personaccount.h"
#include "personswindow.h"
#include "buywindow.h"
#include "historywindow.h"
#include "database.h"

namespace Ui {
class HomeWindowEmployee;
}

class HomeWindowEmployee : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWindowEmployee(QWidget *parent = nullptr);
    ~HomeWindowEmployee();

private slots:
    void on_person_account_emp_clicked();

    void on_buy_emp_clicked();

    void on_persons_emp_clicked();

    void on_history_emp_clicked();

    void on_return_emp_clicked();

signals:
    void openPersonAccount();
    void openPersonsWindow();
    void openBuyWindow();
    void openHistoryWindow();
    void Return();


private:
    Ui::HomeWindowEmployee *ui;

    PersonAccount *_PersonAccount;
    PersonsWindow *_PersonsWindow;
    BuyWindow *_BuyWindow;
    HistoryWindow *_HistoryWindow;

};

#endif // HOMEWINDOWEMPLOYEE_H

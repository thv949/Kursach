#ifndef HOMEWINDOWACCOUNTANT_H
#define HOMEWINDOWACCOUNTANT_H

#include <QWidget>
#include "personaccount.h"
#include "personswindow.h"
#include "historywindow.h"
#include "listunitaccountant.h"
#include "accountantbudgetwindow.h"

namespace Ui {
class HomeWindowAccountant;
}

class HomeWindowAccountant : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWindowAccountant(QWidget *parent = nullptr);
    ~HomeWindowAccountant();

signals:
    void openPersonAccount();
    void openPersonsWindow();
    void openHistoryWindow();
    void openaccountantbudgetwindow();
    void Return();

private slots:
    void on_person_account_acc_clicked();

    void on_persons_acc_clicked();

    void on_history_acc_clicked();

    void on_unit_acc_clicked();

    void on_return_accountant_clicked();

private:
    Ui::HomeWindowAccountant *ui;

    PersonAccount *_PersonAccount;
    PersonsWindow *_PersonsWindow;
    HistoryWindow *_HistoryWindow;
    accountantbudgetwindow *_accountantbudgetwindow;
};

#endif // HOMEWINDOWACCOUNTANT_H

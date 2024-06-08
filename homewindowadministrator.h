#ifndef HOMEWINDOWADMINISTRATOR_H
#define HOMEWINDOWADMINISTRATOR_H

#include <QWidget>
#include "personaccount.h"
#include "personwindowadministrator.h"
#include "listunitadministrator.h"

namespace Ui {
class HomeWindowAdministrator;
}

class HomeWindowAdministrator : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWindowAdministrator(QWidget *parent = nullptr);
    ~HomeWindowAdministrator();

private slots:
    void on_person_account_clicked();
    void on_persons_admin_clicked();
    void on_unit_admin_clicked();

    void on_return_ad_clicked();

signals:
    void openPersonAccount();
    void openPersonWindowAdministrator();
    void openListUnitAdministrator();
    void Return();

private:
    Ui::HomeWindowAdministrator *ui;

    // место под что-то там с выходом из домашнего окна
    PersonAccount *_PersonAccount;
    PersonWindowAdministrator *_PersonWindowAdministrator;
    ListUnitAdministrator *_ListUnitAdministrator;

};

#endif // HOMEWINDOWADMINISTRATOR_H

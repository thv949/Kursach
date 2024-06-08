#ifndef ADDPERSONADMINWINDOW_H
#define ADDPERSONADMINWINDOW_H

#include <QDialog>
#include "QComboBox"
#include <QComboBox>
#include <QLineEdit>
#include <QCompleter>
#include <QStringList>
#include <QFormLayout>
#include <QDebug>
#include "database.h"


namespace Ui {
class AddPersonAdminWindow;
}

class AddPersonAdminWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddPersonAdminWindow(QWidget *parent = nullptr);
    ~AddPersonAdminWindow();
    void showEvent(QShowEvent *event);
    void applyEmptyStyles();
    void applyDefStyles();
    void applyErrorStyles(const QStringList &errors);
    QStringList checkInput(const QString& FName, const QString& LName,
                                     const QString& MName, const QString& phone, const QString& unit,
                                     const QString& login, const QString& password, const QString& Role);


signals:
    void update();

private slots:
    void on_return_adm_clicked();

    void on_add_adm_clicked();

private:
    Ui::AddPersonAdminWindow *ui;
    QComboBox *comboBox;
    void setupSearchableComboBox(QComboBox *comboBox);
    DataBase db;

};

#endif // ADDPERSONADMINWINDOW_H

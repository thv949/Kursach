#ifndef HISTORYWINDOW_H
#define HISTORYWINDOW_H

#include <QWidget>
#include "user.h"
#include "database.h"
#include "qsortfilterproxymodel.h"

namespace Ui {
class HistoryWindow;
}

class HistoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryWindow(QWidget *parent = nullptr);
    ~HistoryWindow();
    void showEvent(QShowEvent *event);

signals:
    void Return();

private slots:
    void on_return_emp_clicked();

private:
    Ui::HistoryWindow *ui;
    DataBase db;
    QSortFilterProxyModel *proxyModel;
};

#endif // HISTORYWINDOW_H

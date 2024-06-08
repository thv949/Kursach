#include "personswindow.h"
#include "ui_personswindow.h"
#include "QMessageBox"
#include <QSqlQueryModel>


PersonsWindow::PersonsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonsWindow),
    proxyModel(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);
    setupTableView();
    setWindowTitle("Список сотрудников");

    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    ui->table_persons->setModel(proxyModel);

    connect(ui->Search, &QLineEdit::textChanged, this, [this](const QString &text) {
        proxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
    });

}

PersonsWindow::~PersonsWindow()
{
    delete ui;
}

void PersonsWindow::setupTableView() {

    QHeaderView* horizontalHeader = ui->table_persons->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_persons->setShowGrid(true);
    ui->table_persons->setFrameShape(QFrame::NoFrame);
    ui->table_persons->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_persons->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_persons->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->table_persons->verticalHeader()->setVisible(false);
}

void PersonsWindow::updateTable()
{
    proxyModel->setSourceModel(db.getPeopleTable());
}

void PersonsWindow::displayEmployees() {
    DataBase db;
    QSqlQueryModel* model = db.fetchEmployees();

    if (model) {
        //ui->table_persons->setModel(model);
        proxyModel->setSourceModel(db.getPeopleTable());

    }
    else {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить данные сотрудников");
    }
}

void PersonsWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateTable();
}

void PersonsWindow::on_return_emp_clicked()
{
    emit Return();
    this->close();
}



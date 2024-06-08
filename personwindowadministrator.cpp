#include "personwindowadministrator.h"
#include "ui_personwindowadministrator.h"
#include "QMessageBox"

PersonWindowAdministrator::PersonWindowAdministrator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonWindowAdministrator),
    proxyModel(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);

    _AddPersonAdminWindow = new AddPersonAdminWindow(this);
    setWindowTitle("Список сотрудников");

    connect(this, &PersonWindowAdministrator::openAddPerson, _AddPersonAdminWindow, &AddPersonAdminWindow::exec);
    connect(_AddPersonAdminWindow , &AddPersonAdminWindow::update, this, &PersonWindowAdministrator::updateTable);

    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    ui->tableView->setModel(proxyModel);

    connect(ui->Search, &QLineEdit::textChanged, this, [this](const QString &text) {
        proxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
    });

    //Махинации с таблицей
    QHeaderView* horizontalHeader = ui->tableView->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);
    //
    ui->tableView->setShowGrid(true);
    ui->tableView->setFrameShape(QFrame::NoFrame);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->verticalHeader()->setVisible(false);
}

int PersonWindowAdministrator::takeID()
{
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty()) {
        return -1;
    }

    bool correctColumnSelected = false;
    foreach (const QModelIndex &index, selectedIndexes) {
        if (index.column() == 0) {
            return index.data().toInt();
        }
    }

    if (!correctColumnSelected)
    {
        return -1;
    }
    return -1;
}

void PersonWindowAdministrator::updateTable()
{
    proxyModel->setSourceModel(db.getPeopleTable());
    //ui->tableView->setModel(db.getPeopleTable());
}

void PersonWindowAdministrator::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateTable();
}

PersonWindowAdministrator::~PersonWindowAdministrator()
{
    delete ui;
}

void PersonWindowAdministrator::on_add_admin_clicked()
{
    emit openAddPerson();
}

void PersonWindowAdministrator::on_return_admin_clicked()
{
    emit Return();
    this->close();
}

void PersonWindowAdministrator::on_del_admin_clicked()
{
    int id = takeID();
    if(id != -1)
    {
        if(db.deleteEmployeeById(id) == true)
        {
           updateTable();
        }
        else QMessageBox::information(nullptr, "Ошибка", "Ошибка при удалении отдела!");
    }
}

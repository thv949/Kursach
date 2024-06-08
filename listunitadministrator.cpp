#include "listunitadministrator.h"
#include "ui_listunitadministrator.h"
#include "QMessageBox"

ListUnitAdministrator::ListUnitAdministrator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListUnitAdministrator),
    proxyModel(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);
    _AddUnitAdminWindow = new AddUnitAdminWindow(this);

    setWindowTitle("Отделы");

    connect(this, &ListUnitAdministrator::openAddUnit, _AddUnitAdminWindow, &AddUnitAdminWindow::exec);
    connect(_AddUnitAdminWindow, &AddUnitAdminWindow::update, this, &ListUnitAdministrator::update);

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

ListUnitAdministrator::~ListUnitAdministrator()
{
    delete ui;
}

int ListUnitAdministrator::takeID()
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

void ListUnitAdministrator::UpdateTable()
{
    db.getUnit();
    proxyModel->setSourceModel(db.getModel());
    //ui->tableView->setModel(db.getModel());
}

void ListUnitAdministrator::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    UpdateTable();
}

void ListUnitAdministrator::on_return_adm_clicked()
{
    emit Return();
    this->close();
}

void ListUnitAdministrator::on_add_unit_admin_clicked()
{
    emit openAddUnit();
}

void ListUnitAdministrator::update()
{
    UpdateTable();
}


void ListUnitAdministrator::on_del_unit_admin_clicked()
{
  int id = takeID();
  if(id != -1 && id != 1 )
  {
      if(db.deleteUnitById(id) == true)
      {
         UpdateTable();
      }
      else QMessageBox::information(nullptr, "Ошибка", "Ошибка при удалении отдела!");
  }
}


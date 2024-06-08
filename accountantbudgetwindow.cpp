#include "accountantbudgetwindow.h"
#include "ui_accountantbudgetwindow.h"

accountantbudgetwindow::accountantbudgetwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::accountantbudgetwindow),
    proxyModel(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);
    _ListUnitAccountant = new ListUnitAccountant(this);

    setWindowTitle("Отделы");

    connect(this, &accountantbudgetwindow::openaddbudget, _ListUnitAccountant, &ListUnitAccountant::exec);
    connect(this, &accountantbudgetwindow::giveInformationChange, _ListUnitAccountant, &ListUnitAccountant::takeID);
    connect(_ListUnitAccountant, &ListUnitAccountant::update, this, &accountantbudgetwindow::updatetables);

    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    ui->table_budget->setModel(proxyModel);

    connect(ui->Search, &QLineEdit::textChanged, this, [this](const QString &text) {
        proxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
    });

    QHeaderView* horizontalHeader = ui->table_budget->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_budget->setShowGrid(true);
    ui->table_budget->setFrameShape(QFrame::NoFrame);
    ui->table_budget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_budget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_budget->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->table_budget->verticalHeader()->setVisible(false);

}

accountantbudgetwindow::~accountantbudgetwindow()
{
    delete ui;
}

void accountantbudgetwindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updatetables();
}

int accountantbudgetwindow::takeID()
{
    QModelIndexList selectedIndexes = ui->table_budget->selectionModel()->selectedIndexes();
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

void accountantbudgetwindow::on_change_budget_clicked()
{
    int b = takeID();
    if(b != -1)
    {
        emit giveInformationChange(b);
        emit openaddbudget();
    }
}

void accountantbudgetwindow::on_return_acccc_clicked()
{
    emit Return();
    this->close();
}

void accountantbudgetwindow::updatetables()
{
    proxyModel->setSourceModel(db.getUnitTable());
    //ui->table_budget->setModel(db.getUnitTable());
}

#include "historywindow.h"
#include "ui_historywindow.h"

HistoryWindow::HistoryWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWindow),
    proxyModel(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);

    setWindowTitle("Список оформленных документов");

    QHeaderView* horizontalHeader = ui->tableView->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setShowGrid(true);
    ui->tableView->setFrameShape(QFrame::NoFrame);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->verticalHeader()->setVisible(false);

    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    ui->tableView->setModel(proxyModel);

    connect(ui->Search, &QLineEdit::textChanged, this, [this](const QString &text) {
        proxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
    });
}

HistoryWindow::~HistoryWindow()
{
    delete ui;
}

void HistoryWindow::on_return_emp_clicked()
{
    emit Return();
    this->close();
}

void HistoryWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    User* session = User::getInstance();
    QString f = session->getRole();
    if (f == "Сотрудник" || "Бухгалтерия")
    {
        //ui->tableView->setModel(db.getPurchasesByDepartmentName());
        proxyModel->setSourceModel(db.getPurchasesByDepartmentName());
    }
}

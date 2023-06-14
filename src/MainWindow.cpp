#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Dialog.h"
#include "DatabaseListModel.h"
#include "DatabaseListItemDelegate.h"
#include "DatabaseHolder.h"
#include <QPushButton>

#include <QSqlError>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_addNewButton = new QPushButton(ui->centralwidget);
    m_addNewButton->setFixedSize(40, 40);
    m_addNewButton->setStyleSheet("QPushButton{background: transparent; border: none; image: url(:/icons/plus.png);}");

    m_model = new DatabaseListModel(DatabaseHolder::getInstance().getDatabase(), this);
    ui->treeView->setModel(m_model);

    auto delegate = new DatabaseListItemDelegate(this);
    connect(delegate, &DatabaseListItemDelegate::removeButtonClicked, this,
            [](int mcc, int mnc) { qInfo() << "removeButtonClicked" << mcc << mnc; });

    ui->treeView->setItemDelegate(delegate);

    connect(ui->treeView, &QTreeView::doubleClicked, this, &MainWindow::onTreeViewDoubleClicked);

    connect(m_addNewButton, &QPushButton::clicked, this, &MainWindow::onAddNewClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    m_addNewButton->move(ui->centralwidget->width() - 40 - m_addNewButton->width(),
                         ui->centralwidget->height() - 20 - m_addNewButton->height());
}

void MainWindow::onTreeViewDoubleClicked(const QModelIndex &index)
{
    auto item = static_cast<DatabaseListModelItem *>(index.internalPointer());

    if (item->parentItem()->dataCount() > 0)
    {
        Dialog dialog;
        dialog.setDatabaseModel(m_model);
        dialog.setEditOnlyMode(true);

        dialog.setName(item->data("name").toString());
        dialog.setMCC(item->data("mcc").toString());
        dialog.setMNC(item->data("mnc").toString());

        connect(&dialog, &Dialog::accepted, this,
                [&] { ui->treeView->model()->setData(index, dialog.name(), NameRole); });

        dialog.exec();
    }
}

void MainWindow::onAddNewClicked()
{
    auto dialog = std::make_shared<Dialog>(new Dialog);

    dialog->setDatabaseModel(m_model);

    connect(dialog.get(), &Dialog::accepted, this,
            [this, dialog] { m_model->addItem(dialog->mcc().toInt(), dialog->mnc().toInt(), dialog->name()); });

    dialog->exec();

    dialog.reset();
}

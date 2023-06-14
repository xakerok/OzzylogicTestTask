#pragma once

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class QPushButton;
class DatabaseListModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onTreeViewDoubleClicked(const QModelIndex &index);
    void onAddNewClicked();

private:
    Ui::MainWindow *ui;

    DatabaseListModel *m_model;
    QPushButton *m_addNewButton;
};

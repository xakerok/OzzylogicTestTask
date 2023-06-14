#pragma once

#include <QTreeView>

class DatabaseTreeView : public QTreeView
{
public:
    DatabaseTreeView(QWidget* parent);

protected:
    void leaveEvent(QEvent* e) override;

private:
    QModelIndex mModelIndex;
};

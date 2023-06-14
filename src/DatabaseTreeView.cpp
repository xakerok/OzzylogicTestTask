#include "DatabaseTreeView.h"

DatabaseTreeView::DatabaseTreeView(QWidget *parent) : QTreeView(parent)
{
    setMouseTracking(true);

    connect(this, &QTreeView::entered, this,
            [this](const QModelIndex &index)
            {
                if (isPersistentEditorOpen(mModelIndex))
                    closePersistentEditor(mModelIndex);
                mModelIndex = index;
                openPersistentEditor(mModelIndex);
            });
}

void DatabaseTreeView::leaveEvent(QEvent *e)
{
    QTreeView::leaveEvent(e);

    if (isPersistentEditorOpen(mModelIndex))
        closePersistentEditor(mModelIndex);

    mModelIndex = QModelIndex();
}

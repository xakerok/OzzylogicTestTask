#pragma once

#include <QStyledItemDelegate>

class DatabaseListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DatabaseListItemDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

signals:
    void removeButtonClicked(int mcc, int mnt) const;
};

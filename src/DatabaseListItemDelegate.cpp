#include "DatabaseListItemDelegate.h"

#include "DatabaseListModelItem.h"

#include <QToolButton>

#include <QPainter>
#include <QDebug>

DatabaseListItemDelegate::DatabaseListItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void DatabaseListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    auto item = static_cast<DatabaseListModelItem *>(index.internalPointer());

    if (option.state & QStyle::State_MouseOver)
        painter->fillRect(option.rect, QColor(246, 247, 252));
    else
        painter->fillRect(option.rect, Qt::white);

    if (item->parentItem()->dataCount() == 0)
    {
        QRect arrowRect(option.rect.x() + 5, option.rect.y() + 10, 10, 10);

        if (item->childCount() != 0)
        {
            if (option.state & QStyle::State_Open)
            {
                if (option.state & QStyle::State_MouseOver)
                    painter->drawPixmap(arrowRect, QIcon(":/icons/arrow/down-hover.svg").pixmap(arrowRect.size()));
                else
                    painter->drawPixmap(arrowRect, QIcon(":/icons/arrow/down-default.svg").pixmap(arrowRect.size()));
            }
            else
            {
                if (option.state & QStyle::State_MouseOver)
                    painter->drawPixmap(arrowRect, QIcon(":/icons/arrow/right-hover.svg").pixmap(arrowRect.size()));
                else
                    painter->drawPixmap(arrowRect, QIcon(":/icons/arrow/right-default.svg").pixmap(arrowRect.size()));
            }
        }

        QRect flagRect(option.rect.x() + 20, option.rect.y() + 7, 16, 16);
        painter->drawPixmap(flagRect, QPixmap(":/icons/Countries/" + item->data("code").toString()));

        QFont f("Arial");
        f.setPixelSize(14);
        f.setBold(true);
        painter->setFont(f);

        QRect nameRect(option.rect.x() + 41, option.rect.y(), option.rect.width() - 86, option.rect.height());
        auto text = painter->fontMetrics().elidedText(item->data("name").toString(), Qt::ElideRight, nameRect.width());
        painter->drawText(nameRect, Qt::TextSingleLine | Qt::AlignVCenter, text);
    }
    else
    {
        auto name = item->data("name").toString();
        auto mcc  = item->data("mcc").toString();
        auto mnc  = item->data("mnc").toString();

        QRect operatorImgRect(option.rect.x(), option.rect.y() + 7, 16, 16);

        painter->drawPixmap(operatorImgRect, QPixmap(":/icons/Operators/" + QString("%1_%2.png").arg(mcc, mnc)));

        QFont f("Arial");
        f.setPixelSize(14);
        painter->setFont(f);

        QRect nameRect(option.rect.x() + 21, option.rect.y(), option.rect.width() - 86, option.rect.height());
        auto text = painter->fontMetrics().elidedText(QString("%1 (%2, %3)").arg(name, mcc, mnc), Qt::ElideRight,
                                                      nameRect.width());
        painter->drawText(nameRect, Qt::TextSingleLine | Qt::AlignVCenter, text);
    }
}

QSize DatabaseListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return {0, 30};
}

QWidget *DatabaseListItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
    auto item = static_cast<DatabaseListModelItem *>(index.internalPointer());

    if (item->parentItem()->dataCount() == 0)
        return nullptr;

    auto btn = new QToolButton(parent);
    btn->setStyleSheet("QToolButton{background: transparent; border: none; image: url(:/icons/close.svg);}");
    connect(btn, &QAbstractButton::clicked, this,
            [this, index]
            {
                auto item = static_cast<DatabaseListModelItem *>(index.internalPointer());
                emit removeButtonClicked(item->data("mcc").toInt(), item->data("mnc").toInt());
            });
    return btn;
}

void DatabaseListItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                                    const QModelIndex &index) const
{
    QFont f("Arial");
    f.setPixelSize(14);

    QFontMetrics fm(f);

    auto item = static_cast<DatabaseListModelItem *>(index.internalPointer());

    auto name = item->data("name").toString();
    auto mcc  = item->data("mcc").toString();
    auto mnc  = item->data("mnc").toString();

    auto textWidth = fm.horizontalAdvance(QString("%1 (%2, %3)").arg(name, mcc, mnc));

    editor->setGeometry(option.rect.x() + 26 + textWidth, option.rect.y() + 7, 16, 16);
}

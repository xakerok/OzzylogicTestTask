#pragma once

#include <QDialog>

namespace Ui
{
class Dialog;
}

class DatabaseListModel;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void setEditOnlyMode(bool isEditMode);

    void setDatabaseModel(const DatabaseListModel *model);

    void setName(const QString &name);
    void setMCC(const QString &mcc);
    void setMNC(const QString &mnc);

    QString name() const;
    QString mcc() const;
    QString mnc() const;

private:
    void updateOperatorImage();

private:
    Ui::Dialog *ui;

    const DatabaseListModel *m_model;
};

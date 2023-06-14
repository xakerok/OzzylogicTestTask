#include "Dialog.h"
#include "ui_Dialog.h"

#include "DatabaseListModel.h"

#include <QFile>

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    connect(ui->saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    connect(ui->lineEditMCC, &QLineEdit::textChanged, this, &Dialog::updateOperatorImage);
    connect(ui->lineEditMNC, &QLineEdit::textChanged, this, &Dialog::updateOperatorImage);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setEditOnlyMode(bool isEditMode)
{
    ui->lineEditMCC->setReadOnly(isEditMode);
    ui->lineEditMNC->setReadOnly(isEditMode);
}

void Dialog::setDatabaseModel(const DatabaseListModel *model)
{
    m_model = model;

    connect(ui->lineEditMCC, &QLineEdit::textChanged, this,
            [this](const QString &text)
            {
                auto path = QString(":/icons/Countries/%1.png").arg(m_model->getCode(text.toInt()));
                if (QFile::exists(path))
                    ui->flagImageLabel->setPixmap(QPixmap(path));
                else
                    ui->flagImageLabel->clear();
            });
}

void Dialog::setName(const QString &name)
{
    ui->lineEditName->setText(name);
}

void Dialog::setMCC(const QString &mcc)
{
    ui->lineEditMCC->setText(mcc);
}

void Dialog::setMNC(const QString &mnc)
{
    ui->lineEditMNC->setText(mnc);
}

QString Dialog::name() const
{
    return ui->lineEditName->text();
}

QString Dialog::mcc() const
{
    return ui->lineEditMCC->text();
}

QString Dialog::mnc() const
{
    return ui->lineEditMNC->text();
}

void Dialog::updateOperatorImage()
{
    auto path = QString(":/icons/Operators/%1_%2.png").arg(ui->lineEditMCC->text(), ui->lineEditMNC->text());

    if (QFile::exists(path))
        ui->operatorImageLabel->setPixmap(QPixmap(path).scaled(ui->operatorImageLabel->size(), Qt::KeepAspectRatio));
    else
        ui->operatorImageLabel->clear();
}

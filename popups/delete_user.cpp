#include "delete_user.h"
#include "ui_delete_user.h"

Delete_user::Delete_user(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Delete_user)
{
    ui->setupUi(this);
}

Delete_user::~Delete_user()
{
    delete ui;
}

QDialogButtonBox* Delete_user::getOkButton(void)
{
    return ui->buttonBox;
}

QString Delete_user::getMail() const
{
    return mail;
}

QString Delete_user::getUtinfo() const
{
    return utinfo;
}

void Delete_user::on_buttonBox_accepted()
{
    this->utinfo = ui->lineEdit_utinfo_delete->text();
    this->mail = ui->lineEdit_mail_delete->text();
}


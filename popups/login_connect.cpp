#include "login_connect.h"
#include "ui_login_connect.h"

Login_connect::Login_connect(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login_connect)
{
    ui->setupUi(this);
    ui->lineEdit_utinfo->setFocus();
}

Login_connect::~Login_connect()
{
    delete ui;
}

QPushButton* Login_connect::getOkPushButton(void)
{
    return ui->pushButton_ok;
}

QPushButton* Login_connect::getAnnulPushButton(void)
{
    return ui->pushButton_Annuler;
}

void Login_connect::setUser(Utilisateur *p_user)
{
    this->user = p_user;
}



void Login_connect::on_pushButton_ok_clicked()
{
    this->user->setUtinfo(ui->lineEdit_utinfo->text());
    this->user->setMdp(ui->lineEdit_mdp->text());
}


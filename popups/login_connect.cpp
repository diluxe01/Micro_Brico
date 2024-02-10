#include "login_connect.h"
#include "ui_login_connect.h"

Login_connect::Login_connect(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login_connect)
{
    ui->setupUi(this);
}

Login_connect::~Login_connect()
{
    delete ui;
}

QDialogButtonBox* Login_connect::getOkButton(void)
{
    return ui->buttonBox;
}

void Login_connect::setUser(Utilisateur *p_user)
{

    this->user = p_user;
}

void Login_connect::on_buttonBox_clicked(QAbstractButton *button)
{
    this->user->setEmail(ui->lineEdit_mail->text());
    this->user->setMdp(ui->lineEdit_mdp->text());

}


void Login_connect::on_buttonBox_accepted()
{

}


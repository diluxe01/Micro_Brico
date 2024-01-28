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

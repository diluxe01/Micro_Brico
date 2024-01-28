#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect_db.h"
#include "datamodel.h"
#include <QDialogButtonBox>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->getUsers->setEnabled(true);
    //Test the connection between signals and slots
    //mgmt = Management::Management();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
void MainWindow::activate_privilege()
{

}
*/
void MainWindow::test_slot_called()
{
    qDebug() << "Hey there! ;)";
}

void MainWindow::on_popupaddUser_destroyed()
{
    qDebug() << "destroyed popup! ;)";
    this->p_popupAddUser->deleteInstance();
}

void MainWindow::on_popupaddUser_ok()
{
    qDebug() << "OK popup! ;)";
    this->mgmt.addUserToDB();
    this->p_popupAddUser->deleteInstance();
}

void MainWindow::on_addUser_clicked()
{
    this->p_popupAddUser = popupAddUsers::GetInstance();
    this->p_popupAddUser->setUser(this->mgmt.getNewUserRef());
    this->p_popupAddUser->show();
    QObject::connect(this->p_popupAddUser->getOkButton(), &QDialogButtonBox::accepted, this, &MainWindow::on_popupaddUser_ok);
    QObject::connect(this->p_popupAddUser->getOkButton(), &QDialogButtonBox::rejected, this, &MainWindow::on_popupaddUser_destroyed);

}

void MainWindow::clearUserList(void)
{
    for(const auto& elem : this->userList)
    {
        delete (elem);
    }
    this->userList.clear();
}

void MainWindow::on_getUsers_clicked()
{
    /* refresh the user list by cleaning and loading it again */
    this->clearUserList();

    g_connect_db.select_all_users(&this->userList);

    qDebug() << "Iterate: ";

    for(const auto& toto : this->userList)
    {
        qDebug()  << toto->ToString() ;
    }

    MainWindow::refreshScrollArea();
}

void MainWindow::refreshScrollArea(void)
{
    this->ui->listWidget->clear();
    for(const auto& toto : this->userList)
    {
        new QListWidgetItem(toto->ToString(), this->ui->listWidget);
    }
}

void MainWindow::on_actionNouvel_Utilisateur_triggered()
{
    this->p_popupAddUser = popupAddUsers::GetInstance();
    this->p_popupAddUser->setUser(this->mgmt.getNewUserRef());
    this->p_popupAddUser->show();
    QObject::connect(this->p_popupAddUser->getOkButton(), &QDialogButtonBox::accepted, this, &MainWindow::on_popupaddUser_ok);
    QObject::connect(this->p_popupAddUser->getOkButton(), &QDialogButtonBox::rejected, this, &MainWindow::on_popupaddUser_destroyed);
}


void MainWindow::on_actionSe_connecter_triggered()
{
    this->p_loginConnect = new (Login_connect);
    this->p_loginConnect->show();
}


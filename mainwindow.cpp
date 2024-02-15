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

void MainWindow::add_user_to_DB(void)
{
    g_connect_db.add_user(&this->new_user);

    qDebug() << "MGMT: AddUser to DB;)";
}

//---------------------------------------------------------
// vvvvvv POPUP ADD USER SECTION vvvvvv

void MainWindow::on_popupaddUser_destroyed()
{
    qDebug() << "destroyed popup! ;)";
    this->p_popupAddUser->deleteInstance();
}

void MainWindow::on_popupaddUser_ok()
{
    qDebug() << "OK popup! ;)";
    this->add_user_to_DB();
    this->p_popupAddUser->deleteInstance();
}


void MainWindow::on_actionNouvel_Utilisateur_triggered()
{
    this->p_popupAddUser = popupAddUsers::GetInstance();
    this->p_popupAddUser->setUser(&(this->new_user));
    // check if a user is connected when creating a new account, and if so, get its privilege
    if (this->login_user.getIs_logged_on())
    {
        this->p_popupAddUser->setCaller_privilege(this->login_user.getPrivilege());

        if (this->login_user.getPrivilege() == E_admin){
            qDebug() << "ADMIN";
        }
        else
        {
            qDebug() << "BASIC";
        }

    }
    this->p_popupAddUser->show_wrapper();
    QObject::connect(this->p_popupAddUser->getOkButton(), &QDialogButtonBox::accepted, this, &MainWindow::on_popupaddUser_ok);
    QObject::connect(this->p_popupAddUser->getOkButton(), &QDialogButtonBox::rejected, this, &MainWindow::on_popupaddUser_destroyed);
}

// ^^^^^^ POPUP ADD USER SECTION ^^^^^^
//---------------------------------------------------------
// vvvvvv POPUP LOGIN SECTION vvvvvv


void MainWindow::on_popupLogin_destroyed()
{
    qDebug() << "destroyed popup! ;)";
    delete (this->p_loginConnect);
}

void MainWindow::on_popupLogin_ok()
{
    qDebug() << "OK popup! ;)";
    if (g_connect_db.is_user_identified(&this->login_user))
    {
        this->ui->tabWidget->setEnabled(true);
        this->ui->TAB_ges_user->setEnabled(true);
        this->ui->TAB_ges_resa->setEnabled(true);
        this->login_user.setIs_logged_on(true);
        this->login_user.setToken(g_connect_db.get_unique_token());

        qDebug() << "token: " << this->login_user.getToken();
        g_connect_db.update_user_token_on_db(&this->login_user);
    }
    else
    {
        this->login_user.setIs_logged_on(false);
    }
    delete (this->p_loginConnect);
}

void MainWindow::on_actionSe_connecter_triggered()
{
    this->p_loginConnect = new (Login_connect);
    this->p_loginConnect->setUser(&this->login_user);
    this->p_loginConnect->show();
    QObject::connect(this->p_loginConnect->getOkButton(), &QDialogButtonBox::accepted, this, &MainWindow::on_popupLogin_ok);
    QObject::connect(this->p_loginConnect->getOkButton(), &QDialogButtonBox::rejected, this, &MainWindow::on_popupLogin_destroyed);
}

// ^^^^^^ POPUP LOGIN SECTION ^^^^^^
//---------------------------------------------------------
// vvvvvv MAIN WINDOW "Gestion Utilisateur" SECTION vvvvvv
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

    MainWindow::refreshScrollArea();
}

void MainWindow::activateWidgets(bool)
{

}

// ^^^^^^ MAIN WINDOW "Gestion Utilisateur" ^^^^^^
//---------------------------------------------------------

void MainWindow::refreshScrollArea(void)
{
    this->ui->listWidget->clear();
    for(const auto& toto : this->userList)
    {
        new QListWidgetItem(toto->ToString(), this->ui->listWidget);
    }
}


//---------------------------------------------------------
// vvvvvv POPUP DELETE_USER SECTION vvvvvv

void MainWindow::on_deleteUser_clicked()
{
    this->p_popupDeleteUser = new (Delete_user);
    this->p_popupDeleteUser->show();
    QObject::connect(this->p_popupDeleteUser->getOkButton(), &QDialogButtonBox::accepted, this, &MainWindow::on_popupDelete_ok);
    QObject::connect(this->p_popupDeleteUser->getOkButton(), &QDialogButtonBox::rejected, this, &MainWindow::on_popupDelete_destroyed);
}


void MainWindow::on_popupDelete_destroyed()
{
    delete (this->p_popupDeleteUser);
}

void MainWindow::on_popupDelete_ok()
{
    Utilisateur tmp_user;
    tmp_user.setEmail(this->p_popupDeleteUser->getMail());
    tmp_user.setUtinfo(this->p_popupDeleteUser->getUtinfo());
    g_connect_db.delete_user(&tmp_user);

    delete (this->p_popupDeleteUser);
}
// ^^^^^^ POPUP DELETE_USER SECTION ^^^^^^
//---------------------------------------------------------


#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect_db.h"
#include "datamodel.h"
#include <QDialogButtonBox>
#include <QListWidgetItem>
#include <QTimer>
#include "logger/logbrowser.h"
#include <QPointer>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QCloseEvent>
#include <QMessageBox>

QPointer<LogBrowser> logBrowser;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(logBrowser)
        logBrowser->outputMessage( type,msg );
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Init of the connection status slot
    this->login_user.setIs_logged_on(true);
    update_connection_status(false);
    connect(&g_connect_db, &Connect_db::log_value_changed, this, &MainWindow::update_connection_status);

    // Init of the logger connected to the application slot
    logBrowser = new LogBrowser;
    qInstallMessageHandler(myMessageOutput);
    connect(logBrowser, &LogBrowser::sendMessage, this, &MainWindow::log_stuffs, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
    logBrowser->close();
    delete logBrowser;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    // QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Gestion Reservations microBrico",
    //                                                            tr("Are you sure?\n"),
    //                                                            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
    //                                                            QMessageBox::Yes);
    // if (resBtn != QMessageBox::Yes) {
    //     event->ignore();
    // } else {
    //     event->accept();
        delete this;
    // }
}

void MainWindow::add_user_to_DB(void)
{
    g_connect_db.add_user(&this->new_user);

    qDebug() << "MGMT: AddUser to DB;)";
}
void MainWindow::on_actionAfficher_les_logs_triggered()
{
    logBrowser->show();
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
    if (g_connect_db.connect_user(&this->login_user))
    {
        this->login_user.setToken(g_connect_db.get_unique_token());
        g_connect_db.update_user_token_on_db(&this->login_user);
    }
    else
    {
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

void MainWindow::on_actionSe_d_connecter_triggered()
{
    update_connection_status(false);
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

    g_connect_db.select_all_users(&this->login_user, &this->userList);

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


//---------------------------------------------------------
// vvvvvv SLOTS SECTION vvvvvv
void MainWindow::update_connection_status(bool is_user_logged)
{
    is_user_logged = 1;
    if (is_user_logged != this->login_user.getIs_logged_on())
    {
        if (is_user_logged)
        {
            qInfo() << "Vous êtes maintenant connecté.";
        }
        else
        {
            qInfo() << "Vous êtes maintenant déconnecté.";
        }
        this->login_user.setIs_logged_on(is_user_logged);
        this->ui->tabWidget->setEnabled(is_user_logged);
        this->ui->TAB_ges_user->setEnabled(is_user_logged);
        this->ui->TAB_ges_kits->setEnabled(is_user_logged);
        this->ui->actionSe_connecter->setEnabled(!is_user_logged);
        this->ui->actionSe_d_connecter->setEnabled(is_user_logged);
        this->ui->listWidget->clear();
    }
}


void MainWindow::log_stuffs(QtMsgType type, const QString &msg)
{
    QDateTime date ;
    QString date_str = date.currentDateTime().toString();
    switch (type) {
    case QtInfoMsg:
        this->ui->textEditLogs->append(date_str +" : "+ msg);
        break;

    case QtWarningMsg:
        this->ui->textEditLogs->append(date_str +tr(" : — WARNING: %1").arg(msg));
        break;

    case QtCriticalMsg:
        this->ui->textEditLogs->append(date_str +tr(" : — CRITICAL: %1").arg(msg));
        break;

    case QtFatalMsg:
        this->ui->textEditLogs->append(date_str +tr(" : — FATAL: %1").arg(msg));
        break;
    }
}

// ^^^^^^ SLOTS SECTION ^^^^^^
//---------------------------------------------------------

//---------------------------------------------------------
// vvvvvv POPUP DELETE_USER SECTION vvvvvv


void MainWindow::on_pushButton_addkit_clicked()
{
    this->p_popupAddKit = new (PoppupAddKit);
    this->p_popupAddKit->show();
    QObject::connect(this->p_popupAddKit->getOkButton(), &QPushButton::clicked, this, &MainWindow::on_popupAddKit_ok);
    QObject::connect(this->p_popupAddKit->getCancelButton(), &QPushButton::clicked, this, &MainWindow::on_popupAddKit_destroyed);
}


void MainWindow::on_popupAddKit_destroyed()
{
    this->p_popupAddKit->close();
    delete (this->p_popupAddKit);
}

void MainWindow::on_popupAddKit_ok()
{
    Utilisateur tmp_user;
    Kit * p_kit;
    p_kit = this->p_popupAddKit->get_kit_from_form();
    if (p_kit == NULL)
    {

    }
    else
    {
        this->p_popupAddKit->close();
        delete (this->p_popupAddKit);
    }
}
// ^^^^^^ POPUP DELETE_USER SECTION ^^^^^^
//---------------------------------------------------------



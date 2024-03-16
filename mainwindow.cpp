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

    // Set active user in g_db_connect
    g_connect_db.setActiveUser(&(this->login_user));

    // Init date Edit in resa tab to current date
    QDate maxDate;
    maxDate.setDate(9999,12,30);
    ui->dateEdit_deb_resa->setDate(QDate::currentDate());
    ui->dateEdit_deb_resa->setDateRange(QDate::currentDate(), maxDate);

    // Init of the connection status slot
    this->login_user.setIs_logged_on(true);
    update_connection_status(false);
    connect(&g_connect_db, &Connect_db::log_value_changed, this, &MainWindow::update_connection_status);

    // Init of the logger connected to the application slot
    logBrowser = new LogBrowser;
    qInstallMessageHandler(myMessageOutput);
    connect(logBrowser, &LogBrowser::sendMessage, this, &MainWindow::log_stuffs, Qt::QueuedConnection);

    // Init of Kit display table
    QStringList header_list;
    ui->tableWidget_kit->setRowCount(0);
    ui->tableWidget_kit->setColumnCount(6);
    header_list.append("Nom");
    header_list.append("Code");
    header_list.append("Caution");
    header_list.append("Date achat");
    header_list.append("Prix d'achat");
    header_list.append("En Panne?");
    ui->tableWidget_kit->setHorizontalHeaderLabels(header_list);


    // Init of items display table
    header_list.clear();
    ui->tableWidget_item->setRowCount(0);
    ui->tableWidget_item->setColumnCount(2);
    header_list.append("Nom");
    header_list.append("Etat");
    ui->tableWidget_item->setHorizontalHeaderLabels(header_list);

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

void MainWindow::clearKitList(std::vector<Kit*> *i_kitlist)
{
    for(const auto& elem : *i_kitlist)
    {
        delete (elem);
    }
    i_kitlist->clear();
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
    if (g_connect_db.connect_user())
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

    g_connect_db.select_all_users(&this->userList);
    MainWindow::refresh_user_list_table();
}

void MainWindow::activateWidgets(bool)
{

}

void MainWindow::refresh_user_list_table(void)
{
    this->ui->listWidget->clear();
    for(const auto& toto : this->userList)
    {
        new QListWidgetItem(toto->ToString(), this->ui->listWidget);
    }
}
// ^^^^^^ MAIN WINDOW "Gestion Utilisateur" ^^^^^^
//---------------------------------------------------------

//---------------------------------------------------------
// vvvvvv MAIN WINDOW "Gestion Kits" SECTION vvvvvv

void MainWindow::on_pushButton_getkit_clicked()
{
    /* refresh the user list by cleaning and loading it again */
    this->clearKitList(&this->kitList);

    /* if user enterd a code, then search for Kits with this code */
    if (ui->lineEdit_findkitbycode->text()!="")
    {
        g_connect_db.select_kits_by_code(&this->kitList,ui->lineEdit_findkitbycode->text() );
    }
    /* if user enterd a text, then search for Kits with this code */
    else if (ui->lineEdit_findkitbyname->text()!="")
    {
        g_connect_db.select_kits_by_name(&this->kitList,ui->lineEdit_findkitbyname->text() );
    }
    else
    {
        g_connect_db.select_all_kits(&this->kitList);
    }


    MainWindow::refresh_kit_list_table();
}

void MainWindow::refresh_kit_list_table(void)
{
    vector<Kit*>::iterator it;
    int row = 0;
    this->ui->tableWidget_kit->clearContents();
    for (it = kitList.begin(); it != kitList.end(); it++)
    {
        row++;
        push_back_new_kit_on_table((*it), row);
    }
}

void MainWindow::push_back_new_kit_on_table(Kit* kit, int row)
{
    ui->tableWidget_kit->setRowCount(row);
    QTableWidgetItem* p_widget_item_nom;
    QTableWidgetItem* p_widget_item_code;
    QTableWidgetItem* p_widget_item_caution;
    QTableWidgetItem* p_widget_item_date;
    QTableWidgetItem* p_widget_item_prix;
    QTableWidgetItem* p_widget_item_etat;

    // Set first item of last row (name)
    p_widget_item_nom= new QTableWidgetItem(kit->getNom());
    ui->tableWidget_kit->setItem(row-1, 0, p_widget_item_nom);

    // Set second item of last row (code)
    p_widget_item_code= new QTableWidgetItem(kit->getCode());
    ui->tableWidget_kit->setItem(row-1, 1, p_widget_item_code);

    // Set third item of last row (caution)
    p_widget_item_caution= new QTableWidgetItem(kit->getCaution().getStringValue());
    ui->tableWidget_kit->setItem(row-1, 2, p_widget_item_caution);

    // Set fourth item of last row (date)
    QString date = kit->getDate_achat().toString("dd.MM.yyyy");
    p_widget_item_date= new QTableWidgetItem(date);
    ui->tableWidget_kit->setItem(row-1, 3, p_widget_item_date);

    // Set fifth item of last row (prix)
    p_widget_item_prix= new QTableWidgetItem(kit->getPrix_achat().getStringValue());
    ui->tableWidget_kit->setItem(row-1, 4, p_widget_item_prix);

    // Set sixth item of last row (etat)
    p_widget_item_etat= new QTableWidgetItem(kit->getEn_panne_str());
    ui->tableWidget_kit->setItem(row-1, 5, p_widget_item_etat);
}

void MainWindow::clearItemList(Kit* k)
{
    for(const auto& elem : k->item_list)
    {
        delete (elem);
    }
    k->item_list.clear();
}




void MainWindow::on_tableWidget_kit_cellClicked(int row, int column)
{

    Kit* k = this->kitList[row];
    if (k->item_list.empty())
    {
        clearItemList(k);
        g_connect_db.select_items_by_kit(k);
    }
    refresh_item_list_table(k);
    GESKIT_refresh_descritption(k);
}

void MainWindow::GESKIT_refresh_descritption(Kit* kit)
{
    this->ui->textEdit_detailsKit->setText(kit->getTexte_libre());
    this->ui->textEdit_descritpionKit->setText(kit->getDescription());
}
/*
This Function updates the item table with every items
associated with the currently selected kit
*/
void MainWindow::refresh_item_list_table(Kit* kit)
{
    vector<Item*>::iterator it;
    int row = 0;
    this->ui->tableWidget_item->clearContents();
    for (it = kit->item_list.begin(); it != kit->item_list.end(); it++)
    {
        row++;
        push_back_new_item_on_table((*it), row);
    }
}

void MainWindow::push_back_new_item_on_table(Item* item, int row)
{
    ui->tableWidget_item->setRowCount(row);
    QTableWidgetItem* p_widget_item_name;
    QTableWidgetItem* p_widget_item_etat;
    // Set first item of last row (name)
    p_widget_item_name= new QTableWidgetItem(item->getName());
    ui->tableWidget_item->setItem(row-1, 0, p_widget_item_name);

    // Set second item of last row (etat)
    p_widget_item_etat= new QTableWidgetItem(item->getEtatStr());
    ui->tableWidget_item->setItem(row-1, 1, p_widget_item_etat);
}

// ^^^^^^ MAIN WINDOW "Gestion Kits" ^^^^^^
//---------------------------------------------------------



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
// vvvvvv POPUP Add_kit SECTION vvvvvv


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
    std::list<Item*> p_list_item;
    p_kit = this->p_popupAddKit->get_kit_from_form();
    // p_list_item = this->p_popupAddKit->get_items_from_form();
    if (p_kit == NULL)
    {

    }
    else
    {
        g_connect_db.add_kit(p_kit);
        this->p_popupAddKit->close();
        delete (this->p_popupAddKit);
    }
}
// ^^^^^^ POPUP Add_kit SECTION ^^^^^^
//---------------------------------------------------------


//---------------------------------------------------------
// vvvvvv MAIN WINDOW "Gestion Reservation" vvvvvv

void MainWindow::on_dateEdit_deb_resa_userDateChanged(const QDate &date)
{
    if(date.dayOfWeek() == 5) // if friday
    {

        ui->pushButton_reserver->setEnabled(true);
    }
    else
    {
        qInfo()<< "Date de début: Vous devez choisir un vendredi!";
        ui->pushButton_reserver->setEnabled(false);
    }
}



void MainWindow::on_pushButton_getkit_resa_clicked()
{
    /* refresh the user list by cleaning and loading it again */
    this->clearKitList(&this->kitListResa);

    /* if user enterd a code, then search for Kits with this code */
    if (ui->lineEdit_findkitbycode_resa->text()!="")
    {
        g_connect_db.select_kits_by_code(&this->kitListResa,ui->lineEdit_findkitbycode_resa->text() );
    }
    /* if user enterd a text, then search for Kits with this code */
    else if (ui->lineEdit_findkitbyname_resa->text()!="")
    {
        g_connect_db.select_kits_by_name(&this->kitListResa,ui->lineEdit_findkitbyname_resa->text() );
    }
    else
    {
        g_connect_db.select_all_kits(&this->kitListResa);
    }

    qDebug() << "Iterate: ";

    MainWindow::RESA_refresh_kit_list_table();
}

void MainWindow::RESA_refresh_kit_list_table(void)
{
    vector<Kit*>::iterator it;
    int row = 0;
    this->ui->listWidget_resa->clear();
    for(const auto& kit_elem : this->kitListResa)
    {
        new QListWidgetItem(kit_elem->toString(), this->ui->listWidget_resa);
    }
}


// ^^^^^^ MAIN WINDOW "Gestion Reservation" ^^^^^^
//---------------------------------------------------------




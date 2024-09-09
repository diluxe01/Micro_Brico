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
#include <QRegularExpression>


#define DAY_OF_RESA 5
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


    //Init default buttons status
    this->GESKIT_enable_geskit_buttons(false);
    this->GESUSER_enable_GESUSER_buttons(false);
    // Init of the connection status slot
    this->login_user.setIs_logged_on(true);
    update_connection_status(false);
    connect(&g_connect_db, &Connect_db::log_value_changed, this, &MainWindow::update_connection_status);

    // Init of the logger connected to the application slot
    logBrowser = new LogBrowser;
    qInstallMessageHandler(myMessageOutput);
    connect(logBrowser, &LogBrowser::sendMessage, this, &MainWindow::log_stuffs, Qt::QueuedConnection);

    QStringList header_list;
    // Init of User display table
    ui->GESUSER_tableWidget_user->setRowCount(0);
    ui->GESUSER_tableWidget_user->setColumnCount(5);
    header_list.append("UTINFO");
    header_list.append("Prénom");
    header_list.append("Nom");
    header_list.append("Mail");
    header_list.append("Privilège");
    ui->GESUSER_tableWidget_user->setHorizontalHeaderLabels(header_list);
    ui->GESUSER_tableWidget_user->setColumnWidth(1, 150);
    ui->GESUSER_tableWidget_user->setColumnWidth(2, 150);
    ui->GESUSER_tableWidget_user->setColumnWidth(3, 250);

    // Init of Kit display table
    header_list.clear();
    ui->GESKIT_tableWidget_kit->setRowCount(0);
    ui->GESKIT_tableWidget_kit->setColumnCount(6);
    header_list.append("Code");
    header_list.append("Nom");
    header_list.append("Caution");
    header_list.append("Date achat");
    header_list.append("Prix d'achat");
    header_list.append("En Panne?");
    ui->GESKIT_tableWidget_kit->setHorizontalHeaderLabels(header_list);
    ui->GESKIT_tableWidget_kit->setColumnWidth(1, 250);

    // Init of items display table
    header_list.clear();
    ui->GESKIT_tableWidget_item->setRowCount(0);
    ui->GESKIT_tableWidget_item->setColumnCount(2);
    header_list.append("Nom");
    header_list.append("Quantité");
    ui->GESKIT_tableWidget_item->setHorizontalHeaderLabels(header_list);
    ui->GESKIT_tableWidget_item->setColumnWidth(0, 150);
    ui->GESKIT_tableWidget_item->setColumnWidth(1, 70);


    // A SUPPRIMER Connection automatique avec l'utilisateur "admin" à chaque démarrage
    this->p_loginConnect = new (Login_connect);
    this->login_user.setEmail("admin");
    this->login_user.setMdp("admin");
    this-> on_popupLogin_ok();

    // Init date Edit in resa tab to current date
    QDate maxDate;
    maxDate.setDate(9999,12,30);
    this->ui->RESA_calendarWidget->setDateRange(QDate::currentDate(), maxDate);
    this->ui->RESA_calendarWidget->setSelectedDate(this->RESA_get_next_resa_day(QDate::currentDate())); // Set Selected date to next friday
    this->on_RESA_calendarWidget_clicked(this->RESA_get_next_resa_day(QDate::currentDate()));
}

MainWindow::~MainWindow()
{
    delete ui;
    logBrowser->close();
    delete logBrowser;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
        delete this;
}

void MainWindow::on_actionAfficher_les_logs_triggered()
{
    logBrowser->show();
}

void MainWindow::GEN_raise_popup_info(QString msg)
{

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(msg);
    msgBox.setTextFormat(Qt::TextFormat::MarkdownText);
    msgBox.setWindowTitle("Informations");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    qDebug() << msg;
}


void MainWindow::GEN_raise_popup_warning(QString msg)
{

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(msg);
    msgBox.setTextFormat(Qt::TextFormat::MarkdownText);
    msgBox.setWindowTitle("Attention");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    qDebug() << msg;
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
    g_connect_db.add_user(&this->new_user);
    this->p_popupAddUser->deleteInstance();
}

void MainWindow::on_actionNouvel_Utilisateur_triggered()
{
    GESUSER_add_new_user();
}

void MainWindow::on_GESUSER_pushButton_add_user_clicked()
{
    GESUSER_add_new_user();
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
    g_connect_db.connect_user();

    delete (this->p_loginConnect);

    activateWidgets();
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
    clean_HMI();
}

// ^^^^^^ POPUP LOGIN SECTION ^^^^^^
//---------------------------------------------------------
// vvvvvv MAIN WINDOW "Gestion Utilisateur" SECTION vvvvvv

void MainWindow::on_GESUSER_pushButton_getuser_clicked()
{

    this->userListView.clear();

    /* if user entered a name, then search for users with this name*/
    if (ui->GESUSER_lineEdit_finduserbyname->text()!="")
    {
        GESUSER_get_user_by_name(&this->userList, &this->userListView, ui->GESUSER_lineEdit_finduserbyname->text());
    }
    /* if user entered a utinfo, then search for users with this utinfo */
    else if (ui->GESUSER_lineEdit_finduserbyutinfo->text()!="")
    {
        GESUSER_get_user_by_utinfo(&this->userList, &this->userListView, ui->GESUSER_lineEdit_finduserbyutinfo->text());
    }
    else
    {
        /* if user entered nothing, get every available kits */
        for(const auto& elem : this->userList)
        {
            this->userListView.push_back(elem);
        }
    }


    MainWindow::GESUSER_refresh_user_table();
}


void MainWindow::GESUSER_get_user_by_name(std::vector<Utilisateur*> *from_user, std::vector<Utilisateur*> *to_user, QString name)
{
    for(const auto& user_elem : *from_user)
    {
        if (user_elem->getNom().contains(name, Qt::CaseInsensitive))
        {
            to_user->push_back(user_elem);
        }
    }
}

void MainWindow::GESUSER_get_user_by_utinfo(std::vector<Utilisateur*> *from_user, std::vector<Utilisateur*> *to_user, QString utinfo)
{
    for(const auto& user_elem : *from_user)
    {
        if (user_elem->getUtinfo().contains(utinfo, Qt::CaseInsensitive))
        {
            to_user->push_back(user_elem);
        }
    }
}

void MainWindow::GESUSER_add_new_user()
{
    T_user_privilege l_privilege_of_connected_user = E_basic;
    this->p_popupAddUser = popupAddUsers::GetInstance();
    this->p_popupAddUser->setUser(&(this->new_user));
    // check if a user is connected when creating a new account, and if so, get its privilege
    if (this->login_user.getIs_logged_on())
    {
        l_privilege_of_connected_user = this->login_user.getPrivilege();
    }
    this->p_popupAddUser->setCaller_privilege(l_privilege_of_connected_user);
    this->p_popupAddUser->setWindowTitle("Ajout d'un nouvel utilisateur");
    this->p_popupAddUser->show_wrapper();
    QObject::connect(this->p_popupAddUser->getOkButton(), &QDialogButtonBox::accepted, this, &MainWindow::on_popupaddUser_ok);
    QObject::connect(this->p_popupAddUser->getOkButton(), &QDialogButtonBox::rejected, this, &MainWindow::on_popupaddUser_destroyed);
}

void MainWindow::activateWidgets(void)
{
    bool do_enable = false;
    if (login_user.getPrivilege() == E_basic)
    {
        do_enable = false;
    }
    else
    {
        do_enable = true;
    }
    this->ui->TAB_ges_user->setEnabled(do_enable);
    this->ui->TAB_ges_kits->setEnabled(do_enable);
    this->ui->TAB_SORTIE->setEnabled(do_enable);
}

void MainWindow::GESUSER_refresh_user_table(void)
{
    int row = 0;

    for(const auto& user_elem : this->userListView)
    {
        row++;
        GESUSER_push_back_new_user_on_table(user_elem, row);
    }
}


void MainWindow::GESUSER_clear_display()
{
    // Block signal because the clearing of contents when "currentCellChanged" signal is activated causes crash for some reasons
    this->ui->GESUSER_tableWidget_user->blockSignals(true);
    this->ui->GESUSER_tableWidget_user->clearContents();
    this->ui->GESUSER_tableWidget_user->setRowCount(0);
    this->ui->GESUSER_tableWidget_user->blockSignals(false);
}

void MainWindow::GESUSER_enable_GESUSER_buttons(bool i_enable)
{
    this->ui->GESUSER_pushbutton_deleteUser->setEnabled(i_enable);
    this->ui->GESUSER_pushButton_modify_user->setEnabled(i_enable);
}

Utilisateur *MainWindow::GESUSER_get_user_selected()
{
    QList<QTableWidgetSelectionRange> items = this->ui->GESUSER_tableWidget_user->selectedRanges();
    QTableWidgetSelectionRange  selectedRange = items.first();
    int row = selectedRange.topRow();
    Utilisateur* p_user = userListView.at(row);
    return p_user;
}

void MainWindow::GESUSER_refresh_user_list_from_server(std::vector<Utilisateur *> *i_list)
{
    /* clean the kit list and delete objects pointed by element of this list */
    g_utils.clearList(i_list);
    // clear every depending lists
    this->userListView.clear();

    //Get every kits on server
    g_connect_db.select_all_users(i_list);
}

void MainWindow::GESUSER_push_back_new_user_on_table(Utilisateur *user, int row)
{
    uint column_index = 0;
    ui->GESUSER_tableWidget_user->setRowCount(row);
    QTableWidgetItem* p_widget_user_utinfo;
    QTableWidgetItem* p_widget_user_prenom;
    QTableWidgetItem* p_widget_user_nom;
    QTableWidgetItem* p_widget_user_mail;
    QTableWidgetItem* p_widget_user_privilege;
    QString l_privilege;
    // set Utinfo
    p_widget_user_utinfo= new QTableWidgetItem(user->getUtinfo());
    ui->GESUSER_tableWidget_user->setItem(row-1, column_index, p_widget_user_utinfo);
    column_index ++;
    // set Prenom
    p_widget_user_prenom= new QTableWidgetItem(user->getPrenom());
    ui->GESUSER_tableWidget_user->setItem(row-1, column_index, p_widget_user_prenom);
    column_index ++;
    // set Nom
    p_widget_user_nom= new QTableWidgetItem(user->getNom());
    ui->GESUSER_tableWidget_user->setItem(row-1, column_index, p_widget_user_nom);
    column_index ++;
    // set Mail
    p_widget_user_mail= new QTableWidgetItem(user->getEmail());
    ui->GESUSER_tableWidget_user->setItem(row-1, column_index, p_widget_user_mail);
    column_index ++;

    // set Privilege
    if (user->getPrivilege() == E_basic)
    {
        l_privilege = "Utilisateur";
    }
    else
    {
        l_privilege = "Administrateur";
    }
    p_widget_user_privilege= new QTableWidgetItem(l_privilege);
    ui->GESUSER_tableWidget_user->setItem(row-1, column_index, p_widget_user_privilege);
    column_index ++;

}


// ^^^^^^ MAIN WINDOW "Gestion Utilisateur" ^^^^^^
//---------------------------------------------------------

//---------------------------------------------------------
// vvvvvv MAIN WINDOW "Gestion Kits" SECTION vvvvvv

void MainWindow::GESKIT_get_kits_by_code(std::vector<Kit*> *from_kits, std::vector<Kit*> *to_kits, QString code)
{

    for(const auto& kit_elem : *from_kits)
    {
        if (kit_elem->getCode().contains(code, Qt::CaseInsensitive))
        {
            to_kits->push_back(kit_elem);
        }
    }
}

void MainWindow::GESKIT_get_kits_by_name(std::vector<Kit*> *from_kits, std::vector<Kit*> *to_kits, QString code)
{
    for(const auto& kit_elem : *from_kits)
    {
        if (kit_elem->getNom().contains(code, Qt::CaseInsensitive))
        {
            to_kits->push_back(kit_elem);
        }
    }
}

///
/// \brief MainWindow::GESKIT_clear_display
/// Clears every tables and textEdit in GESKIT tab
///
void MainWindow::GESKIT_clear_display(void)
{
    this->ui->GESKIT_textEdit_descritpionKit->clear();
    this->ui->GESKIT_textEdit_detailsKit->clear();
    this->ui->GESKIT_tableWidget_item->clearContents();
    this->ui->GESKIT_tableWidget_item->setRowCount(0);

    // Block signal because the clearing of contents when "currentCellChanged" signal is activated causes crash for some reasons
    this->ui->GESKIT_tableWidget_kit->blockSignals(true);
    this->ui->GESKIT_tableWidget_kit->clearContents();
    this->ui->GESKIT_tableWidget_kit->setRowCount(0);
    this->ui->GESKIT_tableWidget_kit->blockSignals(false);
}

void MainWindow::GESKIT_enable_geskit_buttons(bool i_enable)
{
    bool l_enable = false;
    //Enable buttons only if user is admin
    if (this->login_user.getPrivilege() == E_admin)
    {
        l_enable = i_enable;
        this->ui->GESKIT_pushButton_addkit->setEnabled(true); // always enable this one when admin
    }
    else
    {
        l_enable = false;
        this->ui->GESKIT_pushButton_addkit->setEnabled(false); // always enable this one when admin
    }

    this->ui->GESKIT_pushButton_duplicate_kit->setEnabled(l_enable);
    this->ui->GESKIT_pushButton_modify_kit->setEnabled(l_enable);
}

void MainWindow::GESKIT_refresh_kit_list_from_server(std::vector<Kit*> *i_list)
{
    /* clean the kit list and delete objects pointed by element of this list */
    g_utils.clearList(i_list);
    // clear every depending lists
    this->kitListBasket_view.clear();
    this->kitListGeskit_view.clear();
    this->kitListResa_view.clear();
    this->kitListSortie_kitsOfResaView.clear();

    this->ui->RESA_listWidget_panierResa->clear();
    this->ui->RESA_listWidget_resa->clear();

    //Get every kits on server
    g_connect_db.select_all_kits(i_list);
}
void MainWindow::on_GESKIT_pushButton_getkit_clicked()
{
    this->kitListGeskit_view.clear();
    /* if user enterd a code, then search for Kits with this code */
    if (ui->GESKIT_lineEdit_findkitbycode->text()!="")
    {
        GESKIT_get_kits_by_code(&this->kitList,&this->kitListGeskit_view,ui->GESKIT_lineEdit_findkitbycode->text() );
    }

    /* if user enterd a text, then search for Kits with this code */
    else if (ui->GESKIT_lineEdit_findkitbyname->text()!="")
    {
        GESKIT_get_kits_by_name(&this->kitList,&this->kitListGeskit_view,ui->GESKIT_lineEdit_findkitbyname->text() );
    }
    else
    {        /* if user enterd nothing, get every available kits */
        for(const auto& kit_elem : this->kitList)
        {
            this->kitListGeskit_view.push_back(kit_elem);
        }
    }

    // If user gives an input that leads to 0 matches, then disable duplicate button
    if(this->kitListGeskit_view.size() == 0)
    {
        this->GESKIT_enable_geskit_buttons(false);
    }
    MainWindow::GESKIT_refresh_kit_table();
}

void MainWindow::GESKIT_refresh_kit_table(void)
{
    vector<Kit*>::iterator it;
    int row = 0;

    for(const auto& kit_elem : this->kitListGeskit_view)
    {
        row++;
        GESKIT_push_back_new_kit_on_table(kit_elem, row);
    }
}

void MainWindow::GESKIT_push_back_new_kit_on_table(Kit* kit, int row)
{
    uint column_index = 0;
    ui->GESKIT_tableWidget_kit->setRowCount(row);
    QTableWidgetItem* p_widget_item_nom;
    QTableWidgetItem* p_widget_item_code;
    QTableWidgetItem* p_widget_item_caution;
    QTableWidgetItem* p_widget_item_date;
    QTableWidgetItem* p_widget_item_prix;
    QTableWidgetItem* p_widget_item_etat;

    // Set second item of last row (code)
    p_widget_item_code= new QTableWidgetItem(kit->getCode());
    ui->GESKIT_tableWidget_kit->setItem(row-1, column_index, p_widget_item_code);
    column_index ++;

    // Set first item of last row (name)
    p_widget_item_nom= new QTableWidgetItem(kit->getNom());
    ui->GESKIT_tableWidget_kit->setItem(row-1, column_index, p_widget_item_nom);
    column_index ++;

    // Set third item of last row (caution)
    p_widget_item_caution= new QTableWidgetItem(kit->getCaution().getStringValue());
    ui->GESKIT_tableWidget_kit->setItem(row-1, column_index, p_widget_item_caution);
    column_index ++;

    // Set fourth item of last row (date)
    QString date = kit->getDate_achat().toString("dd.MM.yyyy");
    p_widget_item_date= new QTableWidgetItem(date);
    ui->GESKIT_tableWidget_kit->setItem(row-1, column_index, p_widget_item_date);
    column_index ++;

    // Set fifth item of last row (prix)
    p_widget_item_prix= new QTableWidgetItem(kit->getPrix_achat().getStringValue());
    ui->GESKIT_tableWidget_kit->setItem(row-1, column_index, p_widget_item_prix);
    column_index ++;

    // Set sixth item of last row (etat)
    p_widget_item_etat= new QTableWidgetItem(kit->getEn_panne_str());
    ui->GESKIT_tableWidget_kit->setItem(row-1, column_index, p_widget_item_etat);
    column_index ++;
}






void MainWindow::on_GESKIT_tableWidget_kit_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (currentRow != previousRow)
    {
        Kit* k = this->kitListGeskit_view[currentRow];
        // Get every items associated with kit from server
        if (k->item_list.empty())
        {
            g_connect_db.select_items_by_kit(k);
        }

        GESKIT_enable_geskit_buttons(true);

        GESKIT_refresh_item_list_table(k);
        GESKIT_refresh_descritption(k);
    }
}

///
/// \brief MainWindow::on_GESKIT_tableWidget_kit_cellDoubleClicked
///     Function used to modify Kit easily
///
void MainWindow::on_GESKIT_tableWidget_kit_cellDoubleClicked(int row, int column)
{
    this->on_GESKIT_pushButton_modify_kit_clicked();
}

void MainWindow::GESKIT_refresh_descritption(Kit* kit)
{
    this->ui->GESKIT_textEdit_detailsKit->setText(kit->getTexte_libre());
    this->ui->GESKIT_textEdit_descritpionKit->setText(kit->getDescription());
}
/*
This Function updates the item table with every items
associated with the currently selected kit
*/
void MainWindow::GESKIT_refresh_item_list_table(Kit* kit)
{
    vector<Item*>::iterator it;
    int row = 0;
    this->ui->GESKIT_tableWidget_item->clearContents();
    for (it = kit->item_list.begin(); it != kit->item_list.end(); it++)
    {
        row++;
        GESKIT_push_back_new_item_on_table((*it), row);
    }
}

void MainWindow::GESKIT_push_back_new_item_on_table(Item* item, int row)
{
    uint8_t column_index = 0;
    ui->GESKIT_tableWidget_item->setRowCount(row);

    // Set first item of last row (name)
    QTableWidgetItem* p_widget_item_name = new QTableWidgetItem(item->getName());
    ui->GESKIT_tableWidget_item->setItem(row-1, column_index, p_widget_item_name);
    column_index++;

    // Set second item of last row (Quantity)
    QTableWidgetItem* p_widget_item_quantity= new QTableWidgetItem(QString::number(item->getQuantity()));
    ui->GESKIT_tableWidget_item->setItem(row-1, column_index, p_widget_item_quantity);
    column_index++;
}

Kit * MainWindow::GESKIT_find_kit_by_id(uint id)
{
    for(const auto& kit_elem : this->kitList)
    {
        if (kit_elem->getIdKit() == id)
        {
            return kit_elem;
        }
    }
}


// ^^^^^^ MAIN WINDOW "Gestion Kits" ^^^^^^
//---------------------------------------------------------



//---------------------------------------------------------
// vvvvvv POPUP DELETE_USER SECTION vvvvvv



void MainWindow::on_GESUSER_pushbutton_deleteUser_clicked()
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
void MainWindow::clean_HMI(void)
{
    //Clear every table, list and line edit
    this->ui->GESUSER_lineEdit_finduserbyname->clear();
    this->ui->GESUSER_lineEdit_finduserbyutinfo->clear();
    this->GESKIT_clear_display();
    this->GESUSER_clear_display();
    this->ui->GESKIT_lineEdit_findkitbycode->clear();
    this->ui->GESKIT_lineEdit_findkitbyname->clear();
    this->ui->RESA_lineEdit_findkitbycode->clear();
    this->ui->RESA_lineEdit_findkitbyname->clear();
    this->ui->RESA_lineEdit_resa_email_user->clear();
    this->ui->RESA_listWidget_panierResa->clear();
    this->ui->RESA_listWidget_resa->clear();
    this->ui->RESA_listWidget_resa_currentResa->clear();
    this->ui->RESA_listWidget_resa_kitsOfResa->clear();

    //Clear every vector lists
    g_utils.clearList(&this->userList);
    g_utils.clearList(&this->kitList);
    g_utils.clearList(&this->resaList);

}

void MainWindow::update_connection_status(bool is_user_logged)
{
    if (is_user_logged != this->login_user.getIs_logged_on())
    {
        this->login_user.setIs_logged_on(is_user_logged);
        this->ui->tabWidget->setEnabled(is_user_logged);
        this->ui->TAB_ges_user->setEnabled(is_user_logged);
        this->ui->TAB_ges_kits->setEnabled(is_user_logged);
        this->ui->actionSe_connecter->setEnabled(!is_user_logged);
        this->ui->actionSe_d_connecter->setEnabled(is_user_logged);

        this->GESKIT_enable_geskit_buttons(false);

        this->GESUSER_enable_GESUSER_buttons(false);

        if (is_user_logged == true)
        {
            qInfo() << "Vous êtes maintenant connecté.";
            //Refresh kit list every time a user logs in
            this->GESKIT_refresh_kit_list_from_server(&this->kitList);
            //Refresh user list every time a user logs in
            this->GESUSER_refresh_user_list_from_server(&this->userList);

            //Refresh resa lineEdit_resa_email_user with user name and enable it only if user is admin
            this->ui->RESA_lineEdit_resa_email_user->setText(this->login_user.getEmail());
            if (this->login_user.getPrivilege() == E_admin)
            {
                this->ui->RESA_lineEdit_resa_email_user->setEnabled(true);
            }
            else
            {
                this->ui->RESA_lineEdit_resa_email_user->setEnabled(false);
            }


        }
        else
        {
            qInfo() << "Vous êtes maintenant déconnecté.";
            //Clear lineEdit_resa_email_use
            this->ui->RESA_lineEdit_resa_email_user->setText("");
            this->ui->RESA_pushButton_suppr_resa->setEnabled(false);
            this->ui->SORTIE_pushButton_sortir->setEnabled(false);
        }
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


void MainWindow::on_GESKIT_pushButton_addkit_clicked()
{
    this->p_popupAddKit = new (PoppupAddKit);
    this->p_popupAddKit->setWindowTitle("Ajout d'un nouveau Kit");
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
    if (p_kit == NULL)
    {

    }
    else if (p_kit->getTo_duplicate() == true)
    {
        g_connect_db.add_kit(p_kit);
        this->p_popupAddKit->close();
        delete (this->p_popupAddKit);
        /* refresh the kit list by cleaning and loading it again */
        this->GESKIT_refresh_kit_list_from_server(&this->kitList);

        //Refresh display of kits
        on_GESKIT_pushButton_getkit_clicked();
    }
    else //if (p_kit->getTo_duplicate() == false)
    {
        g_connect_db.update_kit(p_kit);
        this->p_popupAddKit->close();
        delete (this->p_popupAddKit);
        /* refresh the kit list by cleaning and loading it again */
        this->GESKIT_refresh_kit_list_from_server(&this->kitList);

        //Refresh display of kits
        on_GESKIT_pushButton_getkit_clicked();
    }
}


Kit* MainWindow::GESKIT_get_kit_selected()
{
    //First get kit selected by user
    QList<QTableWidgetSelectionRange> items = this->ui->GESKIT_tableWidget_kit->selectedRanges();
    QTableWidgetSelectionRange  selectedRange = items.first();
    int row = selectedRange.topRow();
    Kit* p_kit = kitListGeskit_view.at(row);
    return p_kit;
}

void MainWindow::on_GESKIT_pushButton_duplicate_kit_clicked()
{
    //First get kit selected by user
    Kit* p_kit = GESKIT_get_kit_selected();
    p_kit->setTo_duplicate(true);

    //Open popup pre filled with selected kit informations
    this->p_popupAddKit = new (PoppupAddKit);
    this->p_popupAddKit->set_form_from_kit(p_kit);
    this->p_popupAddKit->setWindowTitle("Duplication du kit : "+ p_kit->getNom());
    this->p_popupAddKit->show();
    QObject::connect(this->p_popupAddKit->getOkButton(), &QPushButton::clicked, this, &MainWindow::on_popupAddKit_ok);
    QObject::connect(this->p_popupAddKit->getCancelButton(), &QPushButton::clicked, this, &MainWindow::on_popupAddKit_destroyed);
}

void MainWindow::on_GESKIT_pushButton_modify_kit_clicked()
{
    //First get kit selected by user
    Kit* p_kit = GESKIT_get_kit_selected();
    p_kit->setTo_duplicate(false);

    //Open popup pre filled with selected kit informations
    this->p_popupAddKit = new (PoppupAddKit);
    this->p_popupAddKit->set_form_from_kit(p_kit);
    this->p_popupAddKit->setWindowTitle("Modification du kit : "+ p_kit->getNom());
    this->p_popupAddKit->show();
    QObject::connect(this->p_popupAddKit->getOkButton(), &QPushButton::clicked, this, &MainWindow::on_popupAddKit_ok);
    QObject::connect(this->p_popupAddKit->getCancelButton(), &QPushButton::clicked, this, &MainWindow::on_popupAddKit_destroyed);

}

// ^^^^^^ POPUP Add_kit SECTION ^^^^^^
//---------------------------------------------------------


//---------------------------------------------------------
// vvvvvv MAIN WINDOW "Gestion Reservation" vvvvvv



///
/// \brief MainWindow::RESA_get_next_resa_day. Seek next "RESA_DAY" relatively to start_date.
/// \param start_date: day of the week from which "RESA_DAY" must be searched
/// \return
///
QDate MainWindow::RESA_get_next_resa_day(QDate start_date)
{
    QDate next_friday;
    int remaining_days_to_resa_day = 0;
    remaining_days_to_resa_day = DAY_OF_RESA - start_date.dayOfWeek();
    if (remaining_days_to_resa_day < 0)// saturday or sunday
    {
        next_friday = start_date.addDays(remaining_days_to_resa_day+7);
    }
    else if (remaining_days_to_resa_day > 0) //every days except friday
    {
        next_friday = start_date.addDays(remaining_days_to_resa_day);
    }
    else
    {
        next_friday = start_date;
    }
    return next_friday;
}

void MainWindow::on_RESA_calendarWidget_clicked(const QDate &date)
{
    if(date.dayOfWeek() == DAY_OF_RESA) // if Day_of_resa
    {

        ui->RESA_pushButton_reserver->setEnabled(true);
        ui->RESA_pushButton_getkit_resa->setEnabled(true);


        //Find out if kits in kit list are already booked
        g_connect_db.set_kit_booked_status(&this->kitList, this->ui->RESA_calendarWidget->selectedDate());


        RESA_refresh_kit_list_table();
        RESA_refresh_basket_kit_list_table();
    }
    else
    {
        qInfo()<< "Date de début: Vous devez choisir un vendredi!";
        //Force date to next friday from selected day
        this->ui->RESA_calendarWidget->setSelectedDate(this->RESA_get_next_resa_day(this->ui->RESA_calendarWidget->selectedDate()));
        this->on_RESA_calendarWidget_clicked(this->ui->RESA_calendarWidget->selectedDate());
    }
}

void MainWindow::on_RESA_pushButton_getkit_resa_clicked()
{


    // Set property "is_in_resa_view"" of items in kitListResa_view to "false"
    for(const auto& kit_elem : this->kitListResa_view)
    {
        kit_elem->setIs_in_resa_view(false);
    }
    this->kitListResa_view.clear();

    /* if user entered a code, then search for Kits with this code */
    if (ui->RESA_lineEdit_findkitbycode->text()!="")
    {
        RESA_get_kits_by_code(&this->kitList,&this->kitListResa_view,ui->RESA_lineEdit_findkitbycode->text() );
    }
    /* if user entered a text, then search for Kits with this code */
    else if (ui->RESA_lineEdit_findkitbyname->text()!="")
    {
        RESA_get_kits_by_name(&this->kitList,&this->kitListResa_view,ui->RESA_lineEdit_findkitbyname->text() );
    }
    else
    {
        /* if user entered nothing, get every available kits */
        for(const auto& kit_elem : this->kitList)
        {
            this->kitListResa_view.push_back(kit_elem);
        }
    }


    // Refresh kits in basket to see if none booked them
    g_connect_db.set_kit_booked_status(&this->kitListResa_view, this->ui->RESA_calendarWidget->selectedDate());
    MainWindow::RESA_refresh_kit_list_table();
}

//Function finding every kit with a specific name in from_kit list, and store those kit in to_kits list
void MainWindow::RESA_get_kits_by_name(std::vector<Kit*> *from_kits, std::vector<Kit*> *to_kits, QString code)
{
    for(const auto& kit_elem : *from_kits)
    {
        if (kit_elem->getIs_in_resa_view())
        {
            //do nothing, already in resa view
        }
        else
        {
            if (kit_elem->getNom().contains(code, Qt::CaseInsensitive))
            {
                to_kits->push_back(kit_elem);
                kit_elem->setIs_in_resa_view(true);
            }
        }
    }
}

//Function finding every kit with a specific code in from_kit list, and store those kit in to_kits list
void MainWindow::RESA_get_kits_by_code(std::vector<Kit*> *from_kits, std::vector<Kit*> *to_kits, QString code)
{

    for(const auto& kit_elem : *from_kits)
    {
        if (kit_elem->getIs_in_resa_view())
        {
            //do nothing, already in resa view
        }
        else
        {
            if (kit_elem->getCode().contains(code, Qt::CaseInsensitive))
            {
                to_kits->push_back(kit_elem);
                kit_elem->setIs_in_resa_view(true);
            }
        }
    }
}


void MainWindow::RESA_refresh_kit_list_table(void)
{
    vector<Kit*>::iterator it;
    int row = 0;
    QBrush brush_booked;
    QBrush brush_free;
    QBrush brush_basket;
    // Define brush to display kit free in kit reservation list
    brush_free.setColor(Qt::GlobalColor::blue);
    brush_free.setStyle(Qt::SolidPattern);
    // Define brush to display kit booked in kit reservation list
    brush_booked.setColor(Qt::GlobalColor::gray);
    brush_booked.setStyle(Qt::SolidPattern);
    // Define brush to display kit in basket
    brush_basket.setColor(Qt::GlobalColor::yellow);
    brush_basket.setStyle(Qt::SolidPattern);
    this->ui->RESA_listWidget_resa->clear();
    for(const auto& kit_elem : this->kitListResa_view)
    {
        QListWidgetItem* p_item = new QListWidgetItem(kit_elem->toString(), this->ui->RESA_listWidget_resa);
        if(kit_elem->getIs_in_basket())
        {
            p_item->setBackground(brush_basket);
        }
        else if (kit_elem->getIs_booked())
        {
            p_item->setBackground(brush_booked);
        }
        else
        {
            p_item->setForeground(brush_free);
        }
    }
}

void MainWindow::on_RESA_listWidget_resa_itemDoubleClicked(QListWidgetItem *item)
{
    int row = this->ui->RESA_listWidget_resa->row(item);
    Kit* p_kit = this->kitListResa_view.at(row);

    if (p_kit->getIs_in_basket())
    {
        //do nothing
    }
    else
    {
        p_kit->setIs_in_basket(true);
        this->kitListBasket_view.push_back(p_kit);
    }
    // Refresh kits booked status
    g_connect_db.set_kit_booked_status(&this->kitListBasket_view, this->ui->RESA_calendarWidget->selectedDate());

    // Refresh list displays
    RESA_refresh_kit_list_table();
    RESA_refresh_basket_kit_list_table();
}

void MainWindow::RESA_refresh_basket_kit_list_table(void)
{
    vector<Kit*>::iterator it;
    QBrush brush_booked;
    QBrush brush_free;
    this->isBasketReadyToBook = true;

    // Define brush to display kit booked in kit reservation list
    brush_booked.setColor(Qt::GlobalColor::gray);
    brush_booked.setStyle(Qt::SolidPattern);
    // Define brush to display kit free in kit reservation list
    brush_free.setColor(Qt::GlobalColor::blue);
    brush_free.setStyle(Qt::SolidPattern);
    this->ui->RESA_listWidget_panierResa->clear();
    for(const auto& kit_elem : this->kitListBasket_view)
    {
        QListWidgetItem* p_item = new QListWidgetItem(kit_elem->toString(), this->ui->RESA_listWidget_panierResa);
        if (kit_elem->getIs_booked())
        {
            p_item->setBackground(brush_booked);
            isBasketReadyToBook = false;
        }
        else
        {
            p_item->setForeground(brush_free);
        }
    }
}


void MainWindow::on_RESA_listWidget_panierResa_itemDoubleClicked(QListWidgetItem *item)
{
    int row = this->ui->RESA_listWidget_panierResa->row(item);
    Kit* p_kit = this->kitListBasket_view.at(row);
    p_kit->setIs_in_basket(false);

    this->ui->RESA_listWidget_panierResa->removeItemWidget(item);//Delete kit from basket view
    delete(item);
    this->kitListBasket_view.erase(this->kitListBasket_view.begin()+row);//Delete kit from basket list

    RESA_refresh_kit_list_table();
    RESA_refresh_basket_kit_list_table();
}




void MainWindow::on_RESA_pushButton_reserver_clicked()
{
    int resa_nb = 0;
    Utilisateur l_user;
    bool has_errors = false;
    QDate start_date;
    start_date = this->ui->RESA_calendarWidget->selectedDate();

    g_connect_db.start_resa();// Start of LOCK

    // Refresh kits in basket to see if none booked them
    g_connect_db.set_kit_booked_status(&this->kitListBasket_view, this->ui->RESA_calendarWidget->selectedDate());
    RESA_refresh_basket_kit_list_table();

    //Retrieve user id
    has_errors = g_connect_db.get_user_by_mail(this->ui->RESA_lineEdit_resa_email_user->text(), &l_user);
    if (has_errors == true)
    {
        this->GEN_raise_popup_warning("Impossible de réserver pour l'utilisateur: **" + this->ui->RESA_lineEdit_resa_email_user->text()+"**");
    }
    else
    {
        // Proceed booking
        if (this->isBasketReadyToBook == true)
        {
            if (this->kitListBasket_view.empty() == false)
            {
                resa_nb = g_connect_db.guess_next_resa_nb();
                for(const auto& kit_elem : this->kitListBasket_view)
                {
                    g_connect_db.add_resa_from_kit(kit_elem, l_user.getId(), start_date, resa_nb );
                    kit_elem->setIs_in_basket(false);
                    kit_elem->setIs_booked(true);
                }
                GEN_raise_popup_info("Votre réservation n° **"+QString::number(resa_nb)+"** est bien prise en compte.");
                this->ui->RESA_listWidget_panierResa->clear();
                this->kitListBasket_view.clear();
                RESA_refresh_kit_list_table();
                on_RESA_pushButton_resa_showResa_clicked();
            }
        }
        else
        {
            this->GEN_raise_popup_warning("Impossible de réserver car un Kit n'est pas disponible à la date souhaitée.");
        }
    }
    g_connect_db.end_resa();// End of LOCK

}


void MainWindow::on_RESA_pushButton_resa_showResa_clicked()
{
    Utilisateur l_user;
    bool has_errors = false;

     g_utils.clearList(&this->resaList);
    //Retrieve user id
    has_errors = g_connect_db.get_user_by_mail(this->ui->RESA_lineEdit_resa_email_user->text(), &l_user);
    g_connect_db.select_resa_by_user(&this->resaList, l_user.getId());
    RESA_refresh_current_resa_list_table();
}


void MainWindow::RESA_refresh_current_resa_list_table(void)
{
    int prev_id_resa = 0;

    this->ui->RESA_listWidget_resa_currentResa->clear();
    for(const auto& resa_elem : this->resaList)
    {
        if (resa_elem->getId_resa() == prev_id_resa)
        {
            //pass
        }
        else
        {
            QListWidgetItem* p_item = new QListWidgetItem(resa_elem->toString(), this->ui->RESA_listWidget_resa_currentResa);
            prev_id_resa = resa_elem->getId_resa();
        }
    }
}


int MainWindow::RESA_find_resa_nb_selected(QListWidgetItem *item)
{
    int resa_nb = 0;
    QRegularExpression re;
    re.setPattern("(\\d*) -");
    QRegularExpressionMatch match;
    match = re.match(item->text());
    if (match.hasMatch())
    {
        resa_nb = match.captured(1).toInt();
    }
    else
    {
        resa_nb = -1;
    }
    return resa_nb;
}

void MainWindow::on_RESA_listWidget_resa_currentResa_itemClicked(QListWidgetItem *item)
{

    Kit * p_kit;
    int resa_nb = 0;

    //clear kit list widget
    this->ui->RESA_listWidget_resa_kitsOfResa->clear();
    this->ui->RESA_pushButton_suppr_resa->setEnabled(true);

    resa_nb = this->RESA_find_resa_nb_selected(item);

    if (resa_nb != -1)
    {
        //iter through resa list to find every reservation with resa_nb and get their associated kits
        for(const auto& resa_elem : this->resaList)
        {
            if (resa_elem->getId_resa() == resa_nb)
            {
                p_kit = GESKIT_find_kit_by_id(resa_elem->getId_kit());
                QListWidgetItem* p_item = new QListWidgetItem(p_kit->toString(), this->ui->RESA_listWidget_resa_kitsOfResa);
            }
        }
    }
}

void MainWindow::on_RESA_pushButton_suppr_resa_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Suppression de réservation.",
                                                               tr("Êtes-vous sûr de vouloir supprimer votre réservation ?"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        // event->ignore();


    } else {

        //clear kit list widget
        this->ui->RESA_listWidget_resa_kitsOfResa->clear();

        int resa_nb = 0;
        resa_nb = this->RESA_find_resa_nb_selected(this->ui->RESA_listWidget_resa_currentResa->currentItem());
        if (resa_nb != -1)
        {
            g_connect_db.delete_resa(resa_nb);
            this->on_RESA_pushButton_resa_showResa_clicked();
        }
    }
}

// ^^^^^^ MAIN WINDOW "Gestion Reservation" ^^^^^^
//---------------------------------------------------------


//---------------------------------------------------------
// vvvvvv MAIN WINDOW "Gestion SORTIES" vvvvvv


void MainWindow::on_SORTIE_pushButton_resa_showResa_clicked()
{
    bool has_errors = false;

    //Delete every kits in "KitsOfResa" view
    this->kitListSortie_kitsOfResaView.clear(); //do not delete pointed kits though
    this->ui->SORTIE_listWidget_resa_kitsOfResa->clear();

    //Delete every resa in "currentResa" view
    g_utils.clearList(&this->resaListSortie_byUser);
    this->ui->SORTIE_listWidget_resa_currentResa->clear();

    //Delete every kits in "kitsOut" view
    g_utils.clearList(&this->sortieList_byUser);
    this->ui->SORTIE_listWidget_kitsOut->clear();


    // Populates "sortie_user" object with user informations if email is in database
    if (this->ui->SORTIE_lineEdit_useremail->text() != "")
    {
        has_errors = g_connect_db.get_user_by_mail(this->ui->SORTIE_lineEdit_useremail->text(), &this->sortie_user);
        if (has_errors == false)
        {
            // Updates resa list
            g_connect_db.select_resa_by_user(&this->resaListSortie_byUser, this->sortie_user.getId());
            SORTIE_refresh_current_resa_list_table();

            // Updates sortie list
            g_connect_db.select_sortie_by_user(&this->sortieList_byUser, this->sortie_user.getId());
            SORTIE_refresh_kitsOut_table();
        }
    }

}

void MainWindow::on_SORTIE_listWidget_resa_currentResa_itemClicked(QListWidgetItem *item)
{
    //Save last selected resa item
    item_lastSelectedResa = item;

    // get resa number from item
    int resa_nb = this->RESA_find_resa_nb_selected(item);

    SORTIE_refresh_kits_of_resa_table(resa_nb);

}

void MainWindow::SORTIE_refresh_kits_of_resa_table(int i_resa_nb)
{
    Kit * p_kit;
    QBrush brush_out_by_sortie_user;
    QBrush brush_out_by_someone_else;
    QBrush brush_available;

    brush_out_by_sortie_user.setColor(Qt::GlobalColor::green);
    brush_out_by_sortie_user.setStyle(Qt::SolidPattern);

    brush_out_by_someone_else.setColor(Qt::GlobalColor::red);
    brush_out_by_someone_else.setStyle(Qt::SolidPattern);

    brush_available.setColor(Qt::GlobalColor::blue);
    brush_available.setStyle(Qt::SolidPattern);

    //clear kit list widget
    this->ui->SORTIE_listWidget_resa_kitsOfResa->clear();
    this->ui->SORTIE_pushButton_sortir->setEnabled(false); // Disable "sortir" to force user to select a kit
    this->kitListSortie_kitsOfResaView.clear();
    if (i_resa_nb != -1)
    {
        //iter through resa list to find every reservation with resa_nb and get their associated kits
        for(const auto& resa_elem : this->resaListSortie_byUser)
        {
            if (resa_elem->getId_resa() == i_resa_nb)
            {

                    p_kit = GESKIT_find_kit_by_id(resa_elem->getId_kit());
                    QListWidgetItem* p_item = new QListWidgetItem(p_kit->toString(), this->ui->SORTIE_listWidget_resa_kitsOfResa);
                    this->kitListSortie_kitsOfResaView.push_back(p_kit);
            }
        }


        //refresh "isOut" and "id_user_out" parameters of kits inside "kitListSortie_kitsOfResaView"
        g_connect_db.set_kit_out_status(&this->kitListSortie_kitsOfResaView);

        //Set background color of every items in "SORTIE_listWidget_resa_kitsOfResa"
        int cnt = 0;
        QListWidgetItem* p_item2;
        for(const auto& kit_elem : this->kitListSortie_kitsOfResaView)
        {
            // get corresponding item of listWidget
            p_item2 = this->ui->SORTIE_listWidget_resa_kitsOfResa->item(cnt);
            // Check if kit is already out
            if (kit_elem->getIs_out() == true)
            {
                //If kit out by "sortie_user" set to green
                if (kit_elem->getId_user_out() == this->sortie_user.getId())
                {
                    p_item2->setBackground(brush_out_by_sortie_user);
                }
                //If kit out by anybody else set to red
                else
                {
                    p_item2->setBackground(brush_out_by_someone_else);
                }
            }
            else
            {
                //If kit is available, set to blue
                p_item2->setForeground(brush_available);
            }
            cnt++;
        }
    }
}

void MainWindow::SORTIE_refresh_kitsOut_table(void)
{
    Kit *p_kit;
    this->ui->SORTIE_listWidget_kitsOut->clear();
    for(const auto& sortie_elem : this->sortieList_byUser)
    {
        p_kit = GESKIT_find_kit_by_id(sortie_elem->getId_kit());
        QListWidgetItem* p_item = new QListWidgetItem(QString::number(sortie_elem->getId_sortie())+" - " + p_kit->getNom()+" (code: " +p_kit->getCode() +")" , this->ui->SORTIE_listWidget_kitsOut);

    }
}


///
/// \brief MainWindow::SORTIE_get_kit_selected
///     Function returning a pointer to the kit selected inside "SORTIE_listWidget_resa_kitsOfResa" widget
///     It also populates the item associated to the kit
/// \return
///
Kit* MainWindow::SORTIE_get_kit_selected()
{
    //First get kit selected by user
    QList<QListWidgetItem*> items = this->ui->SORTIE_listWidget_resa_kitsOfResa->selectedItems();
    int row = this->ui->SORTIE_listWidget_resa_kitsOfResa->row(items.first());
    Kit* p_kit = kitListSortie_kitsOfResaView.at(row);
    g_connect_db.select_items_by_kit (p_kit); // get every items of this kit
    return p_kit;
}

void MainWindow::SORTIE_refresh_current_resa_list_table(void)
{
    int prev_id_resa = 0;

    this->ui->SORTIE_listWidget_resa_currentResa->clear();
    for(const auto& resa_elem : this->resaListSortie_byUser)
    {
        if (resa_elem->getId_resa() == prev_id_resa)
        {
            //pass
        }
        else
        {
            QListWidgetItem* p_item = new QListWidgetItem(resa_elem->toString(), this->ui->SORTIE_listWidget_resa_currentResa);
            prev_id_resa = resa_elem->getId_resa();
        }
    }
}

///
/// \brief MainWindow::on_SORTIE_pushButton_sortir_clicked: Function called when "Sortir" Button is called inside main window
///
void MainWindow::on_SORTIE_pushButton_sortir_clicked()
{
    Kit * l_kit = SORTIE_get_kit_selected();
    this->p_popupSortirResa = new (PopupSortirResa);
    this->p_popupSortirResa->setUser(&this->sortie_user);
    this->p_popupSortirResa->setP_kit(l_kit);
    this->p_popupSortirResa->refresh_source_item_list();
    this->p_popupSortirResa->setWindowTitle(l_kit->getNom());
    this->p_popupSortirResa->show();
    QObject::connect(this->p_popupSortirResa->getSortirButton(), &QPushButton::clicked, this, &MainWindow::on_SORTIE_popupSortirResa_pushSortir);
    QObject::connect(this->p_popupSortirResa->getAnnulerButton(), &QPushButton::clicked, this, &MainWindow::on_SORTIE_popupSortirResa_pushAnnuler);
}

void MainWindow::on_SORTIE_listWidget_resa_kitsOfResa_itemClicked(QListWidgetItem *item)
{

    Kit * l_kit = SORTIE_get_kit_selected();
    //if kit not out, then enable "sortir" button
    if (l_kit->getIs_out() == false)
    {
        this->ui->SORTIE_pushButton_sortir->setEnabled(true);
    }
}

///
/// \brief MainWindow::on_SORTIE_popupSortirResa_pushSortir : Callback called when "Sortir" button is pushed inside popup
/////
void MainWindow::on_SORTIE_popupSortirResa_pushSortir()
{

    if (this->p_popupSortirResa->checkIfOk() == true)
    {
        SORTIE_sortir_kit();
        delete(this->p_popupSortirResa);


        // Updates sortie list
        g_utils.clearList(&this->sortieList_byUser);
        g_connect_db.select_sortie_by_user(&this->sortieList_byUser, this->sortie_user.getId());
        SORTIE_refresh_kitsOut_table();
    }
}

void MainWindow::SORTIE_sortir_kit()
{
    int sortie_nb = 0;
    Utilisateur l_user;
    bool has_errors = false;
    QDate start_date;
    Kit * p_kit = this->p_popupSortirResa->getP_kit();
    has_errors = g_connect_db.get_user_by_mail(this->ui->SORTIE_lineEdit_useremail->text(), &l_user);
    start_date = QDate::currentDate();

    g_connect_db.update_items_quantity_of_kit (p_kit, this->p_popupSortirResa->item_list_dest);

    // Start of LOCK
    g_connect_db.start_sortie();
    sortie_nb = g_connect_db.guess_next_sortie_nb();
    g_connect_db.add_sortie_from_kit(p_kit, l_user.getId(), start_date, sortie_nb);
    // End of LOCK
    g_connect_db.end_sortie();


    p_kit->setIs_out(true);
    GEN_raise_popup_info("Votre Sortie n° **"+QString::number(sortie_nb)+"** est bien prise en compte.");

    // Set item
    this->ui->SORTIE_listWidget_resa_currentResa->setCurrentItem(this->item_lastSelectedResa);
    on_SORTIE_listWidget_resa_currentResa_itemClicked(this->item_lastSelectedResa);


}

///
/// \brief MainWindow::on_SORTIE_popupSortirResa_pushAnnuler: : Callback called when "Annuler" button is pushed inside popup
///
void MainWindow::on_SORTIE_popupSortirResa_pushAnnuler()
{
    delete this->p_popupSortirResa;
}
// ^^^^^^ MAIN WINDOW "Gestion SORTIES" ^^^^^^
//---------------------------------------------------------



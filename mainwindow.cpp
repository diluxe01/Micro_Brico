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


    // A SUPPRIMER Connection automatique avec l'utilisateur "a" à chaque démarrage
    this->p_loginConnect = new (Login_connect);
    this->login_user.setEmail("a");
    this->login_user.setMdp("a");
    this-> on_popupLogin_ok();

    // Init date Edit in resa tab to current date
    QDate maxDate;
    maxDate.setDate(9999,12,30);
    this->ui->calendarWidget->setDateRange(QDate::currentDate(), maxDate);
    this->ui->calendarWidget->setSelectedDate(this->RESA_get_next_resa_day(QDate::currentDate())); // Set Selected date to next friday
    this->on_calendarWidget_clicked(this->RESA_get_next_resa_day(QDate::currentDate()));
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
    qDebug() << "OK popup! ;)";
    this->GESUSER_add_user_to_DB();
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
    g_connect_db.connect_user();

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

void MainWindow::GESUSER_add_user_to_DB(void)
{
    g_connect_db.add_user(&this->new_user);

    qDebug() << "MGMT: AddUser to DB;)";
}
void MainWindow::on_getUsers_clicked()
{
    /* refresh the user list by cleaning and loading it again */
    g_utils.clearList(&this->userList);

    g_connect_db.select_all_users(&this->userList);
    MainWindow::GESUSER_refresh_user_list_table();
}

void MainWindow::activateWidgets(bool)
{

}

void MainWindow::GESUSER_refresh_user_list_table(void)
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

void MainWindow::GESKIT_refresh_kit_list_from_server(std::vector<Kit*> *i_list)
{
    /* clean the kit list and delete objects pointed by element of this list */
    g_utils.clearList(i_list);
    // clear every depending lists
    this->kitListBasket_view.clear();
    this->kitListGeskit_view.clear();
    this->kitListResa_view.clear();
    this->ui->tableWidget_kit->clearContents();

    this->ui->listWidget_panierResa->clear();
    this->ui->listWidget_resa->clear();

    //Get every kits on server
    g_connect_db.select_all_kits(i_list);
}
void MainWindow::on_pushButton_getkit_clicked()
{
    this->kitListGeskit_view.clear();
    this->ui->tableWidget_kit->clearContents();
    /* if user enterd a code, then search for Kits with this code */
    if (ui->lineEdit_findkitbycode->text()!="")
    {
        RESA_get_kits_by_code(&this->kitList,&this->kitListGeskit_view,ui->lineEdit_findkitbycode->text() );
    }

    /* if user enterd a text, then search for Kits with this code */
    else if (ui->lineEdit_findkitbyname->text()!="")
    {
        RESA_get_kits_by_name(&this->kitList,&this->kitListGeskit_view,ui->lineEdit_findkitbyname->text() );
    }
    else
    {        /* if user enterd nothing, get every available kits */
        for(const auto& kit_elem : this->kitList)
        {
            this->kitListGeskit_view.push_back(kit_elem);
        }
    }

    MainWindow::GESKIT_refresh_kit_list_table();
}

void MainWindow::GESKIT_refresh_kit_list_table(void)
{
    vector<Kit*>::iterator it;
    int row = 0;
    this->ui->tableWidget_kit->clearContents();

    for(const auto& kit_elem : this->kitListGeskit_view)
    {
        row++;
        GESKIT_push_back_new_kit_on_table(kit_elem, row);
    }
}

void MainWindow::GESKIT_push_back_new_kit_on_table(Kit* kit, int row)
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



void MainWindow::on_tableWidget_kit_cellClicked(int row, int column)
{

    Kit* k = this->kitList[row];
    if (k->item_list.empty())
    {
        g_utils.clearList(&k->item_list);
        g_connect_db.select_items_by_kit(k);
    }
    GESKIT_refresh_item_list_table(k);
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
void MainWindow::GESKIT_refresh_item_list_table(Kit* kit)
{
    vector<Item*>::iterator it;
    int row = 0;
    this->ui->tableWidget_item->clearContents();
    for (it = kit->item_list.begin(); it != kit->item_list.end(); it++)
    {
        row++;
        GESKIT_push_back_new_item_on_table((*it), row);
    }
}

void MainWindow::GESKIT_push_back_new_item_on_table(Item* item, int row)
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

Kit *MainWindow::GESKIT_find_kit_by_id(uint id)
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
    if (is_user_logged != this->login_user.getIs_logged_on())
    {
        this->login_user.setIs_logged_on(is_user_logged);
        this->ui->tabWidget->setEnabled(is_user_logged);
        this->ui->TAB_ges_user->setEnabled(is_user_logged);
        this->ui->TAB_ges_kits->setEnabled(is_user_logged);
        this->ui->actionSe_connecter->setEnabled(!is_user_logged);
        this->ui->actionSe_d_connecter->setEnabled(is_user_logged);
        this->ui->listWidget->clear();

        if (is_user_logged == true)
        {
            qInfo() << "Vous êtes maintenant connecté.";
            //Refresh kit list every time a user logs in
            this->GESKIT_refresh_kit_list_from_server(&this->kitList);

            //Refresh resa lineEdit_resa_email_use with user name and enable it only if user is admin
            this->ui->lineEdit_resa_email_user->setText(this->login_user.getEmail());
            if (this->login_user.getPrivilege() == E_admin)
            {
                this->ui->lineEdit_resa_email_user->setEnabled(true);
            }
            else
            {
                this->ui->lineEdit_resa_email_user->setEnabled(false);
            }

        }
        else
        {
            qInfo() << "Vous êtes maintenant déconnecté.";
            //Clear lineEdit_resa_email_use
            this->ui->lineEdit_resa_email_user->setText("");
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
        /* refresh the user list by cleaning and loading it again */
        this->GESKIT_refresh_kit_list_from_server(&this->kitList);
    }
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

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    if(date.dayOfWeek() == DAY_OF_RESA) // if Day_of_resa
    {

        ui->pushButton_reserver->setEnabled(true);
        ui->pushButton_getkit_resa->setEnabled(true);


        //Find out if kits in kit list are already booked
        g_connect_db.set_kit_booked_status(&this->kitList, this->ui->calendarWidget->selectedDate());


        RESA_refresh_kit_list_table();
        RESA_refresh_basket_kit_list_table();
    }
    else
    {
        qInfo()<< "Date de début: Vous devez choisir un vendredi!";
        //Force date to next friday from selected day
        this->ui->calendarWidget->setSelectedDate(this->RESA_get_next_resa_day(this->ui->calendarWidget->selectedDate()));
        this->on_calendarWidget_clicked(this->ui->calendarWidget->selectedDate());
    }
}

void MainWindow::on_pushButton_getkit_resa_clicked()
{


    // Set property "is_in_resa_view"" of items in kitListResa_view to "false"
    for(const auto& kit_elem : this->kitListResa_view)
    {
        kit_elem->setIs_in_resa_view(false);
    }
    this->kitListResa_view.clear();

    /* if user enterd a code, then search for Kits with this code */
    if (ui->lineEdit_findkitbycode_resa->text()!="")
    {
        RESA_get_kits_by_code(&this->kitList,&this->kitListResa_view,ui->lineEdit_findkitbycode_resa->text() );
    }
    /* if user enterd a text, then search for Kits with this code */
    else if (ui->lineEdit_findkitbyname_resa->text()!="")
    {
        RESA_get_kits_by_name(&this->kitList,&this->kitListResa_view,ui->lineEdit_findkitbyname_resa->text() );
    }
    else
    {
        /* if user enterd nothing, get every available kits */
        for(const auto& kit_elem : this->kitList)
        {
            this->kitListResa_view.push_back(kit_elem);
        }
    }


    // Refresh kits in basket to see if none booked them
    g_connect_db.set_kit_booked_status(&this->kitListResa_view, this->ui->calendarWidget->selectedDate());
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
    brush_free.setColor(Qt::GlobalColor::green);
    brush_free.setStyle(Qt::SolidPattern);
    // Define brush to display kit booked in kit reservation list
    brush_booked.setColor(Qt::GlobalColor::gray);
    brush_booked.setStyle(Qt::SolidPattern);
    // Define brush to display kit in basket
    brush_basket.setColor(Qt::GlobalColor::yellow);
    brush_basket.setStyle(Qt::SolidPattern);
    this->ui->listWidget_resa->clear();
    for(const auto& kit_elem : this->kitListResa_view)
    {
        QListWidgetItem* p_item = new QListWidgetItem(kit_elem->toString(), this->ui->listWidget_resa);
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
            p_item->setBackground(brush_free);
        }
    }
}

void MainWindow::on_listWidget_resa_itemDoubleClicked(QListWidgetItem *item)
{
    int row = this->ui->listWidget_resa->row(item);
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
    g_connect_db.set_kit_booked_status(&this->kitListBasket_view, this->ui->calendarWidget->selectedDate());

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
    brush_free.setColor(Qt::GlobalColor::green);
    brush_free.setStyle(Qt::SolidPattern);
    this->ui->listWidget_panierResa->clear();
    for(const auto& kit_elem : this->kitListBasket_view)
    {
        QListWidgetItem* p_item = new QListWidgetItem(kit_elem->toString(), this->ui->listWidget_panierResa);
        if (kit_elem->getIs_booked())
        {
            p_item->setBackground(brush_booked);
            isBasketReadyToBook = false;
        }
        else
        {
            p_item->setBackground(brush_free);
        }
    }
}


void MainWindow::on_listWidget_panierResa_itemDoubleClicked(QListWidgetItem *item)
{
    int row = this->ui->listWidget_panierResa->row(item);
    Kit* p_kit = this->kitListBasket_view.at(row);
    p_kit->setIs_in_basket(false);

    this->ui->listWidget_panierResa->removeItemWidget(item);//Delete kit from basket view
    this->kitListBasket_view.erase(this->kitListBasket_view.begin()+row);//Delete kit from basket list

    RESA_refresh_kit_list_table();
    RESA_refresh_basket_kit_list_table();
}




void MainWindow::on_pushButton_reserver_clicked()
{
    int resa_nb = 0;
    uint user_id = 0;
    bool has_errors = false;
    QDate start_date;
    start_date = this->ui->calendarWidget->selectedDate();

    g_connect_db.start_resa();// Start of LOCK

    // Refresh kits in basket to see if none booked them
    g_connect_db.set_kit_booked_status(&this->kitListBasket_view, this->ui->calendarWidget->selectedDate());
    RESA_refresh_basket_kit_list_table();

    //Retrieve user id
    has_errors = g_connect_db.get_user_id_by_mail(this->ui->lineEdit_resa_email_user->text(), &user_id);
    if (has_errors == true)
    {
        this->GEN_raise_popup_warning("Impossible de réserver pour l'utilisateur: **" + this->ui->lineEdit_resa_email_user->text()+"**");
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
                    g_connect_db.add_resa_from_kit(kit_elem, user_id, start_date, resa_nb );
                    kit_elem->setIs_in_basket(false);
                    kit_elem->setIs_booked(true);
                }
                GEN_raise_popup_info("Votre réservation n° **"+QString::number(resa_nb)+"** est bien prise en compte.");
                this->ui->listWidget_panierResa->clear();
                this->kitListBasket_view.clear();
                RESA_refresh_kit_list_table();
            }
        }
        else
        {
            this->GEN_raise_popup_warning("Impossible de réserver car un Kit n'est pas disponible à la date souhaitée.");
        }
    }
    g_connect_db.end_resa();// End of LOCK

}


void MainWindow::on_pushButton_resa_showResa_clicked()
{
    uint user_id = 0;

    bool has_errors = false;

     g_utils.clearList(&this->resaList);
    //Retrieve user id
    has_errors = g_connect_db.get_user_id_by_mail(this->ui->lineEdit_resa_email_user->text(), &user_id);
    g_connect_db.select_resa_by_user(&this->resaList, user_id);
    RESA_refresh_current_resa_list_table();
}


void MainWindow::RESA_refresh_current_resa_list_table(void)
{
    int prev_id_resa = 0;

    this->ui->listWidget_resa_currentResa->clear();
    for(const auto& resa_elem : this->resaList)
    {
        if (resa_elem->getId_resa() == prev_id_resa)
        {
            //pass
        }
        else
        {
            QListWidgetItem* p_item = new QListWidgetItem(resa_elem->toString(), this->ui->listWidget_resa_currentResa);
            prev_id_resa = resa_elem->getId_resa();
        }
    }
}




void MainWindow::on_listWidget_resa_currentResa_itemClicked(QListWidgetItem *item)
{
    QRegularExpression re;
    QRegularExpressionMatch match;
    Kit * p_kit;
    int resa_nb = 0;
    // Find resa nb
    re.setPattern("(\\d*) -");

    //clear kit list
    this->ui->listWidget_resa_kitsOfResa->clear();

    match = re.match(item->text());
    if (match.hasMatch())
    {
        qInfo()<< "There is a match: " << match.captured(1);
        resa_nb = match.captured(1).toInt();

        //iter through resa list to find every reservation with resa_nb and get their associated kits
        for(const auto& resa_elem : this->resaList)
        {
            if (resa_elem->getId_resa() == resa_nb)
            {
                p_kit = GESKIT_find_kit_by_id(resa_elem->getId_kit());
                QListWidgetItem* p_item = new QListWidgetItem(p_kit->toString(), this->ui->listWidget_resa_kitsOfResa);
            }
        }
    }
}

void MainWindow::on_pushButton_suppr_resa_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Suppression de réservation.",
                                                               tr("Êtes-vous sûr de vouloir supprimer votre réservation ?"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        // event->ignore();
    } else {
        // event->accept();
    }
}

// ^^^^^^ MAIN WINDOW "Gestion Reservation" ^^^^^^
//---------------------------------------------------------







#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "./popups/popupaddusers.h"
#include "./popups/login_connect.h"
#include "./popups/delete_user.h"
#include "./popups/poppupaddkit.h"
#include "./popups/popupsortirresa.h"
#include <list>

#include "resa.h"
#include <QListWidgetItem>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clean_HMI(void);
    void update_connection_status(bool is_user_logged);

public slots:
    void log_stuffs(QtMsgType type, const QString &msg);

private slots:
    /*Add user popup slots */
    //-----------Actions -------------------
    void on_actionNouvel_Utilisateur_triggered();
    void on_actionSe_connecter_triggered();
    void on_actionSe_d_connecter_triggered();
    void on_actionAfficher_les_logs_triggered();

    //-----------Slots GESUSER-------------------
    void on_GESUSER_pushButton_getuser_clicked();
    void on_GESUSER_pushButton_add_user_clicked();
    void on_GESUSER_pushbutton_deleteUser_clicked();

    //-----------Slots GESKIT-------------------
    void on_GESKIT_pushButton_addkit_clicked();
    void on_GESKIT_pushButton_duplicate_kit_clicked();
    void on_GESKIT_pushButton_modify_kit_clicked();
    void on_GESKIT_pushButton_getkit_clicked();
    void on_GESKIT_tableWidget_kit_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_GESKIT_tableWidget_kit_cellDoubleClicked(int row, int column);

    //-----------Slots RESA-------------------
    void on_RESA_pushButton_getkit_resa_clicked();
    void on_RESA_listWidget_resa_itemDoubleClicked(QListWidgetItem *item);
    void on_RESA_listWidget_panierResa_itemDoubleClicked(QListWidgetItem *item);
    void on_RESA_calendarWidget_clicked(const QDate &date);
    void on_RESA_pushButton_resa_showResa_clicked();
    void on_RESA_listWidget_resa_currentResa_itemClicked(QListWidgetItem *item);
    void on_RESA_pushButton_suppr_resa_clicked();
    void on_RESA_pushButton_reserver_clicked();

    //-----------Slots SORTIES-------------------
    void on_SORTIE_pushButton_resa_showResa_clicked();
    void on_SORTIE_listWidget_resa_currentResa_itemClicked(QListWidgetItem *item);
    void on_SORTIE_pushButton_sortir_clicked(); // Function called when "Sortir" Button is called inside main window
    void on_SORTIE_listWidget_resa_kitsOfResa_itemClicked(QListWidgetItem *item);
    void on_SORTIE_popupSortirResa_pushSortir();//Callback called when "Sortir" button is pushed inside popup
    void on_SORTIE_popupSortirResa_pushAnnuler();

private:
    Ui::MainWindow *ui;
    std::vector<Utilisateur*> userList;
    std::vector<Utilisateur*> userListView;
    std::vector<Kit*> kitList; //List of kits as a mirror of Kits in DB
    std::vector<Kit*> kitListGeskit_view; //List of kits to be displayed in "GESKIT_tableWidget_kit"
    std::vector<Kit*> kitListResa_view; //List of kits to be displayed in "RESA_listWidget_resa"
    std::vector<Kit*> kitListBasket_view; //List of kits to be displayed in "RESA_listWidget_panierResa"
    std::vector<Kit*> kitListSortie_kitsOfResaView; //List of kits to be displayed in "SORTIE_listWidget_resa_kitsOfResa"
    std::vector<Kit*> kitListSortie_kitsOutView; //List of kits to be displayed in "SORTIE_listWidget_kitsOut"
    std::vector<Resa*> resaList;
    std::vector<Resa*> resaListSortie;

    Utilisateur new_user;
    Utilisateur login_user;
    Utilisateur sortie_user; //User that takes a kit out
    popupAddUsers *p_popupAddUser;
    Login_connect *p_loginConnect;
    PopupSortirResa *p_popupSortirResa;
    Delete_user *p_popupDeleteUser;
    PoppupAddKit *p_popupAddKit;
    bool isBasketReadyToBook; //Variable remembering if the basket contains an already booked kit
    QListWidgetItem * item_lastSelectedResa;//Variable remembering the last selected resa in sortie view

    void GEN_raise_popup_info(QString msg);
    void GEN_raise_popup_warning(QString msg);
    void closeEvent (QCloseEvent *event);
    void activateWidgets(bool);

    //-----------popups-------------------
    void on_popupLogin_destroyed();
    void on_popupLogin_ok();
    void on_popupDelete_ok();
    void on_popupDelete_destroyed();
    void on_popupaddUser_destroyed();
    void on_popupaddUser_ok();
    void on_popupAddKit_destroyed();
    void on_popupAddKit_ok();

    //-----------User-------------------
    void GESUSER_get_user_by_name(std::vector<Utilisateur *> *from_user, std::vector<Utilisateur *> *to_user, QString name);
    void GESUSER_get_user_by_utinfo(std::vector<Utilisateur *> *from_user, std::vector<Utilisateur *> *to_user, QString utinfo);
    void GESUSER_add_new_user();
    void GESUSER_clear_display();
    void GESUSER_enable_GESUSER_buttons(bool i_enable);
    Utilisateur* GESUSER_get_user_selected();
    void GESUSER_refresh_user_list_from_server(std::vector<Utilisateur*> *i_list);
    void GESUSER_refresh_user_table();
    void GESUSER_push_back_new_user_on_table(Utilisateur *user, int row);

    //-----------Kit-------------------
    void GESKIT_get_kits_by_name(std::vector<Kit *> *from_kits, std::vector<Kit *> *to_kits, QString code);
    void GESKIT_get_kits_by_code(std::vector<Kit *> *from_kits, std::vector<Kit *> *to_kits, QString code);
    void GESKIT_clear_display();
    void GESKIT_enable_geskit_buttons(bool i_enable);
    Kit* GESKIT_get_kit_selected();
    void GESKIT_refresh_kit_list_from_server(std::vector<Kit*> *i_list);
    void GESKIT_refresh_kit_table();
    void GESKIT_push_back_new_kit_on_table(Kit *kit, int row);
    void GESKIT_refresh_item_list_table(Kit *kit);
    void GESKIT_refresh_descritption(Kit *kit);
    void GESKIT_push_back_new_item_on_table(Item *item, int row);
    Kit* GESKIT_find_kit_by_id(uint id);

    //-----------Resa-------------------
    void RESA_refresh_kit_list_table();
    void RESA_refresh_basket_kit_list_table();
    void RESA_get_kits_by_code(std::vector<Kit *> *from_kits, std::vector<Kit *> *to_kits, QString code);
    void RESA_get_kits_by_name(std::vector<Kit *> *from_kits, std::vector<Kit *> *to_kits, QString code);
    QDate RESA_get_next_resa_day(QDate start_date);
    void RESA_refresh_current_resa_list_table();
    int RESA_find_resa_nb_selected(QListWidgetItem *item);


    //-----------Sorties-------------------
    Kit *SORTIE_get_kit_selected();
    void SORTIE_refresh_current_resa_list_table();
    void SORTIE_sortir_kit();
    void SORTIE_refresh_kits_of_resa_table(int i_resa_nb);
};
#endif // MAINWINDOW_H

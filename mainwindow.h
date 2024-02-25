#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "./popups/popupaddusers.h"
#include "./popups/login_connect.h"
#include "./popups/delete_user.h"
#include "./popups/poppupaddkit.h"
#include <list>
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

public slots:
    void update_connection_status(bool is_user_logged);
    void log_stuffs(QtMsgType type, const QString &msg);

private slots:
    /*Add user popup slots */
    void on_popupaddUser_destroyed();
    void on_popupaddUser_ok();
    void on_getUsers_clicked();
    void on_actionNouvel_Utilisateur_triggered();
    void on_actionSe_connecter_triggered();
    void on_deleteUser_clicked();
    void on_actionSe_d_connecter_triggered();

    void on_actionAfficher_les_logs_triggered();

    void on_pushButton_addkit_clicked();
    void on_popupAddKit_destroyed();
    void on_popupAddKit_ok();

    void on_pushButton_getkit_clicked();

private:
    Ui::MainWindow *ui;
    std::list<Utilisateur*> userList;
    std::list<Kit*> kitList;
    Utilisateur new_user;
    Utilisateur login_user;
    popupAddUsers *p_popupAddUser;
    Login_connect *p_loginConnect;
    Delete_user *p_popupDeleteUser;
    PoppupAddKit *p_popupAddKit;
    void closeEvent (QCloseEvent *event);
    void add_user_to_DB(void);
    void clearUserList(void);
    void refresh_user_list_table(void);

    void activateWidgets(bool);

    void on_popupLogin_destroyed();
    void on_popupLogin_ok();
    void on_popupDelete_ok();
    void on_popupDelete_destroyed();

    void push_back_new_kit_on_table(Kit *kit, int row);
    void refresh_kit_list_table();
    void clearKitList();
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "./popups/popupaddusers.h"
#include "./popups/login_connect.h"
#include "./popups/delete_user.h"
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

private:
    Ui::MainWindow *ui;
    std::list<Utilisateur*> userList;
    Utilisateur new_user;
    Utilisateur login_user;
    popupAddUsers *p_popupAddUser;
    Login_connect *p_loginConnect;
    Delete_user *p_popupDeleteUser;
    void add_user_to_DB(void);
    void clearUserList(void);
    void refreshScrollArea(void);

    void activateWidgets(bool);

    void on_popupLogin_destroyed();
    void on_popupLogin_ok();
    void on_popupDelete_ok();
    void on_popupDelete_destroyed();

};
#endif // MAINWINDOW_H

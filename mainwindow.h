#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "./popups/popupaddusers.h"
#include "./popups/login_connect.h"
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

private slots:
    void test_slot_called();

    /*Add user popup slots */
    void on_addUser_clicked();
    void on_popupaddUser_destroyed();
    void on_popupaddUser_ok();
    void on_getUsers_clicked();

    void on_actionNouvel_Utilisateur_triggered();

    void on_actionSe_connecter_triggered();

private:
    Ui::MainWindow *ui;
    std::list<Utilisateur*> userList;
    Utilisateur new_user;
    Utilisateur login_user;
    popupAddUsers *p_popupAddUser;
    Login_connect *p_loginConnect;
    void add_user_to_DB(void);
    void clearUserList(void);
    void refreshScrollArea(void);

    void on_popupLogin_destroyed();
    void on_popupLogin_ok();
};
#endif // MAINWINDOW_H
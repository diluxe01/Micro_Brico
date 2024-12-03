#ifndef LOGIN_CONNECT_H
#define LOGIN_CONNECT_H

#include <QDialog>
#include <QDialogButtonBox>
#include "../utilisateur.h"

namespace Ui {
class login_connect;
}

class Login_connect : public QDialog
{
    Q_OBJECT

public:
    explicit Login_connect(QWidget *parent = nullptr);
    ~Login_connect();
    void setUser(Utilisateur * p_user);

    QPushButton *getAnnulPushButton();
    QPushButton *getOkPushButton();

private slots:

    void on_pushButton_ok_clicked();

private:
    Ui::login_connect *ui;
    Utilisateur *user;
};

#endif // LOGIN_CONNECT_H

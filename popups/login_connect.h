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
    QDialogButtonBox* getOkButton(void);
    void setUser(Utilisateur * p_user);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);


private:
    Ui::login_connect *ui;
    Utilisateur *user;
};

#endif // LOGIN_CONNECT_H

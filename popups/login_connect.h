#ifndef LOGIN_CONNECT_H
#define LOGIN_CONNECT_H

#include <QDialog>

namespace Ui {
class login_connect;
}

class Login_connect : public QDialog
{
    Q_OBJECT

public:
    explicit Login_connect(QWidget *parent = nullptr);
    ~Login_connect();

private:
    Ui::login_connect *ui;
};

#endif // LOGIN_CONNECT_H

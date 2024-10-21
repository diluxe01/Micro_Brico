#ifndef POPUPADDUSERS_H
#define POPUPADDUSERS_H

#include <QDialog>
#include <qabstractbutton.h>
#include <QDialogButtonBox>
#include "../utilisateur.h"

namespace Ui {
class popupAddUsers;
}

class popupAddUsers : public QDialog
{
    Q_OBJECT


public:
    explicit popupAddUsers(QWidget *parent = nullptr);
    ~popupAddUsers();
    void setCaller_privilege(T_user_privilege newCaller_privilege);
    void show_wrapper(void);

    void closeEvent(QCloseEvent *event);
    Utilisateur* get_user_from_form(void);
    void set_form_from_user(Utilisateur * p_user);

    QPushButton *getOkButton();
    QPushButton *getCancelButton();
private:
    Ui::popupAddUsers *ui;
    Utilisateur *user;
    T_user_privilege caller_privilege;
    void GEN_raise_popup_warning(QString msg);
signals:
    void delete_popup(void);

};

#endif // POPUPADDUSERS_H

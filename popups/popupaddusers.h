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

protected:
    explicit popupAddUsers(QWidget *parent = nullptr);
    static popupAddUsers* popuAddUser_;// memorize the single instance pointer of this singleton
    ~popupAddUsers();

public:
    /****** Tambouille singleton *******/
    static popupAddUsers *GetInstance();
    void deleteInstance();
    /****** FIN tambouille singleton *******/

    void setUser(Utilisateur * p_user);
    QDialogButtonBox* getOkButton(void);


private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::popupAddUsers *ui;
    Utilisateur *user;

};

#endif // POPUPADDUSERS_H

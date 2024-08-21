#include "popupaddusers.h"
#include "ui_popupaddusers.h"
#include "../connect_db.h"
#include "../datamodel.h"

popupAddUsers::popupAddUsers(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::popupAddUsers)
{
    ui->setupUi(this);
    ui->lineEdit_utinfo->setFocus();
}

popupAddUsers::~popupAddUsers()
{
    delete ui;
    popuAddUser_ = nullptr;
}

/**
 * Static methods should be defined outside the class.
 */

popupAddUsers* popupAddUsers::popuAddUser_= nullptr;

popupAddUsers *popupAddUsers::GetInstance()
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if(popuAddUser_ == nullptr){
        popuAddUser_ = new popupAddUsers();
    }
    return popuAddUser_;
}

void popupAddUsers::deleteInstance(void)
{
    delete (popuAddUser_);
}

void popupAddUsers::on_buttonBox_clicked(QAbstractButton *button)
{
    this->user->setUserData(ui->lineEdit_Nom->text(),
                            ui->lineEdit_mdp->text(),
                            ui->lineEdit_Prenom->text(),
                            ui->lineEditMail->text(),
                            ui->lineEdit_utinfo->text()
                            );
    //Convert mdp to sha1
    this->user->setMdp(g_connect_db.get_sha1_from_Qstring(ui->lineEdit_mdp->text()));
}

void popupAddUsers::setCaller_privilege(T_user_privilege newCaller_privilege)
{
    caller_privilege = newCaller_privilege;
}

void popupAddUsers::show_wrapper()
{
    if (this->caller_privilege == E_admin)
    {
        this->ui->comboBox_privilege->setEnabled(true);
    }
    else
    {
        this->ui->comboBox_privilege->setEnabled(false);
    }
    this->show();
}

void popupAddUsers::setUser(Utilisateur * p_user)
{
    this->user = p_user;
}

QDialogButtonBox* popupAddUsers::getOkButton(void)
{
    return ui->buttonBox;
}

#include "popupaddusers.h"
#include "ui_popupaddusers.h"

popupAddUsers::popupAddUsers(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::popupAddUsers)
{
    ui->setupUi(this);
}

popupAddUsers::~popupAddUsers()
{
    delete ui;
    popuAddUser_ = nullptr;
}

/**
 * Static methods should be defined outside the class.
 */

popupAddUsers* popupAddUsers::popuAddUser_= nullptr;;

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
}

void popupAddUsers::setUser(Utilisateur * p_user)
{
    this->user = p_user;
}

QDialogButtonBox* popupAddUsers::getOkButton(void)
{
    return ui->buttonBox;
}

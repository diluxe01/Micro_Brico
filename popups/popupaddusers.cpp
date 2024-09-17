#include "popupaddusers.h"
#include "ui_popupaddusers.h"
#include "../connect_db.h"
#include "../datamodel.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QListWidgetItem>
#include <QMessageBox>

popupAddUsers::popupAddUsers(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::popupAddUsers)
{
    ui->setupUi(this);
    ui->lineEdit_utinfo->setFocus();
    user = new Utilisateur;
}

void popupAddUsers::closeEvent( QCloseEvent* event )
{
    emit delete_popup();
    // event->accept();
}
popupAddUsers::~popupAddUsers()
{
    delete ui;
}

/**
 * Static methods should be defined outside the class.
 */

void popupAddUsers::GEN_raise_popup_warning(QString msg)
{

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(msg);
    msgBox.setTextFormat(Qt::TextFormat::MarkdownText);
    msgBox.setWindowTitle("Attention");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    qDebug() << msg;
}


Utilisateur* popupAddUsers::get_user_from_form(void)
{
    T_user_privilege privilege = E_basic;

    QStringList error_string_list;
    bool ok_to_create_user = true;

    if (ui->comboBox_privilege->currentText() == "Administrateur")
    {
        privilege = E_admin;
    }
    else
    {
        privilege = E_basic;
    }
    if (ui->lineEdit_Nom->text() == "")
    {
        error_string_list.append("Erreur dans le format du nom: Veuillez rentrer votre nom.");
        ok_to_create_user = false;
    }
    if (ui->lineEdit_Prenom->text() == "")
    {
        error_string_list.append("Erreur dans le format du prénom: Veuillez rentrer votre prénom.");
        ok_to_create_user = false;
    }
    if (ui->lineEditMail->text().contains("@") == false )
    {
        error_string_list.append("Votra adresse mail est invalide.");
        ok_to_create_user = false;
    }

    if (ui->lineEdit_utinfo->text().size() != 6 )
    {
        error_string_list.append("Votre Utinfo ne fait pas la bonne taille.");
        ok_to_create_user = false;
    }
    if (ui->lineEdit_utinfo->text().contains("\w+") == true )
    {
        error_string_list.append("Votre Utinfo ne doit pas contenir de lettre.");
        ok_to_create_user = false;
    }
    if (ui->lineEdit_utinfo->text().contains("\S+") == true )
    {
        error_string_list.append("Votre Utinfo ne doit pas contenir d'espace.");
        ok_to_create_user = false;
    }
    if (ok_to_create_user)
    {
        this->user->setUserData(ui->lineEdit_Nom->text(),
                                ui->lineEdit_mdp->text(),
                                ui->lineEdit_Prenom->text(),
                                ui->lineEditMail->text(),
                                ui->lineEdit_utinfo->text(),
                                privilege
                                );
        //Convert mdp to sha1
        this->user->setMdp(g_connect_db.get_sha1_from_Qstring(ui->lineEdit_mdp->text()));
        return this->user;
    }
    else
    {
        QString pop_msg_errors;
        for(const auto& str_elem : error_string_list)
        {
            pop_msg_errors += "\n\r" + str_elem;
        }
        GEN_raise_popup_warning(pop_msg_errors);
        return NULL;
    }
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




QPushButton* popupAddUsers::getOkButton(void)
{
    return ui->pushButton_OK;
}

QPushButton* popupAddUsers::getCancelButton(void)
{
    return ui->pushButton_annuler;
}


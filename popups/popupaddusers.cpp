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
    user->setCreate_type(E_new);// By default a user is created
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

    qsizetype utinfo_char_size = ui->lineEdit_utinfo->text().size();
    if (utinfo_char_size != 8 && utinfo_char_size != 8 &&  utinfo_char_size != 10)
    {
        error_string_list.append("Votre Utinfo ne fait pas la bonne taille. Il devrait contenir 7, 8 ou 10 lettres.");
        ok_to_create_user = false;
    }

    if (ui->lineEdit_utinfo->text().contains("\S+") == true )
    {
        error_string_list.append("Votre Utinfo ne doit pas contenir d'espace.");
        ok_to_create_user = false;
    }

    if (ui->lineEdit_mdp->text() != ui->lineEdit_confirme_mdp_2->text() )
    {
        error_string_list.append("Attention, les deux mots de passe ne sont pas identiques.");
        ok_to_create_user = false;
    }

    // If in edit mode, do not change password, unless the linedit has been changed
    if (this->user->getCreate_type() == E_modify)
    {
        if (this->user->getMdp()!= ui->lineEdit_mdp->text())
        {
            this->user->setMdp(g_connect_db.get_sha1_from_Qstring(ui->lineEdit_mdp->text()));
        }
    }
    else //new user mode
    {
        this->user->setMdp(g_connect_db.get_sha1_from_Qstring(ui->lineEdit_mdp->text()));
    }

    if (ok_to_create_user)
    {
        this->user->setUserData(ui->lineEdit_Nom->text(),
                                ui->lineEdit_Prenom->text(),
                                ui->lineEditMail->text(),
                                ui->lineEdit_utinfo->text(),
                                privilege
                                );

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

void popupAddUsers::set_form_from_user(Utilisateur * p_user)
{
    this->ui->lineEditMail->setText(p_user->getEmail());
    this->ui->lineEdit_utinfo->setText(p_user->getUtinfo());
    this->ui->lineEdit_Nom->setText(p_user->getNom());
    this->ui->lineEdit_Prenom->setText(p_user->getPrenom());
    this->ui->lineEdit_mdp->setText(p_user->getMdp());//Put same password in both line_edit
    this->ui->lineEdit_confirme_mdp_2->setText(p_user->getMdp());
    if (p_user->getPrivilege() == E_admin)
    {
        this->ui->comboBox_privilege->setCurrentIndex(1);
    }
    else
    {
        this->ui->comboBox_privilege->setCurrentIndex(0);
    }
    this->user->setId(p_user->getId());
    this->user->setCreate_type(p_user->getCreate_type());
    this->user->setMdp(p_user->getMdp());
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


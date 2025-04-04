#include "utilisateur.h"
#include <qstring.h>


Utilisateur::Utilisateur(void ) {
    this->nom = "nom";
    this->mdp = "mdp";
    this->prenom = "prenom";
    this->email = "email";
    this->token = "token";
    this->utinfo = "utinfo";
    this->is_logged_on = false;
    this->create_type = E_new;
}

Utilisateur::Utilisateur(QString nom, QString mdp, QString prenom, QString email, QString utinfo, QString telephone, QDate date_caution, bool adhesion_payed, Money caution)
{
    this->nom = nom;
    this->mdp = mdp;
    this->prenom = prenom;
    this->email = email;
    this->utinfo = utinfo;
    this->is_logged_on = false;
    this->create_type = E_new;
    this->telephone = telephone;
    this->date_caution = date_caution;
    this->adhesion_payed = adhesion_payed;
    this->caution = caution;

}


void Utilisateur::setUserData(QString nom, QString prenom, QString email, QString utinfo, T_user_privilege newPrivilege, QString telephone, QDate date_caution, bool adhesion_payed, Money caution)
{
    this->nom = nom;
    this->prenom = prenom;
    this->email = email;
    this->utinfo = utinfo;
    this->privilege = newPrivilege;
    this->telephone = telephone;
    this->date_caution = date_caution;
    this->adhesion_payed = adhesion_payed;
    this->caution = caution;

}


QString Utilisateur::getNom(void)
{
    return this->nom;
}

QString Utilisateur::getMdp(void)
{
    return this->mdp;
}

QString Utilisateur::getPrenom() const
{
    return prenom;
}

QString Utilisateur::getEmail() const
{
    return email;
}

QString Utilisateur::getToken() const
{
    return token;
}

QString Utilisateur::getUtinfo() const
{
    return utinfo;
}

void Utilisateur::setId(int newId)
{
    id = newId;
}

void Utilisateur::setMdp(const QString &newMdp)
{
    mdp = newMdp;
}

void Utilisateur::setEmail(const QString &newEmail)
{
    email = newEmail;
}

void Utilisateur::setUtinfo(const QString &newUtinfo)
{
    utinfo = newUtinfo;
}

T_user_privilege Utilisateur::getPrivilege() const
{
    return privilege;
}

void Utilisateur::setPrivilege(const T_user_privilege &newPrivilege)
{
    privilege = newPrivilege;
}

bool Utilisateur::getIs_logged_on() const
{
    return is_logged_on;
}

void Utilisateur::setIs_logged_on(bool newIs_logged_on)
{
    is_logged_on = newIs_logged_on;
}

void Utilisateur::setPrenom(const QString &newPrenom)
{
    prenom = newPrenom;
}

void Utilisateur::setNom(const QString &newNom)
{
    nom = newNom;
}

void Utilisateur::setToken(const QString &newToken)
{
    token = newToken;
}

int Utilisateur::getId() const
{
    return id;
}

T_modify_or_new Utilisateur::getCreate_type() const
{
    return create_type;
}

void Utilisateur::setCreate_type(T_modify_or_new newCreate_type)
{
    create_type = newCreate_type;
}

QString Utilisateur::getTelephone() const
{
    return telephone;
}

void Utilisateur::setTelephone(const QString &newTelephone)
{
    telephone = newTelephone;
}

QDate Utilisateur::getDate_caution() const
{
    return date_caution;
}

void Utilisateur::setDate_caution(const QDate &newDate_caution)
{
    date_caution = newDate_caution;
}

bool Utilisateur::getAdhesion_payed() const
{
    return adhesion_payed;
}

void Utilisateur::setAdhesion_payed(bool newAdhesion_payed)
{
    adhesion_payed = newAdhesion_payed;
}

Money Utilisateur::getCaution() const
{
    return caution;
}

void Utilisateur::setCaution(const Money &newCaution)
{
    caution = newCaution;
}


QString Utilisateur::ToString() {

    QString ret = " nom: " + this->nom +
                  " mdp: " + this->mdp +
                  " id: " + QString::number(this->id) +
                  " prenom: " + this->prenom +
                  " email: " + this->email +
                  " utinfo: " + this->utinfo +
                  " token: " + this->token
                    ;
    return ret;
}


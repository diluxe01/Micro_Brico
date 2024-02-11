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
}
Utilisateur::Utilisateur( QString nom, QString mdp, QString prenom, QString email, QString utinfo)
{
    this->nom = nom;
    this->mdp = mdp;
    this->prenom = prenom;
    this->email = email;
    this->utinfo = utinfo;
    this->is_logged_on = false;
}

void Utilisateur::setUserData( QString nom, QString mdp, QString prenom, QString email, QString utinfo)
{
    this->nom = nom;
    this->mdp = mdp;
    this->prenom = prenom;
    this->email = email;
    this->utinfo = utinfo;
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

QString Utilisateur::ToString() {

    QString ret = " nom: " + this->nom +
                  " mdp: " + this->mdp +
                  " id: " + QString::number(this->id) +
                  " prenom: " + this->prenom +
                  " email: " + this->email +
                  " utinfo: " + this->utinfo
                    ;
    return ret;
}

#include "utilisateur.h"
#include <qstring.h>


Utilisateur::Utilisateur(void ) {
    this->nom = "nom";
    this->mdp = "mdp";
    this->prenom = "prenom";
    this->email = "email";
    this->token = "token";
    this->utinfo = "utinfo";
}
Utilisateur::Utilisateur( QString nom, QString mdp, QString prenom, QString email, QString utinfo)
{
    this->nom = nom;
    this->mdp = mdp;
    this->prenom = prenom;
    this->email = email;
    this->utinfo = utinfo;
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

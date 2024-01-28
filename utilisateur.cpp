#include "utilisateur.h"
#include <qstring.h>

Utilisateur::Utilisateur(QString name, int id, QString mdp ) {
    this->name = name;
    this->mdp = mdp;
    this->id = id;
}

Utilisateur::Utilisateur(void ) {
    this->name = "foo";
    this->mdp = "mdp";
    this->id = 0;
}
void Utilisateur::setUserData(QString name,int id,QString mdp)
{
    this->name = name;
    this->id = id;
    this->mdp = mdp;
}

QString Utilisateur::getName(void)
{
    return this->name;
}

QString Utilisateur::getMdp(void)
{
    return this->mdp;
}

QString Utilisateur::ToString() {

    QString ret = "name: " + this->name + " mdp:  " + this->mdp + " id: " + QString::number(this->id);
    return ret;
}

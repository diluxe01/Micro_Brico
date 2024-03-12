#include "kit.h"

QString Kit::getNom() const
{
    return nom;
}

QString Kit::getDescription() const
{
    return description;
}

QDate Kit::getDate_achat() const
{
    return date_achat;
}

Money Kit::getPrix_achat() const
{
    return prix_achat;
}

QString Kit::getTexte_libre() const
{
    return texte_libre;
}

bool Kit::getEn_panne() const
{
    return en_panne;
}

QString Kit::getEn_panne_str() const
{
    if (en_panne == true)
    {
        return "Oui";
    }
    else
    {
        return "Non";
    }
}
QString Kit::getCode() const
{
    return code;
}

Money Kit::getCaution() const
{
    return caution;
}

uint Kit::getIdKit() const
{
    return idkit;
}

void Kit::setIdkit(uint newIdkit)
{
    idkit = newIdkit;
}

void Kit::setNom(const QString &newNom)
{
    nom = newNom;
}

void Kit::setDescription(const QString &newDescription)
{
    description = newDescription;
}

void Kit::setDate_achat(const QDate &newDate_achat)
{
    date_achat = newDate_achat;
}

void Kit::setDate_achat(const QString &newDate_achat)
{
    QStringList list1 = newDate_achat.split("-");
    date_achat.setDate(list1.at(0).toInt(), list1.at(1).toInt(),list1.at(2).toInt()) ;
}

void Kit::setPrix_achat(const Money &newPrix_achat)
{
    prix_achat = newPrix_achat;
}

void Kit::setPrix_achat(const QString &newPrix_achat)
{
    prix_achat.setValue(newPrix_achat);
}

void Kit::setTexte_libre(const QString &newTexte_libre)
{
    texte_libre = newTexte_libre;
}

void Kit::setCaution(const Money &newCaution)
{
    caution = newCaution;
}

void Kit::setCaution(const QString &newCaution)
{
    caution.setValue(newCaution);
}

void Kit::setEn_panne(bool newEn_panne)
{
    en_panne = newEn_panne;
}



void Kit::setCode(const QString &newCode)
{
    code = newCode;
}

Kit::Kit(QObject *parent)
    : QObject{parent}
{}

Kit::Kit(uint idkit, const QString &nom, const QString &description, const QDate &date_achat, const Money &prix_achat, const QString &texte_libre, bool en_panne, const QString &code, const Money &caution) :
    idkit(std::move(idkit)),
    nom(nom),
    description(description),
    date_achat(date_achat),
    prix_achat(prix_achat),
    texte_libre(texte_libre),
    en_panne(en_panne),
    code(code),
    caution(caution)
{}

Kit::Kit(uint idkit, const QString &nom, const QString &description, const QString &date_achat, const QString &prix_achat, const QString &texte_libre, const bool en_panne, const QString &code, const QString &caution)
{
    this->setIdkit(idkit);
    this->setNom(nom);
    this->setDescription(description);
    this->setDate_achat(date_achat);
    this->setPrix_achat(prix_achat);
    this->setTexte_libre(texte_libre);
    this->setEn_panne(en_panne);
    this->setCode(code);
    this->setCaution(caution);
}

#include "item.h"

void Item::setid(uint newid)
{
    id = newid;
}

void Item::setName(const QString &newName)
{
    name = newName;
}

void Item::setForkey(uint newForkey)
{
    forkey = newForkey;
}

Item::Item(QObject *parent)
    : QObject{parent}
{}

QString Item::getName() const
{
    return name;
}

uint Item::getForkey() const
{
    return forkey;
}

T_etat_item Item::getEtat() const
{
    return etat;
}

QString Item::getEtatStr() const
{
    QString etat_str;
    switch(etat)
    {
    case E_NEW:
        etat_str = "NEUF";
        break;
    case E_WORN:
        etat_str = "USE";
        break;
    case E_BROKEN:
        etat_str = "CASSE";
        break;
    case E_LOST:
        etat_str = "PERDU";
        break;
    default:
        etat_str = "NEUF";
        break;
    }
    return etat_str;
}

Item::Item(uint id, const QString &name, uint forkey, T_etat_item etat) : id(std::move(id)),
    name(name),
    forkey(std::move(forkey)),
    etat(etat)
{}

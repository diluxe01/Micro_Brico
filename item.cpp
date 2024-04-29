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

// T_etat_item Item::getEtat() const
// {
//     return etat;
// }

// QString Item::getEtatStr() const
// {
//     QString etat_str;
//     switch(this->etat)
//     {
//     case E_NEW:
//         etat_str = "NEUF";
//         break;
//     case E_WORN:
//         etat_str = "USE";
//         break;
//     case E_BROKEN:
//         etat_str = "CASSE";
//         break;
//     case E_LOST:
//         etat_str = "PERDU";
//         break;
//     default:
//         etat_str = "NEUF";
//         break;
//     }
//     return etat_str;
// }

// void Item::setEtatStr(QString etat_str)
// {
//     if (etat_str == "NEUF")
//     {
//         this->etat = E_NEW;
//     }
//     else if (etat_str == "USE")
//     {
//         this->etat = E_WORN;
//     }
//     else if (etat_str == "CASSE")
//     {
//         this->etat = E_BROKEN;
//     }
//     else if (etat_str == "PERDU")
//     {
//         this->etat = E_LOST;
//     }
//     else
//     {
//         this->etat = E_NEW;
//     }
// }

Item::Item(uint id, const QString &name, uint forkey) : id(std::move(id)),
    name(name),
    forkey(std::move(forkey))
{}

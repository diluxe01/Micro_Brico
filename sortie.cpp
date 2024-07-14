#include "sortie.h"

int Sortie::getId() const
{
    return id;
}

QDate Sortie::getStart_date() const
{
    return start_date;
}

int Sortie::getId_kit() const
{
    return id_kit;
}

void Sortie::setId(int newId)
{
    id = newId;
}

void Sortie::setStart_date(const QDate &newStart_date)
{
    start_date = newStart_date;
}

void Sortie::setId_kit(int newId_kit)
{
    id_kit = newId_kit;
}

void Sortie::setId_user(int newId_user)
{
    id_user = newId_user;
}

int Sortie::getId_user() const
{
    return id_user;
}

void Sortie::setId_sortie(int newId_sortie)
{
    id_sortie = newId_sortie;
}

int Sortie::getId_sortie() const
{
    return id_sortie;
}

Sortie::Sortie(QObject *parent)
    : QObject{parent}
{

}

Sortie::Sortie(int id, const QDate &start_date, int id_kit, int id_user, int id_sortie) : id(id),
    start_date(start_date),
    id_kit(id_kit),
    id_user(id_user),
    id_sortie(id_sortie)
{}

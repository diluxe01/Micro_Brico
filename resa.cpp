#include "resa.h"

int Resa::getId() const
{
    return id;
}

void Resa::setId(int newId)
{
    id = newId;
}

QDate Resa::getStart_date() const
{
    return start_date;
}

void Resa::setStart_date(const QDate &newStart_date)
{
    start_date = newStart_date;
}

int Resa::getId_kit() const
{
    return id_kit;
}

void Resa::setId_kit(int newId_kit)
{
    id_kit = newId_kit;
}

int Resa::getId_user() const
{
    return id_user;
}

void Resa::setId_user(int newId_user)
{
    id_user = newId_user;
}

Resa::Resa() {}

Resa::Resa(int id, const QDate &start_date, int id_kit, int id_user) : id(id),
    start_date(start_date),
    id_kit(id_kit),
    id_user(id_user)
{}

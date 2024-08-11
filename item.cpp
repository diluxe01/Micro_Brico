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

uint Item::getId() const
{
    return id;
}

uint Item::getQuantity() const
{
    return quantity;
}

void Item::setQuantity(uint newQuantity)
{
    quantity = newQuantity;
}

bool Item::getIs_verified() const
{
    return is_verified;
}

void Item::setIs_verified(bool newIs_verified)
{
    is_verified = newIs_verified;
}

Item::Item(QObject *parent)
    : QObject{parent}
{
    is_verified = false;
}

QString Item::getName() const
{
    return name;
}

uint Item::getForkey() const
{
    return forkey;
}


Item::Item(uint id, const QString &name, uint forkey, uint quantity) : id(std::move(id)),
    name(name),
    forkey(std::move(forkey)),
    quantity(std::move(quantity))
{
    is_verified = false;
}

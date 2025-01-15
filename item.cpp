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


bool Item::getIs_verified() const
{
    return is_verified;
}

void Item::setIs_verified(bool newIs_verified)
{
    is_verified = newIs_verified;
}

uint Item::getQuantity_init() const
{
    return quantity_init;
}

void Item::setQuantity_init(uint newQuantity_init)
{
    quantity_init = newQuantity_init;
}

uint Item::getQuantity_current() const
{
    return quantity_current;
}

void Item::setQuantity_current(uint newQuantity_current)
{
    quantity_current = newQuantity_current;
}

uint Item::getQuantity_out() const
{
    return quantity_out;
}

void Item::setQuantity_out(uint newQuantity_out)
{
    quantity_out = newQuantity_out;
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


Item::Item(uint id, const QString &name, uint forkey, uint quantity_init, uint quantity_current, uint quantity_out) : id(std::move(id)),
    name(name),
    forkey(std::move(forkey)),
    quantity_init(std::move(quantity_init)),
    quantity_current(std::move(quantity_current)),
    quantity_out(std::move(quantity_out))
{
    is_verified = false;
}

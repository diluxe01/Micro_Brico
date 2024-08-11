#include "utils.h"
Utils g_utils;

Utils::Utils() {}

void Utils::clearList(std::vector<Item*> *i_list)
{
    for(const auto& elem : *i_list)
    {
        delete (elem);
    }
    i_list->clear();
}

void Utils::clearList(std::vector<Kit*> *i_list)
{
    for(const auto& elem : *i_list)
    {
        delete (elem);
    }
    i_list->clear();
}

void Utils::clearList(std::vector<Utilisateur*> *i_list)
{
    for(const auto& elem : *i_list)
    {
        delete (elem);
    }
    i_list->clear();
}


void Utils::clearList(std::vector<Resa*> *i_list)
{
    for(const auto& elem : *i_list)
    {
        delete (elem);
    }
    i_list->clear();
}

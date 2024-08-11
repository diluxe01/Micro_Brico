#ifndef UTILS_H
#define UTILS_H
#include "item.h"
#include "kit.h"
#include "utilisateur.h"
#include "resa.h"
#include <vector>

class Utils
{
public:
    Utils();
    void clearList(std::vector<Item*> *);
    void clearList(std::vector<Kit*> *);
    void clearList(std::vector<Utilisateur*> *);
    void clearList(std::vector<Resa*> *);
};

extern  Utils g_utils;

#endif // UTILS_H

#ifndef UTILS_H
#define UTILS_H
#include "item.h"
#include "kit.h"
#include "utilisateur.h"
#include "resa.h"
#include "sortie.h"
#include "log.h"
#include <vector>
using namespace std;



class Utils
{
public:
    Utils();
    void clearList(std::vector<Item*> *);
    void clearList(std::vector<Kit*> *);
    void clearList(std::vector<Utilisateur*> *);
    void clearList(std::vector<Resa*> *);
    void clearList(std::vector<Sortie*> *);
    void clearList(std::vector<Log*> *);
};

extern  Utils g_utils;

#endif // UTILS_H

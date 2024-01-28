#ifndef UTILISATEUR_H
#define UTILISATEUR_H
#include <qstring.h>
using namespace std;

class Utilisateur
{
public:
    Utilisateur();
    Utilisateur(QString name, int id, QString mdp );
    QString ToString();
    void setUserData(    QString name,int id,QString mdp);
    QString getName(void);
    QString getMdp(void);
private:
    QString name;
    int id;
    QString mdp;
};

#endif // UTILISATEUR_H

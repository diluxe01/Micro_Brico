#ifndef UTILISATEUR_H
#define UTILISATEUR_H
#include <qstring.h>
using namespace std;

class Utilisateur
{
public:
    Utilisateur();
    Utilisateur(QString nom, QString mdp, QString prenom, QString email, QString utinfo);
    QString ToString();
    void setUserData( QString nom, QString mdp, QString prenom, QString email, QString utinfo);
    QString getNom(void);
    QString getMdp(void);
    QString getPrenom() const;
    QString getEmail() const;
    QString getToken() const;
    QString getUtinfo() const;

    void setId(int newId);

    void setMdp(const QString &newMdp);
    void setEmail(const QString &newEmail);

    void setUtinfo(const QString &newUtinfo);

private:
    int id;
    QString nom;
    QString mdp;
    QString prenom;
    QString email;
    QString token;
    QString utinfo;
};

#endif // UTILISATEUR_H

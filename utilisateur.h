#ifndef UTILISATEUR_H
#define UTILISATEUR_H
#include <qstring.h>
using namespace std;

enum T_user_privilege
{
    E_admin,
    E_basic
};

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

    T_user_privilege getPrivilege() const;

    void setPrivilege(const T_user_privilege &newPrivilege);

    bool getIs_logged_on() const;
    void setIs_logged_on(bool newIs_logged_on);

    void setPrenom(const QString &newPrenom);

    void setNom(const QString &newNom);

    void setToken(const QString &newToken);

    int getId() const;

private:
    int id;
    QString nom;
    QString mdp;
    QString prenom;
    QString email;
    QString token;
    QString utinfo;
    T_user_privilege privilege;
    bool is_logged_on;
};

#endif // UTILISATEUR_H

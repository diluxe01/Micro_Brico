#ifndef CONNECT_DB_H
#define CONNECT_DB_H

#include <Qsql>
#include <QSqlDatabase>
#include "utilisateur.h"

#include <list>

class Connect_db
{
private:
    QSqlQuery runQuery(QString);
public:

    QSqlDatabase db;
    Connect_db();
    void select_all_users(std::list<Utilisateur*> *list);
    void add_user (Utilisateur *user);
    bool is_user_identified(Utilisateur *login_user);
    QString get_sha1_from_Qstring(QString mdp);
};

#endif // CONNECT_DB_H

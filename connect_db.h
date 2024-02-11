#ifndef CONNECT_DB_H
#define CONNECT_DB_H

#include <Qsql>
#include <QSqlDatabase>
#include "utilisateur.h"

#include <list>

class Connect_db
{
private:
    void update_user_infos_from_db(Utilisateur *login_user);
public:

    QSqlDatabase db;
    Connect_db();
    void select_all_users(std::list<Utilisateur*> *list);
    bool add_user (Utilisateur *user);
    bool delete_user(Utilisateur *user_to_delete);
    bool is_user_identified(Utilisateur *login_user);
    QString get_sha1_from_Qstring(QString mdp);
};

#endif // CONNECT_DB_H

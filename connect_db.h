#ifndef CONNECT_DB_H
#define CONNECT_DB_H

#include <Qsql>
#include <QSqlDatabase>
#include <QObject>
#include "utilisateur.h"

#include <list>

class Connect_db : public QObject
{
    Q_OBJECT

private:
    void update_user_infos_from_db(Utilisateur *login_user);
    bool is_user_connected(Utilisateur *login_user);
public:

    QSqlDatabase db;
    Connect_db();
    void select_all_users(Utilisateur *login_user, std::list<Utilisateur*> *list);
    bool add_user (Utilisateur *user);
    bool delete_user(Utilisateur *user_to_delete);
    bool connect_user(Utilisateur *login_user);
    QString get_sha1_from_Qstring(QString mdp);
    QString get_unique_token(void);
    void update_user_token_on_db(Utilisateur *login_user);

signals:
    void log_value_changed(bool is_user_logged);
};

#endif // CONNECT_DB_H

#ifndef CONNECT_DB_H
#define CONNECT_DB_H

#include <Qsql>
#include <QSqlDatabase>
#include <QObject>
#include "utilisateur.h"
#include "kit.h"
#include "item.h"
#include "resa.h"
#include "utils.h"

#include <list>
#include <iterator>

class Connect_db : public QObject
{
    Q_OBJECT

private:
    Utilisateur * active_user;

    void update_user_infos_from_db(Utilisateur *login_user);
    bool is_user_connected(void);

    bool get_querry_errors(QSqlQuery &query);
    void populate_kit_list_from_query(std::vector<Kit *> *kits, QSqlQuery query);
    bool runQuery(QSqlQuery &query, QString query_string);
    void populate_resa_list_from_query(std::vector<Resa *> *i_resa, QSqlQuery query);
public:

    QSqlDatabase db;
    Connect_db();
    void select_all_users(std::vector<Utilisateur*> *list);
    void select_all_kits(std::vector<Kit*> *kits);
    bool add_user (Utilisateur *user);
    bool add_kit (Kit *kit);
    bool delete_user(Utilisateur *user_to_delete);
    bool connect_user();
    QString get_sha1_from_Qstring(QString mdp);
    QString get_unique_token(void);
    void update_user_token_on_db(Utilisateur *login_user);
    void setActiveUser(Utilisateur * p_user);
    void select_items_by_kit (Kit * kit);
    void populate_item_list_from_query(Kit * kit, QSqlQuery query);

    void set_kit_booked_status(std::vector<Kit*> *i_kits, QDate i_date);
    void select_all_resa(std::vector<Resa *> *i_resa);
signals:
    void log_value_changed(bool is_user_logged);
};

#endif // CONNECT_DB_H

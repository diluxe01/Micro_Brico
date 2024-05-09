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

    //-----------General----------------
    bool get_querry_errors(QSqlQuery &query);
    bool runQuery(QSqlQuery &query, QString query_string);
    //-----------User-------------------
    void update_user_infos_from_db(Utilisateur *login_user);
    bool is_user_connected(void);
    //-----------Kit--------------------
    void populate_kit_list_from_query(std::vector<Kit *> *kits, QSqlQuery query);
    //-----------Resa-------------------
    void populate_resa_list_from_query(std::vector<Resa *> *i_resa, QSqlQuery query);

public:
    //-----------General----------------
    QSqlDatabase db;
    Connect_db();
    //-----------User-------------------
    void select_all_users(std::vector<Utilisateur*> *list);
    void add_user (Utilisateur *user);
    void setActiveUser(Utilisateur * p_user);
    bool delete_user(Utilisateur *user_to_delete);
    bool connect_user();
    QString get_sha1_from_Qstring(QString mdp);
    QString get_unique_token(void);
    void update_user_token_on_db(void);
    bool get_user_id_by_mail(QString mail, uint * o_user_id);
    //-----------Kit-------------------
    void select_kits_by_code(std::vector<Kit *> *kits, QString code);
    void select_kits_by_name(std::vector<Kit *> *kits, QString code);
    void add_kit (Kit *);
    void update_kit (Kit *);
    void add_item(Item *p_item, QString i_idkit);
    void select_all_kits(std::vector<Kit*> *kits);
    void select_items_by_kit (Kit * );
    void populate_item_list_from_query(Kit * kit, QSqlQuery query);
    void set_kit_booked_status(std::vector<Kit*> *i_kits, QDate i_date);
    //-----------Resa-------------------
    void select_all_resa(std::vector<Resa *> *i_resa);
    void select_resa_by_user(std::vector<Resa *> *o_resa, uint user_id);
    uint32_t guess_next_resa_nb(void);
    void add_resa_from_kit(Kit *i_p_kit, uint user_id, QDate i_start_date, int i_resa_nb);
    void start_resa();
    void end_resa();
    void delete_resa(int i_resa_nb);

public slots:
signals:
    void log_value_changed(bool is_user_logged);
};

#endif // CONNECT_DB_H

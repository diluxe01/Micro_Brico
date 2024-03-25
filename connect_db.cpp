#include "connect_db.h"
#include <Qsql>
#include <qstring.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QSqlError>
#include <QDateTime>

Connect_db::Connect_db() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "dbtest");
    db.setHostName("localhost");
    db.setDatabaseName("dbtest");
    db.setUserName("adrien");
    db.setPassword("adrien");
    bool ok = db.open();

    this->db = db;

    }

bool Connect_db::runQuery(QSqlQuery &query, QString query_string)
{
    qDebug() << query_string;
    if (is_user_connected() == true)
    {
        query.exec(query_string);
        get_querry_errors(query);
    }
}

/*Returns true if last query had an error
return false otherwise */
bool Connect_db::get_querry_errors(QSqlQuery &query)
{
    if (query.lastError().isValid())
        {
            qInfo() << query.lastError();
            return true;
        }
    return false;
}

bool Connect_db::add_user (Utilisateur *user)  {
    QSqlQuery query  = QSqlQuery(this->db);
    runQuery(query, "insert into utilisateur "
               "( nom, mdp, prenom, email, token, utinfo)"
               " values('"+user->getNom()+"','"+user->getMdp()+"','"+user->getPrenom()+"','"+user->getEmail()+"','"+user->getToken()+"','"+user->getUtinfo()+"')");
    return true;
}

bool Connect_db::add_kit (Kit *kit)  {
    vector<Item*>::iterator it;
    bool returnValue = false;
    QString idkit;
    QSqlQuery query  = QSqlQuery(this->db);
    //add a kit in DB
    QString exec_string = "insert into kit "
                          "( nom, description, date_achat, prix_achat, texte_libre, en_panne, code_kit, caution) "
                          "values('"+kit->getNom()+"','"+kit->getDescription()+"','"+kit->getDate_achat().toString(Qt::ISODateWithMs)+"','"+kit->getPrix_achat().getStringValue()+"','"+kit->getTexte_libre()+"','0','"+kit->getCode()+"','"+kit->getCaution().getStringValue()+"')";

    runQuery(query, exec_string);
    //Add associated items to DB if no errors
    if (!get_querry_errors(query))
    {
        exec_string = "select idkit from kit where code_kit='"+kit->getCode()+"'";
        query.exec(exec_string);
        if (!get_querry_errors(query))
        {
            query.first();
            idkit = query.value("idkit").toString();
            for (it = kit->item_list.begin(); it != kit->item_list.end(); it++)
            {

                exec_string = "insert into item (name,forkey, etat) values('"+(*it)->getName()+"', '"+idkit+"', '"+(*it)->getEtatStr()+"')";
                query.exec(exec_string);
                if (!get_querry_errors(query))
                {
                    returnValue = true;
                }
            }
        }


    }

    return returnValue;
}

bool Connect_db::delete_user(Utilisateur *user_to_delete)
{
    QSqlQuery query  = QSqlQuery(this->db);
    if (user_to_delete->getEmail()!= "")
    {
        runQuery(query, "delete from Utilisateur where email = '"+user_to_delete->getEmail()+"'");
    }
    else if (user_to_delete->getUtinfo()!= "")
    {
        runQuery(query, "delete from Utilisateur where utinfo = '"+user_to_delete->getUtinfo()+"'");
    }
    return true;
}

QString Connect_db::get_sha1_from_Qstring(QString mdp)
{
    QString mdp_sha1 = "";
    QCryptographicHash * hash = new QCryptographicHash(QCryptographicHash::Sha1);
    hash->addData(mdp.toStdString());
    QByteArray hashTs = hash->result().toHex();
    mdp_sha1 = QString(hashTs);
    return mdp_sha1;
}

QString Connect_db::get_unique_token(void)
{
    QString token;
    QDateTime datetime;
    QString date = datetime.currentDateTime().toString();
    token = Connect_db::get_sha1_from_Qstring(date);
    return token;
}

void Connect_db::update_user_token_on_db(void)
{
    QString query_string =  "update utilisateur set token ='"+this->active_user->getToken()+"'where email ='"+this->active_user->getEmail()+"'";
    QSqlQuery query(this->db);
    query.exec(query_string);
}

bool Connect_db::is_user_connected(void)
{
    // This function checks if the user given as a parameter is still connected, by comparing its token with the token on the server
    // If not, this functions emits a signal to the appli, to inform that user is disconnected
    QString token_db;
    QString query_string =  "SELECT token from utilisateur where email ='"+this->active_user->getEmail()+"'";
    QSqlQuery query(this->db);

    // If no error in query
    query.exec(query_string);
    query.first();
    token_db = query.value("token").toString();

    if (token_db != this->active_user->getToken())
    {
        qDebug() <<  "token_db: "<<token_db;
        qDebug() <<  "login_user->getToken(): "<<this->active_user->getToken();
        emit log_value_changed(false);
        return false;
    }
}

// Return true if connection succedded
bool Connect_db::connect_user(void)
{
    QString query_string =  "SELECT mdp from utilisateur where email ='"+this->active_user->getEmail()+"'";
    QSqlQuery query( this->db);
    QString mdp_db = "";
    QString mdp_user_sha1 = "";
    bool returnVal = false;
    query.exec(query_string);
    query.next();
    mdp_db = query.value("mdp").toString();
    qDebug() <<  query_string;
    qDebug() <<  "MDP from DB is: "<<mdp_db;

    //Convert user mdp to sha1
    mdp_user_sha1 = get_sha1_from_Qstring(this->active_user->getMdp());
    qDebug() <<  "MDP from USER is: " << mdp_user_sha1;

    if (mdp_db == mdp_user_sha1)
    {
        //Update connection token for user on server
        this->active_user->setToken(this->get_unique_token());
        this->update_user_token_on_db();
        returnVal = true;
        // Retrieve every informations about connected user
        update_user_infos_from_db(this->active_user);
        // Send connection status change to main appli
        emit log_value_changed(true);
    }
    else
    {
        qInfo() << "Utilisateur non identifié. Mauvais mdp ou email.";
        returnVal = false;
        // Send connection status change to main appli
        emit log_value_changed(false);
    }
    return returnVal;
}

void Connect_db::update_user_infos_from_db(Utilisateur *login_user)
{
    QSqlQuery query  = QSqlQuery(this->db);
    runQuery(query, "SELECT id, nom, prenom, utinfo, privilege from utilisateur where email = '"+login_user->getEmail()+"'");
    query.first();
    login_user->setNom(query.value("nom").toString());
    login_user->setPrenom(query.value("prenom").toString());
    login_user->setUtinfo(query.value("utinfo").toString());
    login_user->setId(query.value("id").toInt());
    if (query.value("privilege").toString() == "admin")
    {
        login_user->setPrivilege(E_admin);
    }
    else
    {
        login_user->setPrivilege(E_basic);
    }
}

void  Connect_db::select_all_users (std::vector<Utilisateur*> *list)
{
    QSqlQuery query2 = QSqlQuery(this->db);
    runQuery(query2, "SELECT id, nom, mdp, prenom, email, utinfo, token from utilisateur");
    while (query2.next())
    {
        QString nom = query2.value("nom").toString();
        QString mdp = query2.value("mdp").toString();
        QString prenom = query2.value("prenom").toString();
        QString email = query2.value("email").toString();
        QString utinfo = query2.value("utinfo").toString();
        QString token = query2.value("token").toString();
        int id = query2.value("id").toInt();

        Utilisateur * u = new Utilisateur(nom, mdp,prenom, email, utinfo );
        u->setId(id);
        u->setToken(token);
        list->push_back(u);
    }
}


void  Connect_db::select_items_by_kit (Kit * kit)
{
    QSqlQuery query(this->db);
    runQuery(query,"SELECT id, name, etat from item where forkey ='"+QString::number(kit->getIdKit())+"'");
    populate_item_list_from_query(kit, query);
}

void Connect_db::populate_item_list_from_query(Kit * kit, QSqlQuery query)
{
    while (query.next())
    {
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString etat = query.value("etat").toString();

        Item * i = new Item();
        i->setid((uint)id);
        i->setName(name);
        i->setEtatStr(etat);

        kit->item_list.push_back(i);
    }
}

void  Connect_db::select_all_kits (std::vector<Kit *> *kits)
{
    QSqlQuery query(this->db);
    runQuery(query, "SELECT idkit, nom, description, date_achat, prix_achat, texte_libre, en_panne, code_kit, caution from kit");

    populate_kit_list_from_query(kits, query);
}

void  Connect_db::select_kits_by_code (std::vector<Kit*> *kits, QString code)
{
    QSqlQuery query(this->db);
    runQuery(query, "SELECT idkit, nom, description, date_achat, prix_achat, texte_libre, en_panne, code_kit, caution from kit where code_kit REGEXP '"+code+"'");
    populate_kit_list_from_query(kits, query);
}

void  Connect_db::select_kits_by_name (std::vector<Kit*> *kits, QString code)
{
    QSqlQuery query(this->db);
    runQuery(query, "SELECT idkit, nom, description, date_achat, prix_achat, texte_libre, en_panne, code_kit, caution from kit where nom REGEXP '"+code+"'");
    populate_kit_list_from_query(kits, query);
}


void Connect_db::setActiveUser(Utilisateur *p_user)
{
    active_user = p_user;
}

void Connect_db::populate_kit_list_from_query(std::vector<Kit*> *kits, QSqlQuery query)
{
    while (query.next())
    {
        int id = query.value("idkit").toInt();
        QString nom = query.value("nom").toString();
        QString description = query.value("description").toString();
        QString date_achat = query.value("date_achat").toString();
        QString prix_achat = query.value("prix_achat").toString();
        QString texte_libre = query.value("texte_libre").toString();
        int en_pane = query.value("en_panne").toInt();
        QString code_kit = query.value("code_kit").toString();
        QString caution = query.value("caution").toString();

        bool bool_panne;
        if (en_pane == 1)
            bool_panne = true;
        else
            bool_panne = false;
        Kit * k = new Kit(id,nom, description,date_achat,prix_achat,texte_libre, bool_panne,code_kit, caution);

        kits->push_back(k);
    }
}


void  Connect_db::set_kit_booked_status (std::vector<Kit*> *i_kits, QDate i_date)
{
    std::vector<Resa *> resa_list;
    select_all_resa(&resa_list);
    bool is_booked = false;

    // Determine if id kit is in resa table at given date. If so, set "is_booked" var to true and break "for" loop.
    for(const auto& elem_kit : *i_kits)
    {
        is_booked = false;
        for(const auto& elem_resa : resa_list)
        {
            if ((elem_kit->getIdKit() == elem_resa->getId_kit())
                &&(i_date == elem_resa->getStart_date()))
            {
                is_booked = true;
                break;
            }
        }

        elem_kit->setIs_booked(is_booked);
    }


}

void  Connect_db::select_all_resa (std::vector<Resa *> *i_resa)
{
    QSqlQuery query(this->db);
    runQuery(query, "SELECT * from resa");

    populate_resa_list_from_query(i_resa, query);
}

void Connect_db::populate_resa_list_from_query(std::vector<Resa *> *i_resa, QSqlQuery query)
{
    QDate qdate;
    while (query.next())
    {
        int id = query.value("id").toInt();
        QString start_date = query.value("start_date").toString();
        int id_kit = query.value("id_kit").toInt();
        int id_user = query.value("id_user").toInt();

        qdate = QDate::fromString(start_date,"yyyy-MM-dd");

        Resa * k = new Resa(id,qdate,id_kit,id_user);

        i_resa->push_back(k);
    }
}

//Returns next reservation number by incr. the last number inserted.
// Updates uid_resa table with incremented number
uint32_t Connect_db::guess_next_resa_nb(void)
{
    QSqlQuery query(this->db);
    runQuery(query, "SELECT iduid_resa from uid_resa order by iduid_resa DESC limit 1");
    query.next();

    int id = query.value("iduid_resa").toInt();
    id += 1 ;
    runQuery(query, "insert into uid_resa (iduid_resa) values  ("+QString::number(id)+")");
    return id;
}


void Connect_db::add_resa_from_kit(Kit *i_p_kit, Utilisateur *i_p_user, QDate i_start_date, int i_resa_nb)
{
    QSqlQuery query(this->db);
    QSqlQuery query2(this->db);
    //First check if kit is not already booked at this date:
    runQuery(query,"select id from resa where start_date = "
                            "'"+i_start_date.toString("yyyy-MM-dd")+"' and  id_kit = "
                            "'"+QString::number(i_p_kit->getIdKit())+"'");
    if (query.next())
    {
        qWarning()<< "Le Kit " << i_p_kit->getNom() <<" que vous souhaitez reserver a été reservé par un autre utilisateur pendant que vous constituiez votre panier. Pas de chance.";
    }
    else
    {
        //The kit is free at this date, it is ok to book it.
        // query.clear();
        runQuery(query2,"insert into resa (start_date, id_user, id_kit, id_resa) values("
                        "'"+i_start_date.toString("yyyy-MM-dd")+"', "
                        "'"+QString::number(i_p_user->getId())+"', "
                        "'"+QString::number(i_p_kit->getIdKit())+"',"
                        "'"+QString::number(i_resa_nb)+"')");

        qInfo()<< "Le Kit " << i_p_kit->getNom() <<" a été reservé avec succès. N° de réservation: " << QString::number(i_resa_nb) ;
    }
}

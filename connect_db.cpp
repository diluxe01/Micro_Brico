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

bool Connect_db::get_querry_errors(QSqlQuery &query)
{
    if (query.lastError().isValid())
        {
            // qDebug() << query.lastError();
            qInfo() << query.lastError();
            return true;
        }
    return false;
}
bool Connect_db::add_user (Utilisateur *user)  {
    QSqlQuery query  = QSqlQuery(this->db);
    query.exec("insert into utilisateur "
               "( nom, mdp, prenom, email, token, utinfo)"
               " values('"+user->getNom()+"','"+user->getMdp()+"','"+user->getPrenom()+"','"+user->getEmail()+"','"+user->getToken()+"','"+user->getUtinfo()+"')");
    get_querry_errors(query);
    return true;
}

bool Connect_db::add_kit (Kit *kit)  {
    list<Item*>::iterator it;
    bool returnValue = false;
    QString idkit;
    QSqlQuery query  = QSqlQuery(this->db);
    //add a kit in DB
    QString exec_string = "insert into kit "
                          "( nom, description, date_achat, prix_achat, texte_libre, en_panne, code_kit, caution) "
                          "values('"+kit->getNom()+"','"+kit->getDescription()+"','"+kit->getDate_achat().toString(Qt::ISODateWithMs)+"','"+kit->getPrix_achat().getStringValue()+"','"+kit->getTexte_libre()+"','0','"+kit->getCode()+"','"+kit->getCaution().getStringValue()+"')";

    qDebug() << exec_string;
    query.exec(exec_string);

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
        query.exec("delete from Utilisateur where email = '"+user_to_delete->getEmail()+"'");
    }
    else if (user_to_delete->getUtinfo()!= "")
    {
        query.exec("delete from Utilisateur where utinfo = '"+user_to_delete->getUtinfo()+"'");
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

void Connect_db::update_user_token_on_db(Utilisateur *login_user)
{
    QString query_string =  "update utilisateur set token ='"+login_user->getToken()+"'where email ='"+login_user->getEmail()+"'";
    QSqlQuery query(this->db);
    query.exec(query_string);
}

bool Connect_db::is_user_connected(Utilisateur *login_user)
{
    // This function checks if the user given as a parameter is still connected, by comparing its token with the token on the server
    // If not, this functions emits a signal to the appli, to inform that user is disconnected
    QString token_db;
    QString query_string =  "SELECT token from utilisateur where email ='"+login_user->getEmail()+"'";
    QSqlQuery query(this->db);
    query.exec(query_string);
    query.first();
    token_db = query.value("token").toString();

    if (token_db != login_user->getToken())
    {
        qDebug() <<  "token_db: "<<token_db;
        qDebug() <<  "login_user->getToken(): "<<login_user->getToken();
        emit log_value_changed(false);
        return false;
    }
    return true;
}

// Return true if connection succedded
bool Connect_db::connect_user(Utilisateur *login_user)
{
    QString query_string =  "SELECT mdp from utilisateur where email ='"+login_user->getEmail()+"'";
    QSqlQuery query(query_string, this->db);
    QString mdp_db = "";
    QString mdp_user_sha1 = "";
    bool returnVal = false;

    query.next();
    mdp_db = query.value("mdp").toString();
    qDebug() <<  query_string;
    qDebug() <<  "MDP from DB is: "<<mdp_db;

    //Convert user mdp to sha1
    mdp_user_sha1 = get_sha1_from_Qstring(login_user->getMdp());

    qDebug() <<  "MDP from USER is: "<<mdp_user_sha1;

    if (mdp_db == mdp_user_sha1)
    {
        qDebug() << "User is identified";
        returnVal = true;
        update_user_infos_from_db(login_user);
        emit log_value_changed(true);
    }
    else
    {
        qDebug() << "Utilisateur non identifié. Mauvais mdp ou email.";
        returnVal = false;
        emit log_value_changed(false);
    }
    return returnVal;
}

void Connect_db::update_user_infos_from_db(Utilisateur *login_user)
{
    QSqlQuery query  = QSqlQuery(this->db);
    query.exec("SELECT id, nom, prenom, utinfo, privilege from utilisateur where email = '"+login_user->getEmail()+"'");
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

void  Connect_db::select_all_users (std::list<Utilisateur*> *list)
{
    QSqlQuery query2("SELECT id, nom, mdp, prenom, email, utinfo, token from utilisateur", this->db);
    get_querry_errors(query2);
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

void  Connect_db::select_all_kits (std::list<Kit*> *kits)
{
    QSqlQuery query2("SELECT idkit, nom, description, date_achat, prix_achat, texte_libre, en_panne, code_kit, caution from kit", this->db);
    get_querry_errors(query2);

    populate_kit_list_from_query(kits, query2);
}

void  Connect_db::select_kits_by_code (std::list<Kit*> *kits, QString code)
{
    QSqlQuery query2("SELECT idkit, nom, description, date_achat, prix_achat, texte_libre, en_panne, code_kit, caution from kit where code_kit='"+code+"'", this->db);
    get_querry_errors(query2);
    populate_kit_list_from_query(kits, query2);
}

void Connect_db::populate_kit_list_from_query(std::list<Kit*> *kits, QSqlQuery query)
{
    while (query.next())
    {
        get_querry_errors(query);
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

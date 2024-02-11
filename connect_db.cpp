#include "connect_db.h"
#include <Qsql>
#include <qstring.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QSqlError>


Connect_db::Connect_db() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "dbtest");
    db.setHostName("localhost");
    db.setDatabaseName("dbtest");
    db.setUserName("adrien");
    db.setPassword("adrien");
    bool ok = db.open();

    this->db = db;

    }

bool Connect_db::add_user (Utilisateur *user)  {
    QSqlQuery query  = QSqlQuery(this->db);
    query.exec("insert into utilisateur "
               "( nom, mdp, prenom, email, token, utinfo)"
               " values('"+user->getNom()+"','"+user->getMdp()+"','"+user->getPrenom()+"','"+user->getEmail()+"','"+user->getToken()+"','"+user->getUtinfo()+"')");
    return true;
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

bool Connect_db::is_user_identified(Utilisateur *login_user)
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
    }
    else
    {
        qDebug() << "Utilisateur non identifié. Mauvais mdp ou email.";
        returnVal = false;
    }

    Connect_db::update_user_infos_from_db(login_user);
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

void  Connect_db::select_all_users (std::list<Utilisateur*> *list)  {

    QSqlDatabase dbtest = QSqlDatabase::database();
    QSqlQuery query2("SELECT id, nom, mdp, prenom, email, utinfo from utilisateur", this->db);
    while (query2.next())
    {
        QString nom = query2.value("nom").toString();
        QString mdp = query2.value("mdp").toString();
        QString prenom = query2.value("prenom").toString();
        QString email = query2.value("email").toString();
        QString utinfo = query2.value("utinfo").toString();
        int id = query2.value("id").toInt();

        Utilisateur * u = new Utilisateur(nom, mdp,prenom, email, utinfo );
        u->setId(id);
        list->push_back(u);
    }

}

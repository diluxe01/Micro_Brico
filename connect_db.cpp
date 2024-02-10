#include "connect_db.h"
#include <Qsql>
#include <qstring.h>
#include <QSqlDatabase>
#include <QSqlQuery>


Connect_db::Connect_db() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "dbtest");
    db.setHostName("localhost");
    db.setDatabaseName("dbtest");
    db.setUserName("adrien");
    db.setPassword("adrien");
    bool ok = db.open();

    this->db = db;

    }

QSqlQuery Connect_db::runQuery(QString str){
    qDebug() << str;
    QSqlQuery query(str, this->db);
    return query;
}


void Connect_db::add_user (Utilisateur *user)  {
    QSqlQuery query =  Connect_db::runQuery("insert into utilisateur "
                                           "( nom, mdp, prenom, email, token, utinfo)"
                                           " values('"+user->getNom()+"','"+user->getMdp()+"',"+user->getPrenom()+"',"+user->getEmail()+"',"+user->getToken()+"',"+user->getUtinfo()+"', )");
    while (query.next())
    {
        qDebug() << query.value(0).toString();
    }
}

bool Connect_db::is_user_identified(Utilisateur *login_user)
{
    QSqlQuery query("SELECT mdp from utilisateur where email ="+login_user->getEmail(), this->db);
    QString mdp_db = "";
    while (query.next())
    {
        mdp_db = query.value("mdp").toString();
    }
    if (mdp_db == login_user->getMdp())
    {

        qDebug() << "User is identified";
    }
    else
    {
        qDebug() << "Utilisateur non identifié. Mauvais mdp ou email.";
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

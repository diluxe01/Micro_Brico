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
    QSqlQuery query =  Connect_db::runQuery("insert into utilisateur (nom, mdp) values('"+user->getName()+"','"+user->getMdp()+"')");
    while (query.next())
    {
        qDebug() << query.value(0).toString();
    }
}

void  Connect_db::select_all_users (std::list<Utilisateur*> *list)  {

    QSqlDatabase dbtest = QSqlDatabase::database();

    QSqlQuery query1("SELECT database()", this->db);
    while (query1.next())
    {
        qDebug() << query1.value(0).toString();
    }
    QSqlQuery query2("SELECT id, nom, mdp from utilisateur", this->db);

    while (query2.next())
    {
        QString nom = query2.value("nom").toString();
        QString mdp = query2.value("mdp").toString();
        int id = query2.value("id").toInt();

        Utilisateur * u = new Utilisateur(nom, id, mdp);
        list->push_back(u);
    }

}

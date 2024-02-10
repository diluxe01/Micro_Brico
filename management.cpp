#include "qdebug.h"

Management::Management(QObject *parent)
    : QObject{parent}
{

}

void Management::addUserToDB(void)
{
    g_connect_db.add_user(&this->new_user);

    qDebug() << "MGMT: AddUser to DB;)";
}


Utilisateur * Management::getNewUserRef(void)
{
    return &(this->new_user);
}

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
    db.setDatabaseName("db_dev");
    db.setUserName("adrien");
    db.setPassword("adrien");
    bool ok = db.open();

    this->db = db;

    }

bool Connect_db::runQuery(QSqlQuery &query, QString query_string)
{
    bool querry_had_errors = false;
    qDebug().noquote() << query_string;
    if (is_user_connected() == true)
    {
        query.exec(query_string);
        querry_had_errors = get_querry_errors(query);
    }
    return querry_had_errors;
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

void Connect_db::add_user (Utilisateur *user)  {
    QSqlQuery query  = QSqlQuery(this->db);
    runQuery(query, "insert into utilisateur "
               "( nom, mdp, prenom, email, token, utinfo)"
               " values('"+user->getNom()+"','"+user->getMdp()+"','"+user->getPrenom()+"','"+user->getEmail()+"','"+user->getToken()+"','"+user->getUtinfo()+"')");
}
///
/// \brief Connect_db::add_item adds item to database,
/// \param p_item: pointer to item to add to db
/// \param i_idkit: idkit (foreign key of item) associated to added item
///
void Connect_db::add_item (Item *p_item, QString i_idkit)  {

    QSqlQuery query  = QSqlQuery(this->db);
    QString exec_string = "insert into item (name, forkey, quantity) values('"+p_item->getName()+"', '"+i_idkit+"', '"+QString::number(p_item->getQuantity())+"')";

    runQuery(query, exec_string);
}

void Connect_db::add_kit (Kit *kit)  {
    vector<Item*>::iterator it;
    QString idkit;
    QSqlQuery query  = QSqlQuery(this->db);
    //add a kit in DB
    QString exec_string = "insert into kit "
                          "( nom, description, date_achat, prix_achat, texte_libre, en_panne, code_kit, caution) "
                          "values(\""+kit->getNom()+"\",\""+kit->getDescription()+"\",'"+kit->getDate_achat().toString(Qt::ISODateWithMs)+"','"+kit->getPrix_achat().getStringValue()+"',\""+kit->getTexte_libre()+"\",'0','"+kit->getCode()+"','"+kit->getCaution().getStringValue()+"')";

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
                add_item(*it, idkit);
            }
        }
    }
}


void Connect_db::update_kit (Kit *i_kit)  {
    vector<Item*>::iterator it;
    Kit l_db_kit;
    bool item_matching = false;

    QSqlQuery query  = QSqlQuery(this->db);
    //add a kit in DB
    QString exec_string = "update kit set  nom = \"%1\" ,  description = \"%2\", date_achat = \"%3\",  prix_achat = \"%4\", texte_libre = \"%5\",en_panne = \"%6\", code_kit = \"%7\", caution = \"%8\" where idkit = %9";
    exec_string = exec_string .arg(i_kit->getNom())
                              .arg(i_kit->getDescription())
                              .arg(i_kit->getDate_achat().toString(Qt::ISODateWithMs))
                              .arg(i_kit->getPrix_achat().getStringValue())
                              .arg(i_kit->getTexte_libre())
                              .arg("0")
                              .arg(i_kit->getCode())
                              .arg(i_kit->getCaution().getStringValue())
                              .arg(QString::number(i_kit->getIdKit()));

    runQuery(query, exec_string);
    //Add associated items to DB if no errors
    if (!get_querry_errors(query))
    {
        //Get every items of kit in DB
        l_db_kit.setIdkit(i_kit->getIdKit());
        this->select_items_by_kit(&l_db_kit);

        // Iterate through items of db, and compare to new items to find those: deleted or edited
        for(const auto& elem_item_db : l_db_kit.item_list)
        {
            item_matching = false;
            for(const auto& elem_item_new : i_kit->item_list)
            {
                // 1) There is a matching edited item => do nothing
                if (elem_item_db->getId() == elem_item_new->getId())
                {
                    item_matching = true;
                    // exec_string = "update item set name = \""+elem_item_new->getName()+"\" where id =  "+ QString::number(elem_item_new->getId());
                    // runQuery(query, exec_string);
                    break;
                }
            }
            // 2) There is no matching edited item => delete item
            if (item_matching == false)
            {
                exec_string = "delete from item where id =  "+ QString::number(elem_item_db->getId());
                runQuery(query, exec_string);
            }
        }

        // Iterate through new items, and compare to items on db to find those : added
        for(const auto& elem_item_new : i_kit->item_list)
        {
            item_matching = false;
            for(const auto& elem_item_db : l_db_kit.item_list)
            {
                if (elem_item_db->getId() == elem_item_new->getId())
                {
                    item_matching = true;
                }
            }
            // 3) if no item is matching then it is a new item => add item
            if (item_matching == false)
            {
                add_item(elem_item_new, QString::number(i_kit->getIdKit()));
            }
        }
    }
}

///
/// \brief Connect_db::update_items_quantity_of_kit
/// Update the quantity of the items of kit pointed by i_kit, with the quantity of matching items in i_new_items list
/// \param i_kit: kit where item are to be changed
/// \param i_new_items: list containing items whith new quantity
///
void Connect_db::update_items_quantity_of_kit(Kit * i_kit, std::vector<Item *> i_new_items)  {

    QSqlQuery query  = QSqlQuery(this->db);
    QString exec_string = "";
    for(const auto& elem_new_item : i_new_items)
    {
        for(const auto& elem_kit_item : i_kit->item_list)
        {
            if (elem_new_item->getId() == elem_kit_item->getId())
            {
                if (elem_new_item->getQuantity() != elem_kit_item->getQuantity())
                {
                    exec_string = "update item set quantity = " +QString::number(elem_new_item->getQuantity())+" where id = " + QString::number(elem_new_item->getId()) ;
                    runQuery(query, exec_string);
                }
            }
        }
    }
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


///
/// \brief Connect_db::get_user_id_by_mail safely gives the user id, given its mail.
/// \param i_mail: user booking email
/// \param o_user: pointer where user is to be stored
/// \return true if an error occured, false otherwise
///
bool Connect_db::get_user_by_mail(QString i_mail, Utilisateur * o_user)
{
    QSqlQuery query  = QSqlQuery(this->db);

    bool querry_had_errors = false;
    runQuery(query, "SELECT id, nom, mdp, prenom, utinfo, token from utilisateur where email='"+i_mail+"'");
    if (query.size() == 0)
    {
        querry_had_errors = true;
        qDebug() <<  "get_user_id_by_mail: Une erreur s'est produite, l'email fourni est inconnu.";
    }
    else
    {   //no errors
        query.first();
        o_user->setNom(query.value("nom").toString());
        o_user->setMdp(query.value("mdp").toString());
        o_user->setPrenom(query.value("prenom").toString());
        o_user->setUtinfo(query.value("utinfo").toString());
        o_user->setToken(query.value("token").toString());
        o_user->setId(query.value("id").toInt());
        o_user->setEmail(i_mail);
    }
    return querry_had_errors;
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
    //Convert user mdp to sha1
    mdp_user_sha1 = get_sha1_from_Qstring(this->active_user->getMdp());

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
    runQuery(query2, "SELECT id, nom, mdp, prenom, email, utinfo, token, privilege from utilisateur");
    while (query2.next())
    {
        QString nom = query2.value("nom").toString();
        QString mdp = query2.value("mdp").toString();
        QString prenom = query2.value("prenom").toString();
        QString email = query2.value("email").toString();
        QString utinfo = query2.value("utinfo").toString();
        QString token = query2.value("token").toString();
        QString privilege_str = query2.value("privilege").toString();


        int id = query2.value("id").toInt();

        Utilisateur * u = new Utilisateur(nom, mdp,prenom, email, utinfo );
        u->setId(id);
        u->setToken(token);
        if (privilege_str == "admin")
        {
            u->setPrivilege(E_admin);
        }
        else
        {
            u->setPrivilege(E_basic);
        }

        list->push_back(u);
    }
}


void  Connect_db::select_items_by_kit (Kit * kit)
{
    QSqlQuery query(this->db);
    runQuery(query,"SELECT id, name, quantity from item where forkey ='"+QString::number(kit->getIdKit())+"'");
    g_utils.clearList(&kit->item_list);
    populate_item_list_from_query(kit, query);
}

void Connect_db::populate_item_list_from_query(Kit * kit, QSqlQuery query)
{
    while (query.next())
    {
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        int quantity = query.value("quantity").toInt();

        Item * i = new Item();
        i->setid((uint)id);
        i->setName(name);
        i->setQuantity(quantity);

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
        elem_kit->setIs_booked(false);
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

void Connect_db::select_resa_by_user(std::vector<Resa *> *o_resa, uint user_id)
{

    QSqlQuery query(this->db);
    runQuery(query, "SELECT * from resa WHERE id_user="+QString::number(user_id));

    populate_resa_list_from_query(o_resa, query);
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
        int id_resa = query.value("id_resa").toInt();

        qdate = QDate::fromString(start_date,"yyyy-MM-dd");

        Resa * k = new Resa(id,qdate,id_kit,id_user, id_resa);

        i_resa->push_back(k);
    }
}

///
/// \brief Connect_db::start_resa:
///    Lock resa table and uid_resa in write mode
///
///
void  Connect_db::start_resa(void)
{
    QSqlQuery query(this->db);
    runQuery(query, "LOCK TABLES uid_resa WRITE, resa WRITE, utilisateur READ");
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


void Connect_db::add_resa_from_kit(Kit *i_p_kit, uint user_id, QDate i_start_date, int i_resa_nb)
{
    QSqlQuery query(this->db);
    QSqlQuery query2(this->db);

    runQuery(query2,"insert into resa (start_date, id_user, id_kit, id_resa) values("
                    "'"+i_start_date.toString("yyyy-MM-dd")+"', "
                    "'"+QString::number(user_id)+"', "
                    "'"+QString::number(i_p_kit->getIdKit())+"',"
                    "'"+QString::number(i_resa_nb)+"')");

    qInfo()<< "Le Kit " << i_p_kit->getNom() <<" a été reservé avec succès. N° de réservation: " << QString::number(i_resa_nb) ;
}


///
/// \brief Connect_db::end_resa
///
void  Connect_db::end_resa(void)
{
    QSqlQuery query(this->db);
    runQuery(query, "UNLOCK TABLES");
}

void Connect_db::delete_resa(int i_resa_nb)
{
    QSqlQuery query(this->db);
    runQuery(query, "delete from resa where id_resa = "+ QString::number(i_resa_nb));

}




void  Connect_db::select_all_sortie (std::vector<Sortie *> *i_sortie)
{
    QSqlQuery query(this->db);
    runQuery(query, "SELECT * from sortie");

    populate_sortie_list_from_query(i_sortie, query);
}

void Connect_db::populate_sortie_list_from_query(std::vector<Sortie *> *i_sortie, QSqlQuery query)
{
    QDate qdate;
    while (query.next())
    {
        int id = query.value("id").toInt();
        QString start_date = query.value("start_date").toString();
        int id_kit = query.value("id_kit").toInt();
        int id_user = query.value("id_user").toInt();
        int id_sortie = query.value("id_sortie").toInt();

        qdate = QDate::fromString(start_date,"yyyy-MM-dd");

        Sortie * k = new Sortie(id, qdate, id_kit, id_user, id_sortie);

        i_sortie->push_back(k);
    }
}

///
/// \brief Connect_db::set_kit_out_status
///     Set parameters "isOut" of kits in i_kits list to true if given kit is currently out.
///     Also sets parameter 'id_user_out" of kits to the id of the user currently having the kit out.
/// \param i_kits
/// \param i_user
///
void  Connect_db::set_kit_out_status (std::vector<Kit*> *i_kits)
{
    std::vector<Sortie *> sortie_list;
    select_all_sortie(&sortie_list);
    bool is_out = false;
    int id_user_out = 0;
    // Determine if id kit is in sortie table at given date. If so, set "is_out" var to true and break "for" loop.
    for(const auto& elem_kit : *i_kits)
    {
        is_out = false;
        elem_kit->setIs_out(false);
        for(const auto& elem_sortie : sortie_list)
        {
            if (elem_kit->getIdKit() == elem_sortie->getId_kit())
            {
                is_out = true;
                id_user_out = elem_sortie->getId_user();
                break;
            }
        }
        elem_kit->setIs_out(is_out);
        elem_kit->setId_user_out(id_user_out);
    }
}

void  Connect_db::start_sortie(void)
{
    QSqlQuery query(this->db);
    runQuery(query, "LOCK TABLES uid_sortie WRITE, sortie WRITE,resa WRITE, utilisateur READ, kit READ");
}

// void  Connect_db::insert_sortie (Kit * i_kit )
// {

// }

void  Connect_db::end_sortie(void)
{
    QSqlQuery query(this->db);
    runQuery(query, "UNLOCK TABLES");
}

//Returns next sortie number by incr. the last number inserted.
// Updates uid_sortie table with incremented number
uint32_t Connect_db::guess_next_sortie_nb(void)
{
    QSqlQuery query(this->db);
    runQuery(query, "SELECT iduid_sortie from uid_sortie order by iduid_sortie DESC limit 1");
    query.next();

    int id = query.value("iduid_sortie").toInt();
    id += 1 ;
    runQuery(query, "insert into uid_sortie (iduid_sortie) values  ("+QString::number(id)+")");

    return id;
}


void Connect_db::add_sortie_from_kit(Kit *i_p_kit, uint user_id, QDate i_start_date, int i_sortie_nb)
{
    QSqlQuery query(this->db);
    QSqlQuery query2(this->db);

    runQuery(query2,"insert into sortie (start_date, id_user, id_kit, id_sortie) values("
                    "'"+i_start_date.toString("yyyy-MM-dd")+"', "
                    "'"+QString::number(user_id)+"', "
                    "'"+QString::number(i_p_kit->getIdKit())+"',"
                    "'"+QString::number(i_sortie_nb)+"')");

    qInfo()<< "Le Kit " << i_p_kit->getNom() <<" a été sorti avec succès. N° de sortie: " << QString::number(i_sortie_nb) ;
}

#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QObject>
#include "utilisateur.h"
#include "connect_db.h"
#include "datamodel.h"

class Management : public QObject
{
    Q_OBJECT
private:
    Utilisateur new_user;

public:
    explicit Management(QObject *parent = nullptr);

    Utilisateur *getNewUserRef(void );

    void addUserToDB(void);

signals:
};

#endif // MANAGEMENT_H

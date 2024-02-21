#ifndef KIT_H
#define KIT_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include "money.h"

class Kit : public QObject
{
    Q_OBJECT
private:
    uint idkit;
    QString nom;
    QString description;
    QDateTime date_achat;
    Money prix_achat;
    QString texte_libre;
    bool en_panne;
    QString code;
    Money caution;
public:
    explicit Kit(QObject *parent = nullptr);

signals:
};

#endif // KIT_H

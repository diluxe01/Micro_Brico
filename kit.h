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
    Kit(uint idkit, const QString &nom, const QString &description, const QDateTime &date_achat, const Money &prix_achat, const QString &texte_libre, bool en_panne, const QString &code, const Money &caution);

signals:
};

#endif // KIT_H

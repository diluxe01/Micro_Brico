#ifndef KIT_H
#define KIT_H

#include <QObject>
#include <QString>
#include <QDate>
#include "money.h"
#include "item.h"
#include <list>
class Kit : public QObject
{
    Q_OBJECT
private:
    uint idkit;
    QString nom;
    QString description;
    QDate date_achat;
    Money prix_achat;
    QString texte_libre;
    bool en_panne;
    QString code;
    Money caution;


public:
    explicit Kit(QObject *parent = nullptr);
    Kit(uint idkit, const QString &nom, const QString &description, const QDate &date_achat, const Money &prix_achat, const QString &texte_libre, bool en_panne, const QString &code, const Money &caution);

    std::list<Item*> item_list;

    QString getNom() const;
    QString getDescription() const;
    QDate getDate_achat() const;
    Money getPrix_achat() const;
    QString getTexte_libre() const;
    bool getEn_panne() const;
    QString getCode() const;
    Money getCaution() const;

    void setIdkit(uint newIdkit);
    void setNom(const QString &newNom);
    void setDescription(const QString &newDescription);
    void setDate_achat(const QDate &newDate_achat);
    void setPrix_achat(const Money &newPrix_achat);
    void setTexte_libre(const QString &newTexte_libre);

    void setCaution(const Money &newCaution);

    void setEn_panne(bool newEn_panne);

    void setCode(const QString &newCode);

signals:
};

#endif // KIT_H

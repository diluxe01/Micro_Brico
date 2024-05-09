#ifndef ITEM_H
#define ITEM_H

#include <QObject>

// enum T_etat_item
// {
//     E_NEW,
//     E_WORN,
//     E_BROKEN,
//     E_LOST
// };

class Item : public QObject
{
    Q_OBJECT
private:
    uint id;
    QString name;

    uint forkey;
    uint quantity;
    // T_etat_item etat;

public:
    explicit Item(QObject *parent = nullptr);
    Item(uint id, const QString &name, uint forkey);
    Item(uint id, const QString &name, uint forkey, uint quantity);

    QString getName() const;
    uint getForkey() const;
    // T_etat_item getEtat() const;

    void setid(uint newid);
    void setName(const QString &newName);
    void setForkey(uint newForkey);

    // QString getEtatStr() const;
    // void setEtatStr(QString etat_str);
    uint getId() const;

    uint getQuantity() const;
    void setQuantity(uint newQuantity);

signals:
};

#endif // ITEM_H

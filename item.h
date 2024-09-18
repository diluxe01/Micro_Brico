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
    uint quantity_init;
    uint quantity_current;

    bool is_verified;
public:
    explicit Item(QObject *parent = nullptr);
    Item(uint id, const QString &name, uint forkey);
    Item(uint id, const QString &name, uint forkey, uint quantity_init, uint quantity_current);

    QString getName() const;
    uint getForkey() const;
    // T_etat_item getEtat() const;

    void setid(uint newid);
    void setName(const QString &newName);
    void setForkey(uint newForkey);

    // QString getEtatStr() const;
    // void setEtatStr(QString etat_str);
    uint getId() const;

    bool getIs_verified() const;
    void setIs_verified(bool newIs_verified);

    uint getQuantity_init() const;
    void setQuantity_init(uint newQuantity_init);

    uint getQuantity_current() const;
    void setQuantity_current(uint newQuantity_current);

signals:
};

#endif // ITEM_H

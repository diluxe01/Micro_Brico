#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include "types.h"

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
    uint quantity_out;

    bool is_verified;
    T_ITEM_STATE state;
public:
    explicit Item(QObject *parent = nullptr);
    Item(uint id, const QString &name, uint forkey);
    Item(uint id, const QString &name, uint forkey, uint quantity_init, uint quantity_current, uint quantity_out);

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

    uint getQuantity_out() const;
    void setQuantity_out(uint newQuantity_out);

    T_ITEM_STATE getState() const;
    void setState(T_ITEM_STATE newState);

signals:
};

#endif // ITEM_H

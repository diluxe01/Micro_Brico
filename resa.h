#ifndef RESA_H
#define RESA_H

#include <QObject>
#include <QDate>

class Resa: public QObject
{
    Q_OBJECT
private:
    int id;
    QDate start_date;
    int id_kit;
    int id_user;


public:
    Resa();
    Resa(int id, const QDate &start_date, int id_kit, int id_user);
    int getId() const;
    void setId(int newId);
    QDate getStart_date() const;
    void setStart_date(const QDate &newStart_date);
    int getId_kit() const;
    void setId_kit(int newId_kit);
    int getId_user() const;
    void setId_user(int newId_user);
};

#endif // RESA_H

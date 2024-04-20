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
    int id_resa;


public:
    Resa();
    Resa(int id, const QDate &start_date, int id_kit, int id_user, int id_resa);
    int getId() const;
    void setId(int newId);
    QDate getStart_date() const;
    void setStart_date(const QDate &newStart_date);
    int getId_kit() const;
    void setId_kit(int newId_kit);
    int getId_user() const;
    void setId_user(int newId_user);
    int getId_resa() const;
    void setId_resa(int newId_resa);
    QString toString();
};

#endif // RESA_H

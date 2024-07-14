#ifndef SORTIE_H
#define SORTIE_H

#include <QObject>
#include <QDate>

class Sortie : public QObject
{
    Q_OBJECT
private:
    int id;
    QDate start_date;
    int id_kit;
    int id_user;
    int id_sortie;

public:
    explicit Sortie(QObject *parent = nullptr);
    Sortie(int id, const QDate &start_date, int id_kit, int id_user, int id_sortie);

    int getId() const;
    QDate getStart_date() const;
    int getId_kit() const;

    void setId(int newId);
    void setStart_date(const QDate &newStart_date);
    void setId_kit(int newId_kit);

    void setId_user(int newId_user);

    int getId_user() const;

    void setId_sortie(int newId_sortie);

    int getId_sortie() const;

signals:
protected:
};

#endif // SORTIE_H

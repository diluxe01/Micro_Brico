#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QString>
#include <QDate>

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = nullptr);
    Log(int id_kit, int id_user, const QString &text);


    int getId_kit() const;
    void setId_kit(int newId_kit);

    int getId_user() const;
    void setId_user(int newId_user);

    QString getText() const;
    void setText(const QString &newText);

    QDateTime getDate() const;
    void setDate(const QDateTime &newDate);

private :
    int id_kit;
    int id_user;
    QString text;
    QDateTime date;
signals:
};

#endif // LOG_H

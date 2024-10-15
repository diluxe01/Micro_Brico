#include "log.h"

Log::Log(QObject *parent)
    : QObject{parent}
{}

int Log::getId_kit() const
{
    return id_kit;
}

void Log::setId_kit(int newId_kit)
{
    id_kit = newId_kit;
}

int Log::getId_user() const
{
    return id_user;
}

void Log::setId_user(int newId_user)
{
    id_user = newId_user;
}

QString Log::getText() const
{
    return text;
}

void Log::setText(const QString &newText)
{
    text = newText;
}

QDateTime Log::getDate() const
{
    return date;
}

void Log::setDate(const QDateTime &newDate)
{
    date = newDate;
}

Log::Log(int id_kit, int id_user, const QString &text) : id_kit(id_kit),
    id_user(id_user),
    text(text)
{}

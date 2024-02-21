#include "money.h"
#include "QStringList"

Money::Money()
{}

Money::Money(QString init_value)
{
    setValue(init_value);
}

QString Money::getStringValue()
{

}


bool Money::setValue(QString new_value)
{
    QStringList liste;
    uint32_t u32_value;
    if (cleanInput(new_value, (QString)","))
    {
        return true;
    }
    else if (cleanInput(new_value, (QString)"."))
    {
        return true;
    }
    else
    {
        this->u32_value = new_value.toInt();
        this->str_value = new_value+",00";
    }
}

bool Money::cleanInput(QString new_value, QString separator)
{
    QStringList liste;
    uint32_t u32_value;
    if (new_value.contains(separator))
    {
        liste = new_value.split(separator);

        liste[1].resize(2,'0');
        u32_value = liste[0].toInt()*100;
        u32_value += liste[1].toInt();
        this->u32_value = u32_value;
        this->str_value = liste[0]+","+liste[1];
        return true;
    }

}

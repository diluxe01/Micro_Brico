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


void Money::setValue(QString new_value)
{
    QStringList liste;
    uint32_t u32_value;
    if (new_value.contains(","))
    {
        liste = new_value.split(",");
        u32_value = liste[0].toInt()*100;
        u32_value += liste[1].toInt();
        this->u32_value = u32_value;
    }

}

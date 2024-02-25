#include "money.h"
#include "QStringList"
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

Money::Money()
{}

Money::Money(QString init_value)
{
    setValue(init_value);
}

uint32_t Money::getU32_value() const
{
    return u32_value;
}

QString Money::getPartie_entiere() const
{
    return partie_entiere;
}

QString Money::getStringValue()
{
    return this->str_value;
}

/*
This function returns true when setting a money value was possible
returns false when it wasn't
*/
bool Money::setValue(QString new_value)
{
    // Detecte un prix du type xxxx,yy
    QRegularExpression re("(?<entier>\\d+)[.,]+(?<decimal>\\d+)");
    QRegularExpressionMatch match = re.match(new_value);

    if (match.hasMatch())
    {
        this->partie_entiere = match.captured("entier");
        this->u32_value = this->partie_entiere.toInt()*100;
        this->partie_decimale = match.captured("decimal");
        this->partie_decimale.resize(2,'0');
        this->u32_value += this->partie_decimale.toInt();
        this->str_value = this->partie_entiere+"."+ this->partie_decimale;
        return true;
    }
    else
    {
        // Detecte un prix du type xxxx
        re.setPattern("(?<entier>\\d+)");
        match = re.match(new_value);
        if (match.hasMatch())
        {
            this->partie_entiere = match.captured("entier");
            this->partie_decimale = "00";
            this->u32_value = this->partie_entiere.toInt()*100;
            this->str_value = this->partie_entiere+".00";

            return true;
        }
        else
        {
            return false;
        }
    }
}


#ifndef MONEY_H
#define MONEY_H

# include <QString>

class Money
{
private:
    uint32_t u32_value;
    QString str_value;
    QString partie_entiere;
    QString partie_decimale;

public:
    Money();
    Money(QString init_value);

    QString getStringValue(void);


    bool setValue(QString new_value);
};

#endif // MONEY_H

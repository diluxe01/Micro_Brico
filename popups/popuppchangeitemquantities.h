#ifndef POPUPPCHANGEITEMQUANTITIES_H
#define POPUPPCHANGEITEMQUANTITIES_H

#include <QWidget>
#include "../item.h"
#include <qabstractbutton.h>
#include <QDialogButtonBox>

namespace Ui {
class PopuppChangeItemQuantities;
}

class PopuppChangeItemQuantities : public QWidget
{
    Q_OBJECT

public:

    void closeEvent(QCloseEvent *event);
    explicit PopuppChangeItemQuantities(QWidget *parent = nullptr);
    ~PopuppChangeItemQuantities();

    void set_form_from_item(Item *p_item);
    QPushButton *getOkButton();
    QPushButton *getCancelButton();
    bool get_form_data(Item *o_item);
private:
    Ui::PopuppChangeItemQuantities *ui;

    void GEN_raise_popup_warning(QString msg);
signals:
    void delete_popup(void);
};

#endif // POPUPPCHANGEITEMQUANTITIES_H

#ifndef POPPUPADDKIT_H
#define POPPUPADDKIT_H

#include <QDialog>
#include <QPushButton>
#include "../item.h"
#include "../kit.h"
#include "PopuppChangeItemQuantities.h"
#include <QListWidgetItem>
#include <list>

namespace Ui {
class PoppupAddKit;
}

class PoppupAddKit : public QDialog
{
    Q_OBJECT

public:
    explicit PoppupAddKit(QWidget *parent = nullptr);
    QPushButton* getOkButton(void);
    QPushButton *getCancelButton();
    ~PoppupAddKit();

    Kit *get_kit_from_form();
    void set_form_from_kit(Kit *);

    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_addobject_clicked();
    void on_pushButton_deleteitemfromlist_clicked();
    void on_spinBox_item_quantity_init_valueChanged(int arg1);
    void on_listWidget_itemBasket_itemDoubleClicked(QListWidgetItem *item);
    void on_popupchangeQtyok();
    void on_popupchangeQtyDestroyed();

private:
    Ui::PoppupAddKit *ui;
    Kit * kit;
    PopuppChangeItemQuantities * popupChangeQty;

    void push_back_new_item_on_tabWidget(Item *item);
    void GEN_raise_popup_warning(QString msg);
    Item *get_item_selected();
    void refresh_item_basket();
signals:
    void delete_popup(void);
};

#endif // POPPUPADDKIT_H

#ifndef POPPUPADDKIT_H
#define POPPUPADDKIT_H

#include <QDialog>
#include <QPushButton>
#include "../item.h"
#include "../kit.h"
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

private slots:
    void on_pushButton_addobject_clicked();

    void on_pushButton_deleteitemfromlist_clicked();


private:
    Ui::PoppupAddKit *ui;
    Kit * kit;
    void push_back_new_item_on_tabWidget(Item *item);
};

#endif // POPPUPADDKIT_H

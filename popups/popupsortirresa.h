#ifndef POPUPSORTIRRESA_H
#define POPUPSORTIRRESA_H

#include <QWidget>

#include <QPushButton>
#include <QListWidget>
#include "../utilisateur.h"
#include "../kit.h"

namespace Ui {
class PopupSortirResa;
}

class PopupSortirResa : public QWidget
{
    Q_OBJECT

public:
    explicit PopupSortirResa(QWidget *parent = nullptr);
    ~PopupSortirResa();

    QPushButton * getSortirButton();
    void setUser (Utilisateur *newUser);
    bool checkIfOk (void);

    Kit *getP_kit() const;
    void setP_kit(Kit *newP_kit);
    void refresh_source_item_list();

    void refresh_dest_item_list();
private slots:
    void on_listWidget_popupSortie_ItemSource_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_popupSortie_ItemDest_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_popupSortie_ItemSource_itemClicked(QListWidgetItem *item);

    void on_pushButton_popupSortie_pushDest_clicked();

private:
    Ui::PopupSortirResa *ui;
    Utilisateur *user;
    Kit *p_kit;
    std::vector<Item*> item_list_dest;
    Item *get_sel_item_from_QlistWidget(QListWidget *i_QlistWidget, std::vector<Item *> i_item_list);
    void GEN_raise_popup_warning(QString msg);
    void push_item_to_dest_list(int new_quantity);
    Item *get_item_from_id(std::vector<Item *> i_item_list, uint i_id);
};

#endif // POPUPSORTIRRESA_H

#include "poppupaddkit.h"
#include "ui_poppupaddkit.h"
#include <QStringList>
#include <QTableWidgetItem>
#include "../money.h"

PoppupAddKit::PoppupAddKit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PoppupAddKit)
{
    QStringList header_list;
    ui->setupUi(this);
    ui->tableWidget_itemBasket->setRowCount(0);
    ui->tableWidget_itemBasket->setColumnCount(2);
    header_list.append("Nom");
    header_list.append("Etat");
    ui->tableWidget_itemBasket->setHorizontalHeaderLabels(header_list);
}

PoppupAddKit::~PoppupAddKit()
{
    delete ui;
}

QPushButton* PoppupAddKit::getOkButton(void)
{
    return ui->pushButton_OK;
}

QPushButton* PoppupAddKit::getCancelButton(void)
{
    return ui->pushButton_annuler;
}

void PoppupAddKit::on_pushButton_addobject_clicked()
{

    QString item_text = ui->lineEdit_new_item->text();
    int tmp_int_etat;
    T_etat_item item_etat;
    // If user entered text
    if (item_text!= "")
    {
        tmp_int_etat = ui->comboBox->currentIndex();

        // Find corresponding item state from combobox
        switch (tmp_int_etat)
        {
            case 0:
                item_etat = E_NEW;
                break;
            case 1:
                item_etat = E_WORN;
                break;
            case 2:
                item_etat = E_BROKEN;
                break;
            case 3:
                item_etat = E_LOST;
                break;

        }
        //Push_back new item into private item list
        Item* item = new Item(0, item_text, 0, item_etat);
        this->item_list.push_back(item);

        //Push_back new item into display tab
        push_back_new_item_on_tabWidget(item);

    }

}

void PoppupAddKit::push_back_new_item_on_tabWidget(Item* item)
{
    int nb_row = this->item_list.size();
    ui->tableWidget_itemBasket->setRowCount(nb_row);
    QTableWidgetItem* p_widget_item_name;
    QTableWidgetItem* p_widget_item_state;
    // Set first item of last row (name)
    p_widget_item_name= new QTableWidgetItem(item->getName());
    ui->tableWidget_itemBasket->setItem(nb_row-1, 0, p_widget_item_name);

    // Set second item of last row (Etat)
    p_widget_item_state = new QTableWidgetItem(item->getEtatStr());
    ui->tableWidget_itemBasket->setItem(nb_row-1, 1, p_widget_item_state);
}

void PoppupAddKit::on_pushButton_deleteitemfromlist_clicked()
{
    std::list<Item*>::iterator it;
    if (this->item_list.empty() == false)
    {
        //Get current row
        int currentRow = ui->tableWidget_itemBasket->currentRow();

        //Delete element on tab widget
        ui->tableWidget_itemBasket->removeRow(currentRow);

        //Delete element on the list at given index
        it = this->item_list.begin();
        advance(it, currentRow);
        this->item_list.erase(it);
    }

}

Kit * PoppupAddKit::get_kit_from_form()
{
    bool ok_to_make_a_kit = true;
    QString caution = ui->lineEdit_kitcaution->text();
    QString prix = ui->lineEdit_kitprix->text();
    QString code = ui->lineEdit_kit_code->text();
    QString dateAchat = ui->lineEdit_kit_dateachat->text();
    QString nom = ui->lineEdit_kit_name->text();
    QString description = ui->textEdit_kit_description->toPlainText();
    QString texte_libre = ui->textEdit_kit_texte_libre->toPlainText();
   //uint idkit, const QString &nom, const QString &description, const QDateTime &date_achat, const Money &prix_achat, const QString &texte_libre, bool en_panne, const QString &code, const Money &caution

    Money prix_formatted;

    //Verify the date
    if ( (dateAchat != "") || (dateAchat.contains("/") == false) )
    {
        QStringList date_split = dateAchat.split("/");
        if (date_split.size() == 3)
        {
            QDate date_formatted(date_split[2].toInt(), date_split[1].toInt(), date_split[0].toInt());
        }
        else
        {
            qWarning() << "Erreur dans le format de la date: Doit avoir la forme: JJ/MM/AAAA";
            ok_to_make_a_kit = false;
        }
    }
    else{
        qWarning() << "Erreur dans le format de la date: Doit avoir la forme: JJ/MM/AAAA";
        ok_to_make_a_kit = false;
    }

    //Verify the price
    if (prix_formatted.setValue(prix) == false)
    {
        qWarning() << "Erreur dans le format du prix: Doit avoir la forme: 125,50";
        ok_to_make_a_kit = false;
    }


    if (ok_to_make_a_kit)
    {
        // Kit kit = new Kit(0, nom, description,/* date */ 0,  );
        Kit* p_kit = new Kit();
        return p_kit;
    }
    else
    {

        return NULL;
    }
}

// Kit PoppupAddKit::get_items_from_form()
// {

// }


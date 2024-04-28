#include "poppupaddkit.h"
#include "ui_poppupaddkit.h"
#include <QStringList>
#include <QTableWidgetItem>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

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
    this->kit = new Kit();
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
        this->kit->item_list.push_back(item);

        //Push_back new item into display tab
        push_back_new_item_on_tabWidget(item);

    }

}

void PoppupAddKit::push_back_new_item_on_tabWidget(Item* item)
{
    int nb_row = this->kit->item_list.size();
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
    if (this->kit->item_list.empty() == false)
    {
        //Get current row
        int currentRow = ui->tableWidget_itemBasket->currentRow();

        //Delete element on tab widget
        ui->tableWidget_itemBasket->removeRow(currentRow);

        //Delete element on the list at given index
        this->kit->item_list.erase(this->kit->item_list.begin()+currentRow);
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

    Money prix_formatted;
    Money caution_formatted;
    QDate date_formatted;

    QRegularExpression re("^(\\d\\d)[/ .]+(\\d\\d)[/ .]+(\\d\\d\\d\\d)$");
    QRegularExpressionMatch match = re.match(dateAchat);


    //-----------Verifie le nom------------
    if (nom =="")
    {
        qInfo() << "Erreur dans le format du nom: Veuillez nommer le kit";
        ok_to_make_a_kit = false;
    }
    //-----------Verifie le code------------
    if (code =="")
    {
        qInfo() << "Erreur dans le format du code: Veuillez donner un code";
        ok_to_make_a_kit = false;
    }
    //-----------Verifie la date------------
    if (match.hasMatch()) {
        QString day = match.captured(1); // day == "08"
        QString month = match.captured(2); // month == "12"
        QString year = match.captured(3); // year == "1985"
        if (date_formatted.setDate(year.toInt(),month.toInt(),day.toInt()) == false)
        {
            ok_to_make_a_kit = false;
            qInfo() << "Erreur dans le format de la date: Doit avoir le format: JJ/MM/AAAA";
        }
    }
    else
    {
        ok_to_make_a_kit = false;
        qInfo() << "Erreur dans le format de la date: Doit avoir le format: JJ/MM/AAAA";
    }

    //-----------Verifie le prix-----------
    if (prix_formatted.setValue(prix) == false)
    {
        qInfo() << "Erreur dans le format du prix: Doit avoir le format: xxxx,yy";
        ok_to_make_a_kit = false;
    }else if (prix_formatted.getPartie_entiere().toInt()>9999999)
    {
        qInfo() << "Erreur dans le format du prix: Le prix dépasse la limite autorisée.";
        ok_to_make_a_kit = false;

    }
    //-----------Verifie la caution -----------
    if (caution_formatted.setValue(caution) == false)
    {
        qInfo() << "Erreur dans le format de la caution: Doit avoir le format: xxxx,yy";
        ok_to_make_a_kit = false;
    }

    //-----------Verifie qu'il y a au moins un item -----------
    if (this->kit->item_list.empty())
    {
        qInfo() << "Erreur. Vous devez ajouter au moins un sous-objet.";
        ok_to_make_a_kit = false;
    }

    if (ok_to_make_a_kit)
    {
        this->kit->setNom( nom);
        this->kit->setDescription(description);
        this->kit->setDate_achat(date_formatted);
        this->kit->setPrix_achat(prix_formatted);
        this->kit->setTexte_libre(texte_libre);
        this->kit->setEn_panne(false);
        this->kit->setCode(code);
        this->kit->setCaution( caution_formatted);
        return this->kit;
    }
    else
    {

        return NULL;
    }
}

void PoppupAddKit::set_form_from_kit(Kit * p_kit)
{
    this->ui->lineEdit_kitcaution->setText(p_kit->getCaution().getStringValue());
    this->ui->lineEdit_kitprix->setText(p_kit->getPrix_achat().getStringValue());
    this->ui->lineEdit_kit_code->setText(p_kit->getCode());
    this->ui->lineEdit_kit_dateachat->setText(p_kit->getDate_achat().toString("dd.MM.yyyy"));
    this->ui->lineEdit_kit_name->setText(p_kit->getNom());
    this->ui->textEdit_kit_description->setText(p_kit->getDescription());
    this->ui->textEdit_kit_texte_libre->setText(p_kit->getTexte_libre());

}



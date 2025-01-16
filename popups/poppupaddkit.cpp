#include "poppupaddkit.h"
#include "ui_poppupaddkit.h"
#include <QStringList>
#include <QTableWidgetItem>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QListWidgetItem>
#include <QMessageBox>

#include "../money.h"

PoppupAddKit::PoppupAddKit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PoppupAddKit)
{
    QStringList header_list;
    ui->setupUi(this);
    ui->lineEdit_kit_name->setFocus();
    this->kit = new Kit();
}

void PoppupAddKit::closeEvent( QCloseEvent* event )
{
    emit delete_popup();
    // event->accept();
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

void PoppupAddKit::GEN_raise_popup_warning(QString msg)
{

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(msg);
    msgBox.setTextFormat(Qt::TextFormat::MarkdownText);
    msgBox.setWindowTitle("Attention");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    qDebug() << msg;
}

void PoppupAddKit::on_pushButton_addobject_clicked()
{

    QString item_text = ui->lineEdit_new_item->text();
    int item_qty_init = ui->spinBox_item_quantity_init->value();
    int item_qty_current = ui->spinBox_item_quantity_current->value();
    // If user entered text
    if (item_text!= "")
    {
        //Push_back new item into private item list, before deleted items, to keep order with
        Item* p_item = new Item(0, item_text, 0, item_qty_init, item_qty_current, 0);

        int list_size = this->ui->listWidget_itemBasket->count(); //Get the number of item in listWidget_itemBasket
        this->kit->item_list.insert(this->kit->item_list.begin() + list_size,p_item);//insert the new item in item list of kit, before deleted items

        //Push_back new item into display tab
        push_back_new_item_on_tabWidget(p_item);

        p_item->setState(E_STATE_ADDED);

    }

}

void PoppupAddKit::push_back_new_item_on_tabWidget(Item* p_item)
{
    new QListWidgetItem(p_item->getName()+ " (Qté init: "+ QString::number(p_item->getQuantity_init())+")"+ " (Qté restante: "+ QString::number(p_item->getQuantity_current())+")", this->ui->listWidget_itemBasket);
}

Item* PoppupAddKit::get_item_selected()
{
    Item * p_item = NULL;
    if (this->kit->item_list.empty() == false)
    {
        //Get current row
        int currentRow = ui->listWidget_itemBasket->currentRow();


        //get item on list corresponding to item selected on listWidget
        p_item = this->kit->item_list.at(currentRow);
    }
    return p_item;

}

void PoppupAddKit::on_pushButton_deleteitemfromlist_clicked()
{
    if (this->kit->item_list.empty() == false)
    {
        //Get current row
        int currentRow = ui->listWidget_itemBasket->currentRow();

        //Delete element on tab widget
        ui->listWidget_itemBasket->takeItem(currentRow);

        //Change item state to deleted
        Item * l_item = this->kit->item_list.at(currentRow);

        if (l_item->getState() == E_STATE_ADDED)
        {
            //Item can be completely erased from list only if it has been added
            this->kit->item_list.erase(this->kit->item_list.begin()+currentRow);
        }
        else
        {
            //Item must not be erased from list in any other state
            // It state must change to "DELETED" and it must be pushed at the end of the list
            l_item->setState(E_STATE_DELETED);
            this->kit->item_list.erase(this->kit->item_list.begin()+currentRow);
            this->kit->item_list.push_back(l_item);
        }
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

    QStringList error_string_list;

    //-----------Verifie le nom------------
    if (nom =="")
    {
        error_string_list.append("Erreur dans le format du nom: Veuillez nommer le kit");
        ok_to_make_a_kit = false;
    }
    //-----------Verifie le code------------
    if (code =="")
    {
        error_string_list.append("Erreur dans le format du code: Veuillez donner un code");
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
            error_string_list.append("Erreur dans le format de la date: Doit avoir le format: JJ/MM/AAAA");
        }
    }
    else
    {
        ok_to_make_a_kit = false;
        error_string_list.append("Erreur dans le format de la date: Doit avoir le format: JJ/MM/AAAA");
    }

    //-----------Verifie le prix-----------
    if (prix_formatted.setValue(prix) == false)
    {
        error_string_list.append("Erreur dans le format du prix: Doit avoir le format: xxxx,yy");
        ok_to_make_a_kit = false;
    }else if (prix_formatted.getPartie_entiere().toInt()>9999999)
    {
        error_string_list.append("Erreur dans le format du prix: Le prix dépasse la limite autorisée.");
        ok_to_make_a_kit = false;
    }

    //-----------Verifie la caution -----------
    if (caution_formatted.setValue(caution) == false)
    {
        error_string_list.append("Erreur dans le format de la caution: Doit avoir le format: xxxx,yy");
        ok_to_make_a_kit = false;
    }

    //-----------Verifie qu'il y a au moins un item -----------
    if (this->kit->item_list.empty())
    {
        error_string_list.append("Erreur. Vous devez ajouter au moins un sous-objet.");
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
        QString pop_msg_errors;
        for(const auto& str_elem : error_string_list)
        {
            pop_msg_errors += "\n\r" + str_elem;
        }
        GEN_raise_popup_warning(pop_msg_errors);
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

    // copies "to_duplicate" to know if it is a kit to duplicate or to modify
    this->kit->setTo_duplicate(p_kit->getTo_duplicate());

    // copies idkit
    this->kit->setIdkit(p_kit->getIdKit());

    // Add items of kit
    for(const auto& item_elem : p_kit->item_list)
    {
        //Push_back new item into private item list
        Item* p_item = new Item(item_elem->getId(), item_elem->getName(), 0,item_elem->getQuantity_init(), item_elem->getQuantity_current(), item_elem->getQuantity_out());
        this->kit->item_list.push_back(p_item);
        push_back_new_item_on_tabWidget(item_elem);
    }
}

void PoppupAddKit::on_spinBox_item_quantity_init_valueChanged(int arg1)
{
    // automatically set value of current qty spinbox to init_value

    this->ui->spinBox_item_quantity_current->setValue(arg1);
}

///
/// \brief PoppupAddKit::on_listWidget_itemBasket_itemDoubleClicked
/// Function called when user wants to modify item quantities
/// \param item
///
void PoppupAddKit::on_listWidget_itemBasket_itemDoubleClicked(QListWidgetItem *item)
{
    //First get item selected by user
    Item* p_item = get_item_selected();
    if (p_item !=nullptr)
    {
        //Open popup pre filled with selected item informations
        this->popupChangeQty = new (PopuppChangeItemQuantities);
        this->popupChangeQty->set_form_from_item(p_item);
        this->popupChangeQty->setWindowTitle("Modification des quantités de l'item : "+ p_item->getName());
        this->popupChangeQty->show();
        this->setEnabled(false);//disable current window
        QObject::connect(this->popupChangeQty->getOkButton(), &QPushButton::clicked, this, &PoppupAddKit::on_popupchangeQty_ok);
        QObject::connect(this->popupChangeQty->getCancelButton(), &QPushButton::clicked, this, &PoppupAddKit::on_popupchangeQty_destroyed);
        QObject::connect(this->popupChangeQty, &PopuppChangeItemQuantities::delete_popup, this, &PoppupAddKit::on_popupchangeQty_destroyed);
    }
}

void PoppupAddKit::refresh_item_basket(void)
{
    this->ui->listWidget_itemBasket->clear();
    // Add items of kit
    for(const auto& item_elem : this->kit->item_list)
    {
        if (item_elem->getState() != E_STATE_DELETED )
        {
            push_back_new_item_on_tabWidget(item_elem);
        }
    }
}


void PoppupAddKit::on_popupchangeQty_ok()
{
    Item* p_item = get_item_selected();
    this->popupChangeQty->get_form_data(p_item);
    refresh_item_basket();

    //if current item state is added or deleted, it must remain in the same state
    if ((p_item->getState() == E_STATE_ADDED) || (p_item->getState() == E_STATE_DELETED ))
    {
    }
    else
    {
        // Otherwise it must be marked as edited
        p_item->setState(E_STATE_EDITED);
    }

    delete (this->popupChangeQty);
    this->setEnabled(true);//enable mainWindow
}

void PoppupAddKit::on_popupchangeQty_destroyed()
{
    delete (this->popupChangeQty);
    this->setEnabled(true);//enable mainWindow
}

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
        //Push_back new item into private item list
        Item* p_item = new Item(0, item_text, 0, item_qty_init, item_qty_current);
        this->kit->item_list.push_back(p_item);

        //Push_back new item into display tab
        push_back_new_item_on_tabWidget(p_item);

    }

}

void PoppupAddKit::push_back_new_item_on_tabWidget(Item* p_item)
{

    QListWidgetItem* p_WidegtItem = new QListWidgetItem(p_item->getName()+ " (Qté init: "+ QString::number(p_item->getQuantity_init())+")"+ " (Qté courante: "+ QString::number(p_item->getQuantity_current())+")", this->ui->listWidget_itemBasket);
}

void PoppupAddKit::on_pushButton_deleteitemfromlist_clicked()
{
    if (this->kit->item_list.empty() == false)
    {
        //Get current row
        int currentRow = ui->listWidget_itemBasket->currentRow();

        //Delete element on tab widget
        ui->listWidget_itemBasket->takeItem(currentRow);

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
        Item* p_item = new Item(item_elem->getId(), item_elem->getName(), 0,item_elem->getQuantity_init(), item_elem->getQuantity_current());
        this->kit->item_list.push_back(p_item);
        push_back_new_item_on_tabWidget(item_elem);
    }
}



void PoppupAddKit::on_spinBox_item_quantity_init_valueChanged(int arg1)
{
    // automatically set value of current qty spinbox to init_value

    this->ui->spinBox_item_quantity_current->setValue(arg1);
}


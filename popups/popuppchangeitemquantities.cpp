#include "popuppchangeitemquantities.h"
#include "ui_popuppchangeitemquantities.h"

#include <QMessageBox>

PopuppChangeItemQuantities::PopuppChangeItemQuantities(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PopuppChangeItemQuantities)
{
    ui->setupUi(this);
}

void PopuppChangeItemQuantities::GEN_raise_popup_warning(QString msg)
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

void PopuppChangeItemQuantities::closeEvent( QCloseEvent* event )
{
    emit delete_popup();
    // event->accept();
}

PopuppChangeItemQuantities::~PopuppChangeItemQuantities()
{
    delete ui;
}


bool PopuppChangeItemQuantities::get_form_data(Item * o_item)
{
    bool is_verif_ok = false;

    if (this->ui->spinBox_qty_init->value() < this->ui->spinBox_qty_current->value())
    {
        GEN_raise_popup_warning("Vous ne pouvez pas définir une quantité initiale plus petite que la quantité courante.");
    }
    else if (this->ui->spinBox_qty_current->value()<this->ui->spinBox_qty_out->value())
    {
        GEN_raise_popup_warning("Vous ne pouvez pas définir une quantité courante plus petite que la quantité sortie.");
    }
    else
    {
        is_verif_ok = true;
    }

    if (is_verif_ok == true)
    {
        o_item->setName(this->ui->lineEdit_nom->text());
        o_item->setQuantity_init(this->ui->spinBox_qty_init->value());
        o_item->setQuantity_current(this->ui->spinBox_qty_current->value());
        o_item->setQuantity_out(this->ui->spinBox_qty_out->value());
    }

    return is_verif_ok;
}

void PopuppChangeItemQuantities::set_form_from_item(Item * p_item)
{
    this->ui->lineEdit_nom->setText(p_item->getName());
    this->ui->spinBox_qty_init->setValue(p_item->getQuantity_init());
    this->ui->spinBox_qty_out->setValue(p_item->getQuantity_out());
    this->ui->spinBox_qty_current->setValue(p_item->getQuantity_current());

}

QPushButton* PopuppChangeItemQuantities::getOkButton(void)
{
    return ui->pushButton_OK;
}

QPushButton* PopuppChangeItemQuantities::getCancelButton(void)
{
    return ui->pushButton_Annuler;
}

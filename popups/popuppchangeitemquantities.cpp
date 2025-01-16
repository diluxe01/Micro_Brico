#include "popuppchangeitemquantities.h"
#include "ui_popuppchangeitemquantities.h"


PopuppChangeItemQuantities::PopuppChangeItemQuantities(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PopuppChangeItemQuantities)
{
    ui->setupUi(this);
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


void PopuppChangeItemQuantities::get_form_data(Item * o_item)
{
    o_item->setQuantity_init(this->ui->spinBox_qty_init->value());
    o_item->setQuantity_current(this->ui->spinBox_qty_current->value());
    o_item->setQuantity_out(this->ui->spinBox_qty_out->value());
}

void PopuppChangeItemQuantities::set_form_from_item(Item * p_item)
{
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
